#include "Client.hpp"

#include <iostream>

#include "Connection.hpp"
#include "GameUi.hpp"


static std::atomic_int64_t responses = 0 ;

Client::Client(std::shared_ptr<boost::asio::io_context> io)
    : io_ctx_{io}, connection_{nullptr}, signals_(*io_ctx_) {
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait(boost::bind(&Client::CloseConnection, this));
}

Client::~Client() {
    if (state_ != State::CLOSED) {
        this->CloseConnection();
    }
}

void Client::PerfTest() {
    auto time = std::chrono::system_clock::now();
    for (int i = 0; i < 10000; i++) {
        GetAllRooms();
    }
    while (responses < 10000);
    auto time2 = std::chrono::system_clock::now();
    auto dif = time2.time_since_epoch() - time.time_since_epoch();
    std::cout << responses << " per " << std::chrono::duration_cast<std::chrono::milliseconds>(dif).count() << "ms"
              << std::endl;
}

    void Client::Run() {
    TextDrawer drawer;
    drawer.Clear();
    drawer.DrawAuthorise();
    std::string nick;
    std::cin >> nick;
    if (nick.length() > 0) {
        nick_ = nick;
    }
    Authorise();
    while (!is_authorised) {
    }
    drawer.Clear();
    drawer.DrawBasicMenu();
    int choice = 0;
    bool stop = false;
    while (!stop) {
        fflush(stdin);
        std::cin >> choice;
        switch (choice) {
            case 1:
                GetAllRooms();
                drawer.Clear();
                while (waiting_responses_)
                    ;
                drawer.DrawGetAllRooms(rooms_);
                break;
            case 2: {
                drawer.DrawCreateRoom();
                std::string room_name, color;
                std::cin >> room_name;
                std::cout << "your color --->       ";
                std::cin >> color;
                CreateRoom(room_name, StrToColor(color));
                drawer.DrawMessage("Wait for enemy ...");
                while (!game_.is_room_full)
                    ;
                drawer.DrawMessage("Player " + game_.enemy_name +
                                   " entered, Do yo want to start (Y/n):");
                std::cout << "your choose --->       ";
                std::string resp;
                std::cin >> resp;
                if (resp == "n") {
                    LeaveRoom();
                    break;
                }
                StartGame();
                gameUI_ = std::make_shared<GameUi>();
                gameUI_->addClient(this->shared_from_this());
                gameUI_->setupRoomInfo(nick_, game_.enemy_name,
                                       ColorToStr(game_.color), room_name);
                gameUI_->setupMsg("Started");
                gameUI_->start();
                LeaveRoom();
                drawer.Clear();
                drawer.DrawBasicMenu();
                break;
            }
            case 3: {
                drawer.DrawEnterRoom();
                std::string room_name;
                std::cin >> room_name;
                EnterRoom(room_name);
                while (!game_.is_in_room)
                    ;
                drawer.DrawMessage(
                    "You entered\nLets wait for start from host");
                while (!game_.is_started)
                    ;
                drawer.DrawMessage("Game started!");
                gameUI_ = std::make_shared<GameUi>();
                gameUI_->addClient(this->shared_from_this());
                gameUI_->setupRoomInfo(nick_, game_.enemy_name,
                                       ColorToStr(game_.color), room_name);
                gameUI_->setupMsg("Started");
                gameUI_->start();
                LeaveRoom();
                drawer.Clear();
                drawer.DrawBasicMenu();
                break;
            }
            case 4:
                stop = true;
            default:
                drawer.DrawMessage("There's no this choice, try again");
                sleep(1);
                drawer.Clear();
                drawer.DrawBasicMenu();
        }
    }
}

void Client::GetAllRooms() {
    GetRoomsRequest req;
    Write(req.toJSON());
    waiting_responses_++;
}

void Client::Authorise() {
    AuthRequest req(nick_);
    Write(req.toJSON());
    waiting_responses_++;
}

void Client::EnterRoom(const std::string& id) {
    EnterRoomRequest req(id);
    Write(req.toJSON());
    waiting_responses_++;
}

void Client::LeaveRoom() {
    LeaveRoomRequest req;
    game_.reset();
    Write(req.toJSON());
    waiting_responses_++;
}

move_status Client::MoveFigure(const std::string& fromTo) {
    state_ = State::PERFORMING_ACTION;
    MoveFigureRequest req(fromTo);
    Write(req.toJSON());
    waiting_responses_++;
    while (waiting_responses_)
        ;
    return game_.last_move;
}

void Client::StartGame() {
    StartGameRequest req;
    Write(req.toJSON());
    waiting_responses_++;
}

void Client::CreateRoom(const std::string& name, const figure_color& color) {
    CreateRoomRequest req(name, color);
    game_.color = color;
    game_.is_your_turn = color == WHITE;
    Write(req.toJSON());
    waiting_responses_++;
}

void Client::Connect(std::string_view path, std::string_view port) {
    connection_ =
        std::make_shared<client::Connection>(this->weak_from_this(), io_ctx_);
    connection_->Connect(path, port);
}

void Client::Write(std::string text) { connection_->Write(std::move(text)); }

void Client::SetState(State state) noexcept {
    std::lock_guard<std::mutex> lock{mutex_};
    state_ = state;
}

Client::State Client::GetState() const noexcept {
    std::lock_guard<std::mutex> lock{mutex_};
    return state_;
}

void Client::CloseConnection() {
    LeaveRoom();
    connection_->Close();
}

void Client::HandleMessage(Response&& response) {
    response_ = std::move(response);
    std::lock_guard<std::mutex> lock{mutex_};
    switch (response_.resp_type) {
        case QueryType::UNDEFINED:
            std::cout << "undefined";
            break;
        case QueryType::CREATE_ROOM:
            handleCreateRoom(response_.resp_data);
            break;
        case QueryType::MOVE_FIGURE:
            handleMoveFigure(response_.resp_data);
            break;
        case QueryType::START_GAME:
            handleStartGame(response_.resp_data);
            break;
        case QueryType::LEAVE_ROOM:
            handleLeaveRoom(response_.resp_data);
            break;
        case QueryType::ENTER_ROOM:
            handleEnterRoom(response_.resp_data);
            break;
        case QueryType::AUTHORISE:
            handleAuth(response_.resp_data);
            break;
        case QueryType::GET_ROOMS:
            handleGetAllRooms(response_.resp_data);
            break;
        default:
            break;
    }
}

void Client::handleCreateRoom(const std::string& data) {
    connection_->WriteLog(LogType::info, "handling creating rooms  ... \n");
    connection_->WriteLog(LogType::info, data);
    CreateRoomResponse response;
    response.parse(data);
    if (response.status == 0) {
        game_.is_in_room = true;
        waiting_responses_--;
        return;
    }
    waiting_responses_--;
    game_.is_in_room = false;
}

void Client::handleAuth(const std::string& data) {
    connection_->WriteLog(LogType::info, "handling authorising  ... \n");
    connection_->WriteLog(LogType::info, data);
    AuthResponse response;
    response.parse(data);
    if (response.status == 0) {
        is_authorised = true;
        waiting_responses_--;
        return;
    }
    is_authorised = false;
    waiting_responses_--;
}

void Client::handleEnterRoom(const std::string& data) {
    connection_->WriteLog(LogType::info, "handling entering room  ... \n");
    connection_->WriteLog(LogType::info, data);
    EnterRoomResponse response;
    response.parse(data);
    if (response.status == 0) {
        game_.enemy_name = response.enemy_name;
        if (!game_.is_in_room) {
            game_.is_in_room = true;
            game_.color = response.player_color;
            game_.is_your_turn = response.player_color == figure_color::WHITE;
        }
        game_.is_room_full = true;
        if (waiting_responses_) waiting_responses_--;
        return;
    }
    game_.is_in_room = false;
    if (waiting_responses_) waiting_responses_--;
}

void Client::handleLeaveRoom(const std::string& data) {
    connection_->WriteLog(LogType::info, "handling leaving room  ... \n");
    connection_->WriteLog(LogType::info, data);
    waiting_responses_--;
}

void Client::handleGetAllRooms(const std::string& data) {
    connection_->WriteLog(LogType::info, "handling getting rooms  ... \n");
    connection_->WriteLog(LogType::info, data);
    GetRoomsResponse response;
    response.parse(data);
    rooms_ = response.rooms;
    waiting_responses_--;
    responses++;
}

void Client::handleMoveFigure(const std::string& data) {
    GameResponse response;
    response.parse(data);
    if (response.moveStatus != MOVE_ERROR) {
        if (game_.is_your_turn) {
            game_.is_your_turn = false;
        } else {
            game_.is_your_turn = true;
        }
        game_.FEN_game_state = response.tableFEN;

        if (response.moveStatus == MOVE_CHECKMATE_WHITE) {
            gameUI_->finishGame();
        }
        if (response.moveStatus == MOVE_CHECKMATE_BLACK) {
            gameUI_->finishGame();
        }
        gameUI_->makeMove(response.move_str);
    }
    gameUI_->setupMsg(
        MoveStatusAsString(static_cast<move_status>(response.moveStatus)));
    state_ = State::READY;
    game_.last_move = move_status(response.moveStatus);
    chan_.moves_chan_.Push(response.move_str);
    if (waiting_responses_) waiting_responses_--;
}

void Client::handleStartGame(const std::string& data) {
    game_.is_started = true;
    if (waiting_responses_) waiting_responses_--;
}
