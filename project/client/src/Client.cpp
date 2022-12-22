#include "Client.hpp"

#include <iostream>

#include "Connection.hpp"

void makeMove(std::shared_ptr<GUIFactory> gui, std::string mov, std::map<std::string, GUIObj*>& figPos) {
    auto from = mov.substr(0, 2);
    auto to = mov.substr(2, 2);
    if (figPos.find(to) != figPos.end()) {
        gui->remove(figPos[to]);
    }
    figPos[to] = figPos[from];
    figPos.erase(from);
    auto [x, y] = cell(to);
    dynamic_cast<SFMLSprite*>(figPos[to]) -> x(x)
            -> y(y);
}


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
    while (!is_authorised) {}
    drawer.Clear();
    drawer.DrawBasicMenu();
    int choice = 0;
    bool stop = false;
    while (!stop){
        fflush(stdin);
        std::cin >> choice;
        switch (choice) {
            case 1:
                GetAllRooms();
                drawer.Clear();
                drawer.DrawGetAllRooms(rooms_);
                break;
            case 2: {
//                drawer.Clear();
                drawer.DrawCreateRoom();
                std::string room_name, color;
                std::cin >> room_name;
                std::cout << "your color --->       ";
                std::cin >> color;
                CreateRoom(room_name, StrToColor(color));
                while(waiting_responses_);
                if (game_.is_in_room){
                    std::thread tr ([this](){
                        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chess");
                        std::shared_ptr<GUIFactory> gui(new SFMLGUIFactory(&window));

                        std::shared_ptr<MoveChan> chan_ptr = std::make_shared<MoveChan>(chan_);
                        std::map<std::string, GUIObj*> figPos;
                        setupBoard(gui, chan_ptr, figPos);
                        
                        std::string player1 = "First Player";
                        std::string player2 = "Second Player";
                        std::string room = "This room's name";
                        setupInfo(gui, player1, player2, room);
                        
                        std::cout << "started" << std::endl;

                        while (gui->handleEvents()) {
                            if (game_.is_room_full) {
                                if (!game_.is_started) {
                                    StartGame();
                                    game_.is_started = true;
                                }
                            }
                            if (game_.is_started) {
                                while (!chan_.moves_chan_.empty()) {
                                    std::cout << "ACTION" << std::endl;
                                    auto move = chan_.moves_chan_.TryPop();
                                    std::cerr << move << '\n';
                                    if (move.length() > 0) {
                                        if (MoveFigure(move) != move_status::MOVE_ERROR && (move !=  chan_.last_move_ )) {
                                            makeMove(gui, move, figPos);
                                        }
                                    }
                                }
                            }
//                            std::cout << "handling" << std::endl;
                            gui->display();
                        }
                    });
                    tr.detach();
                }
                drawer.Clear();
                drawer.DrawBasicMenu();
            }
            case 3: {
//                drawer.Clear();
                drawer.DrawEnterRoom();
                std::string room_name;
                std::cin >> room_name;
                EnterRoom(room_name);
                while(waiting_responses_);
                if (game_.is_in_room){
                    std::thread tr ([this]() {
                        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chess");
                        std::shared_ptr<GUIFactory> gui(new SFMLGUIFactory(&window));
                        auto chan_ptr = std::make_shared<MoveChan>(chan_);
                        std::map<std::string, GUIObj*> figPos;
                        setupBoard(gui, chan_ptr,  figPos);

                        std::string player1 = "First Player";
                        std::string player2 = "Second Player";
                        std::string room = "This room's name";
                        setupInfo(gui, player1, player2, room);
                        gui->display();
                        std::cout << "wait" << std::endl;
                        while (!game_.is_room_full && !game_.is_started);
                        std::cout << "started" << std::endl;
                        while (gui->handleEvents()) {
                            while (!chan_.moves_chan_.empty()) {
                                std::string move =chan_.moves_chan_.TryPop();
                                std::cerr << move << '\n';
                                if (MoveFigure(move) != move_status::MOVE_ERROR && (move !=  chan_.last_move_ )){
                                    makeMove(gui, move, figPos);
                                }
                            }
                            gui->display();
                        }
                    });
                    tr.detach();
                }
                break;
            }
            case 4:
                stop = true;
            default:
                std::cout << "There's no this choice, try again" << std::endl;
                sleep(1);
                drawer.Clear();
                drawer.DrawBasicMenu();
        }
    }
}

bool Client::IsGameStarted() const noexcept { return game_.is_started; }
bool Client::IsGameFinished() const noexcept { return game_.is_finished; }
bool Client::IsYourTurn() const noexcept {
    if (state_ == State::PERFORMING_ACTION){
        return false;
    }

    return game_.is_your_turn;
}

std::string Client::GetFENState() const noexcept {
    return game_.FEN_game_state;
}

figure_color Client::GetColor() const noexcept { return game_.color; }

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
    Write(req.toJSON());
    waiting_responses_++;
}

move_status Client::MoveFigure(const std::string& fromTo) {
    state_ = State::PERFORMING_ACTION;
    MoveFigureRequest req(fromTo);
    Write(req.toJSON());
    waiting_responses_++;
    while (waiting_responses_);
    return game_.last_move;
}

void Client::StartGame(){
    StartGameRequest req;
    Write(req.toJSON());
    waiting_responses_++;
}

void Client::CreateRoom(const std::string& name, const figure_color& color) {
    CreateRoomRequest req(name, color);
    game_.color = color;
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

void Client::CloseConnection() { connection_->Close(); }

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
    connection_->WriteLog(LogType::info,  "handling authorising  ... \n");
    connection_->WriteLog(LogType::info,  data);
    AuthResponse response;
    response.parse(data);
    if (response.status == 0){
        is_authorised = true;
        waiting_responses_--;
        return;
    }
    is_authorised = false;
    waiting_responses_--;
}

void Client::handleEnterRoom(const std::string& data) {
    connection_->WriteLog(LogType::info,  "handling entering room  ... \n");
    connection_->WriteLog(LogType::info,  data);
    std::cout << data;
    EnterRoomResponse response;
    response.parse(data);
    if (response.status == 0){
        if (!game_.is_in_room){
            game_.is_in_room = true;
            game_.color = response.player_color;
            game_.is_your_turn = response.player_color == figure_color::WHITE;
        }
        game_.is_room_full = true;
        if ( waiting_responses_) waiting_responses_--;
        return;
    }
    game_.is_in_room = false;
    if ( waiting_responses_) waiting_responses_--;
}

void Client::handleLeaveRoom(const std::string& data) {
    std::cout << "handling leaving room ... " << std::endl;
    std::cout << data << std::endl;
    waiting_responses_--;
}


void Client::handleGetAllRooms(const std::string& data) {
    connection_->WriteLog(LogType::info,  "handling getting rooms  ... \n");
    connection_->WriteLog(LogType::info,  data);
    GetRoomsResponse response;
    response.parse(data);
    rooms_ = response.rooms;
    waiting_responses_--;
}

void Client::handleMoveFigure(const std::string& data) {
//    std::cout << "handling moving figure ... " << std::endl;
    GameResponse response;
    response.parse(data);
    if (response.moveStatus != MOVE_ERROR) {
        game_.FEN_game_state = response.tableFEN;
        if (response.moveStatus == MOVE_CHECK_WHITE) {
            std::cout << "Check white" << std::endl;
        }
        if (response.moveStatus == MOVE_CHECKMATE_WHITE) {
            std::cout << "Check and Mate white" << std::endl;
            game_.is_finished = true;
        }
        if (response.moveStatus == MOVE_CHECK_BLACK) {
            std::cout << "Check black" << std::endl;
        }
        if (response.moveStatus == MOVE_CHECKMATE_BLACK) {
            std::cout << "Check mate black" << std::endl;
            game_.is_finished = true;
        }
        if (response.moveStatus == MOVE_OK) {
            std::cout << "Successful move" << std::endl;
        }
        game_.is_your_turn ? true : false;

    } else {
        response.moveStatus = MOVE_ERROR;
        if (response.moveStatus) {
            std::cout << "Wrong move" << std::endl;
        }
    }
    state_ = State::READY;
    game_.last_move = move_status(response.moveStatus);
    chan_.moves_chan_.Push(response.move_str);
    if ( waiting_responses_) waiting_responses_--;
}

void Client::handleStartGame(const std::string& data) {
    std::cout << "handling starting game... " << std::endl;
    game_.is_started = true;
    if ( waiting_responses_) waiting_responses_--;
}
