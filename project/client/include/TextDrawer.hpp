#pragma once
#include "tabulate/table.hpp"
#include "Base.hpp"

using namespace tabulate;
using Row_t = Table::Row_t;

class TextDrawer{
public:
    void DrawRoom(std::string host_nick, std::string room_name, GameState state) {
        Table menu;
        menu.format()
                .border_color(Color::white)
                .padding_left(4)
                .padding_right(4);

        menu.add_row(Row_t{"Welcome to Room, here you can play chess"});
        menu[0].format().font_align(FontAlign::center).font_color(Color::magenta);

        Table room_data_table;
        room_data_table.add_row(Row_t{"Host", "Room name"});
        room_data_table[0].format().font_align(FontAlign::center).color(tabulate::Color::white);
        room_data_table.add_row(Row_t{host_nick, room_name});
        room_data_table[0].format().font_align(FontAlign::center).color(tabulate::Color::white);
        menu.add_row(Row_t{room_data_table});
        menu[1].format().font_align(FontAlign::center).hide_border_top();

        Table empty_row;
        empty_row.format().hide_border();
        menu.add_row(Row_t{empty_row});
        menu[2].format().hide_border_left().hide_border_right();

        Table chess_table;
        for (int i = 0; i < 8; i++){
            chess_table.add_row(Row_t{"|   |   |   |   |   |   |   |"});
        }
        menu.add_row(Row_t{chess_table});
        menu[3].format().font_align(FontAlign::center).hide_border_top();

        if (state.is_your_turn && state.is_started) {
            menu.add_row(Row_t{"Please enter your move (ex: E2E4): "});
            menu[4]
                    .format()
                    .font_align(FontAlign::center)
                    .font_style({FontStyle::underline, FontStyle::italic})
                    .font_color(Color::white)
                    .hide_border_top();
            std::cout << menu << "\n";
            std::cout << "--->       ";
            return;
        } else if (!state.is_your_turn && state.is_started){
            menu.add_row(Row_t{"Wait for enemy move: "});
            menu[4]
                    .format()
                    .font_align(FontAlign::center)
                    .font_style({FontStyle::underline, FontStyle::italic})
                    .font_color(Color::white)
                    .hide_border_top();
            return;
        }
        menu.add_row(Row_t{"Wait for somemody else in room: "});
        menu[4]
                .format()
                .font_align(FontAlign::center)
                .font_style({FontStyle::underline, FontStyle::italic})
                .font_color(Color::white)
                .hide_border_top();
        std::cout << menu << "\n";
        std::cout << "--->       ";
        return;
    }

    void Clear(){
        system("clear");
    }
    void DrawCreateRoom(){
        Table menu;
        menu.add_row(Row_t{"Please enter room name, your color (black / white): "});
        menu[0]
                .format()
                .font_align(FontAlign::center)
                .font_style({FontStyle::underline, FontStyle::italic})
                .font_color(Color::white)
                .hide_border_top();
        std::cout << menu << "\n";
        std::cout << "room name --->       ";
    }
    void DrawEnterRoom(){
        Table menu;
        menu.add_row(Row_t{"Please enter room name to enter: "});
        menu[0]
                .format()
                .font_align(FontAlign::center)
                .font_style({FontStyle::underline, FontStyle::italic})
                .font_color(Color::white)
                .hide_border_top();
        std::cout << menu << "\n";
        std::cout << "room name --->       ";
    }

    void DrawGetAllRooms(std::map<std::string, RoomData> rooms_data) {
        Table menu;
        menu.format()
                .border_color(Color::white)
                .padding_left(4)
                .padding_right(4);

        menu.add_row(Row_t{"All rooms are here"});
        menu[0].format().font_align(FontAlign::center).font_color(Color::magenta);
        Table rooms;
        rooms.add_row(Row_t{"Room id", "Room name", "Host nick", "Host color"});
        rooms[0].format().font_align(FontAlign::center).color(tabulate::Color::white);
        for (const auto& r : rooms_data){
            rooms.add_row(Row_t{r.second.room_id, r.second.room_name, r.second.host_nick, ColorToStr(r.second.host_color)});
        }
        menu.add_row(Row_t{rooms});
        menu[1].format().font_align(FontAlign::center).hide_border_top();

        menu.add_row(Row_t{"Here you can choose action"});
        menu[2].format().font_align(FontAlign::center).font_color(Color::magenta);
        Table highlights;
        highlights.add_row(Row_t{"1 Get Rooms", "2 Create Room", "3 Enter Room", "4 Exit Game"});
        menu.add_row(Row_t{highlights});
        menu[3].format().font_align(FontAlign::center).hide_border_top();

        menu.add_row(Row_t{"Please enter yor choice (ex 1): "});
        menu[4]
                .format()
                .font_align(FontAlign::center)
                .font_style({FontStyle::underline, FontStyle::italic})
                .font_color(Color::white)
                .hide_border_top();
        std::cout << menu << "\n";
        std::cout << "--->       ";
    }

    void DrawBasicMenu() {
        Table menu;
        menu.format()
                .border_color(Color::white)
                .padding_left(4)
                .padding_right(4);

        menu.add_row(Row_t{"Welcome to Basic Menu, here you can choose action"});
        menu[0].format().font_align(FontAlign::center).font_color(Color::magenta);

        Table highlights;
        highlights.add_row(Row_t{"1 Get Rooms", "2 Create Room", "3 Enter Room", "4 Exit Game"});
        menu.add_row(Row_t{highlights});
        menu[1].format().font_align(FontAlign::center).hide_border_top();

        menu.add_row(Row_t{"Please enter yor choice (ex 1): "});
        menu[2]
                .format()
                .font_align(FontAlign::center)
                .font_style({FontStyle::underline, FontStyle::italic})
                .font_color(Color::white)
                .hide_border_top();
        std::cout << menu << "\n";
        std::cout << "--->       ";
    }

    void DrawAuthorise(){
        Table auth_table;
        auth_table.format()
                    .border_color(Color::white)
                    .padding_left(4)
                    .padding_right(4);

        auth_table.add_row(Row_t{"Welcome to C+- client server chess project"});
        auth_table[0].format().font_align(FontAlign::center).font_color(Color::magenta);

        auth_table.add_row(Row_t{"Please enter yor nick: "});
        auth_table[1]
                .format()
                .font_align(FontAlign::center)
                .font_style({FontStyle::underline, FontStyle::italic})
                .font_color(Color::white)
                .hide_border_top();
        std::cout << auth_table << "\n--->       ";
    }
};