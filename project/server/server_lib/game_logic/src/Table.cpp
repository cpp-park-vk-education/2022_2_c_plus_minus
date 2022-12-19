#include "Table.hpp"

#include "iostream"

Figure *ChessTable::get_figure(int index) {
    if ((index >= 0) && (index < BOARD_SIZE)) {
        return ChessTable::table[index];
    }
    return nullptr;
}

ChessTable::ChessTable(const ChessTable &mainTable) {
    // копируем доску

    FEN_parser(mainTable.get_FEN());
}

move_status ChessTable::is_legal(int from, int to) {
    Figure *actor = ChessTable::table[from];
    figure_color enemy_color = (actor->color == WHITE) ? BLACK : WHITE;
    coords coords_from = data_to_coords(from);
    coords coords_to = data_to_coords(to);
    move_status result = MOVE_ERROR;

    bool legality = false;
    bool frendly_fire = false;
    if (actor->name == "King") {
        if ((actor->color == BLACK) && (from == 60) &&
            ((to == 62) || (to == 58))) {  // рокировка
            if (ChessTable::check_cast(actor, enemy_color, coords_from,
                                       coords_to)) {
                result = MOVE_CAST;
                return result;
            }
        } else if ((actor->color == WHITE) && (from == 4) &&
                   ((to == 2) || (to == 6))) {  // рокировка
            if (ChessTable::check_cast(actor, enemy_color, coords_from,
                                       coords_to)) {
                result = MOVE_CAST;
                return result;
            }
        } else if ((abs(coords_from.x - coords_to.x) <= 1) &&
                   (abs(coords_from.y - coords_to.y) <= 1)) {
            bool enemy_hit = ChessTable::get_hit_table(
                enemy_color, coords_to);  // не попалали под шах?
            if (ChessTable::get_figure(to) != nullptr) {
                frendly_fire =
                    (ChessTable::get_figure(to)->color) != enemy_color;
            }
            if ((!enemy_hit) && (!frendly_fire)) {
                result = MOVE_OK;
            }
            return result;
        }
    } else if (actor->name == "Queen") {
        legality = ChessTable::Queen_move(actor->color, coords_from, coords_to);
        if (ChessTable::get_figure(to) != nullptr) {
            frendly_fire = (ChessTable::get_figure(to)->color) != enemy_color;
        }
        if ((legality) && (!frendly_fire)) {
            result = MOVE_OK;
        }
        return result;
    } else if (actor->name == "Bishop") {
        legality =
            ChessTable::Bishop_move(actor->color, coords_from, coords_to);
        if (ChessTable::get_figure(to) != nullptr) {
            frendly_fire = (ChessTable::get_figure(to)->color) != enemy_color;
        }
        if ((legality) && (!frendly_fire)) {
            result = MOVE_OK;
        }

        return result;
    } else if (actor->name == "Knight") {
        if (((abs(coords_from.x - coords_to.x) == 2) &&
             (abs(coords_from.y - coords_to.y) == 1)) ||
            ((abs(coords_from.x - coords_to.x) == 1) &&
             (abs(coords_from.y - coords_to.y) == 2))) {
            if (ChessTable::get_figure(to) != nullptr) {
                frendly_fire =
                    (ChessTable::get_figure(to)->color) != enemy_color;
            }
            if ((!frendly_fire)) {
                result = MOVE_OK;
            }

            return result;
        }
    } else if (actor->name == "Rock") {
        legality = ChessTable::Rock_move(actor->color, coords_from, coords_to);
        if (ChessTable::get_figure(to) != nullptr) {
            frendly_fire = (ChessTable::get_figure(to)->color) != enemy_color;
        }
        if ((legality) && (!frendly_fire)) {
            result = MOVE_OK;
        }
        return result;
    } else if (actor->name == "Pawn") {
        if ((coords_from.x == coords_to.x) &&
            ((coords_from.y == 1) || (coords_from.y == 6)))  // начальный ход
        {
            legality = ((coords_to.y - coords_from.y) == 2 * actor->color) &&
                       (ChessTable::get_figure(
                            coords_to.x + (coords_to.y - actor->color) * 8) ==
                        nullptr) &&
                       ((ChessTable::get_figure(coords_to.index) == nullptr));
            if (legality) {
                result = MOVE_SET_ENPASSANT;
                return result;
            }
        } else if ((abs(coords_from.x - coords_to.x) == 1) &&
                   ((coords_to.y - coords_from.y) == actor->color))  // сьесть
        {
            if ((ChessTable::check_en_passant(from, to)) && (!frendly_fire)) {
                result = MOVE_ENPASSANT;
                return result;
            }
            if (ChessTable::get_figure(to) != nullptr) {
                legality = (ChessTable::get_figure(to)->color == enemy_color);
            }
        }
        if (!legality) {
            legality = (((coords_from.x == coords_to.x)) &&
                        ((coords_to.y - coords_from.y) ==
                         actor->color));  // обычный ход
        }
        if (ChessTable::get_figure(to) != nullptr) {
            if (ChessTable::get_figure(to)->color != enemy_color) {
                legality = false;
            }
        }
        if (legality) {
            result = MOVE_OK;
        }
        return result;
    }

    return result;
}
void ChessTable::kill_figure(int index) {
    if (ChessTable::get_figure(index) != nullptr) {
        if (ChessTable::get_figure(index)->color == WHITE) {
            ChessTable::whiteDeadFigures +=
                ChessTable::get_figure(index)->FEN_name;
        } else if (ChessTable::get_figure(index)->color == BLACK) {
            ChessTable::blackDeadFigures +=
                ChessTable::get_figure(index)->FEN_name;
        }

        delete ChessTable::get_figure(index);
        ChessTable::table[index] = nullptr;
    }
}
void ChessTable::make_move(int from, int to) {
    if (ChessTable::get_figure(to) == nullptr) {
        ChessTable::table[to] = ChessTable::table[from];
        ChessTable::table[from] = nullptr;
    } else {
        ChessTable::kill_figure(to);
        ChessTable::table[to] = ChessTable::table[from];
        ChessTable::table[from] = NULL;
    }
}
move_status ChessTable::move_figure(int from, int to) {
    // проверяем легальность хода
    figure_color color = ChessTable::get_figure(from)->color;
    figure_color enemy_color = (color == WHITE) ? BLACK : WHITE;
    move_status result = ChessTable::is_legal(from, to);
    if (result == MOVE_ERROR) {
        std::cout << "NOT LEGAL" << std::endl;  // DEBUG
        return MOVE_ERROR;
    }
    // проверяем состояние короля
    // проверяем, будет ли нам шах или мат( даже если нам)
    int temp_White_king_ind = White_king_ind;
    int temp_Black_king_ind = Black_king_ind;
    if (from == White_king_ind) {
        temp_White_king_ind = to;
    } else if (from == Black_king_ind) {
        temp_Black_king_ind = to;
    }

    ChessTable tempTable;  // копия класса
    tempTable.FEN_parser(get_FEN());
    tempTable.make_move(from, to);
    coords king;
    if ((get_figure(from)->color) == WHITE) {
        king = data_to_coords(temp_White_king_ind);
    } else {
        king = data_to_coords(temp_Black_king_ind);
    }
    bool checked = tempTable.get_hit_table(enemy_color, king);
    std::cout << checked << std::endl;  // DEBUG
    if (checked) {
        return MOVE_ERROR;
    }
    // считываем последствия (убили, превратились, эн пассант, рокировка, не
    // рокировка)
    White_king_ind = temp_White_king_ind;
    Black_king_ind = temp_Black_king_ind;


    if (get_figure(from)->color == WHITE) {
        while (!white_enpassant.empty()) {
            int get_fig = white_enpassant.back();
            if (get_figure(get_fig) != NULL) {
                Pawn *temp_pawn = static_cast<Pawn *>(get_figure(get_fig));
                temp_pawn->enPassant = false;
            }
            white_enpassant.pop_back();
        }
    }
    if (get_figure(from)->color == BLACK) {
        while (!black_enpassant.empty()) {
            int get_fig = black_enpassant.back();
            if (get_figure(get_fig) != NULL) {
                Pawn *temp_pawn = static_cast<Pawn *>(get_figure(get_fig));
                temp_pawn->enPassant = false;
            }
            black_enpassant.pop_back();
        }
    }

    if (result == MOVE_OK) {
        if (get_figure(to) !=
            NULL) {  // отсутствие френдли фаера гарантированно проверкой
                     // легальности хода
            result = MOVE_EAT;
        }
        make_move(from, to);  // ходим

        // обработка удаления рокировки
        if (get_figure(to)->name == "Rock") {
            if (get_figure(to)->color == WHITE) {
                King *temp_king =
                    static_cast<King *>(get_figure(White_king_ind));
                if (temp_king->cast_k) {
                    if (from = 7) {
                        temp_king->cast_k = 0;
                    }
                } else if (temp_king->cast_q) {
                    if (from = 0) {
                        temp_king->cast_q = 0;
                    }
                }
            }
            if (get_figure(to)->color == BLACK) {
                King *temp_king =
                    static_cast<King *>(get_figure(Black_king_ind));
                if (temp_king->cast_k) {
                    if (from = 63) {
                        temp_king->cast_k = 0;
                    }
                } else if (temp_king->cast_q) {
                    if (from = 56) {
                        temp_king->cast_q = 0;
                    }
                }
            }
        } else if (get_figure(to)->name == "King") {
            if (get_figure(to)->color == WHITE) {
                King *temp_king =
                    static_cast<King *>(get_figure(White_king_ind));
                temp_king->cast_k = 0;
                temp_king->cast_q = 0;
            }
            if (get_figure(to)->color == BLACK) {
                King *temp_king =
                    static_cast<King *>(get_figure(Black_king_ind));
                temp_king->cast_k = 0;
                temp_king->cast_q = 0;
            }
        }
    } else if (result == MOVE_CAST) {
        make_move(from, to);
        // если подвинули короля, то нужно подвинуть и ладью. будем считать, что
        // положение ладби на начальной позиции гарантированно
        if (to == 62) {
            make_move(63, 61);
        } else if (to == 58) {
            make_move(56, 59);
        } else if (to == 2) {
            make_move(0, 3);
        } else if (to == 6) {
            make_move(7, 5);
        }
        if (get_figure(to)->color == WHITE) {
            King *temp_king = static_cast<King *>(get_figure(White_king_ind));
            temp_king->cast_k = 0;
            temp_king->cast_q = 0;
        }
        if (get_figure(to)->color == BLACK) {
            King *temp_king = static_cast<King *>(get_figure(Black_king_ind));
            temp_king->cast_k = 0;
            temp_king->cast_q = 0;
        }
    } else if (result == MOVE_ENPASSANT) {
        make_move(from, to);
        kill_figure(to - 8 * get_figure(to)->color);
    } else if (result == MOVE_SET_ENPASSANT) {
        make_move(from, to);
        Pawn *temp_pawn = static_cast<Pawn *>(get_figure(to));
        temp_pawn->enPassant = true;
        if (temp_pawn->color == WHITE) {
            white_enpassant.push_back(to);
        }
        if (temp_pawn->color == BLACK) {
            black_enpassant.push_back(to);
        }
    }

    // считываем последствия для врага

    bool draw = check_draw(enemy_color);
    if (enemy_color == WHITE) {
        king = data_to_coords(White_king_ind);
    } else {
        king = data_to_coords(Black_king_ind);
    }
    bool enemy_cheked = get_hit_table(get_figure(to)->color, king);

    if (draw && !enemy_cheked) {
        result = MOVE_DRAW;
        return result;
    } else if (!draw && enemy_cheked) {
        if (enemy_color == WHITE) {
            result = MOVE_CHECK_WHITE;
        } else {
            result = MOVE_CHECK_BLACK;
        }
    } else if (draw && enemy_cheked) {
        if (enemy_color == WHITE) {
            result = MOVE_CHECKMATE_WHITE;
        } else {
            result = MOVE_CHECKMATE_BLACK;
        }
    }
    return result;
}
bool ChessTable::check_draw(figure_color enemy_color) {  // не оптимизированно
    bool result = true;
    figure_color non_enemy_color = (enemy_color == WHITE) ? BLACK : WHITE;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (table[i] != NULL) {
            if (table[i]->color == enemy_color) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    move_status move_result = ChessTable::is_legal(i, j);
                    if (move_result != MOVE_ERROR) {
                        int temp_White_king_ind = White_king_ind;
                        int temp_Black_king_ind = Black_king_ind;
                        if (i == White_king_ind) {
                            temp_White_king_ind = j;
                        } else if (i == Black_king_ind) {
                            temp_Black_king_ind = j;
                        }

                        ChessTable tempTable;  // копия класса
                        tempTable.FEN_parser(get_FEN());
                        tempTable.make_move(i, j);
                        coords king;
                        if (enemy_color == WHITE) {
                            king = data_to_coords(temp_White_king_ind);
                        } else {
                            king = data_to_coords(temp_Black_king_ind);
                        }
                        bool checked =
                            tempTable.get_hit_table(non_enemy_color, king);
                        if (!checked) {
                            result = false;
                            return result;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool ChessTable::Bishop_move(figure_color color, coords from, coords to) {
    if (abs(from.x - to.x) != abs(from.y - to.y)) {
        return false;
    }
    if (ChessTable::get_figure(to.index) !=
        NULL) {  // отдельно проверим конечную клетку на легальность
        if (ChessTable::get_figure(to.index)->color == color) {
            return false;
        }
    }
    int delta = abs(to.x - from.x);
    int dir_x = ((to.x - from.x) > 0) ? 1 : -1;
    int dir_y = ((to.y - from.y) > 0) ? 1 : -1;

    for (int i = 1; i < delta; i++) {
        if (ChessTable::get_figure((from.x + i * dir_x) +
                                   (from.y + i * dir_y) * 8) != NULL) {
            return false;
        }
    }
    return true;
}

bool ChessTable::Rock_move(figure_color color, coords from, coords to) {
    if ((from.x == to.x) && (from.y != to.y) && (from.y >= 0) && (from.y < 8) &&
        (to.x >= 0) && (to.x < 8)) {
        if (from.y < to.y) {
            for (int i = from.y + 1; i < to.y; i++) {
                if (ChessTable::get_figure(from.x + i * 8) != NULL) {
                    return false;
                }
            }
        } else if (from.y > to.y) {
            for (int i = from.y - 1; i > to.y; i--) {
                if (ChessTable::get_figure(from.x + i * 8) != NULL) {
                    return false;
                }
            }
        }
        if (ChessTable::get_figure(to.index) !=
            NULL) {  // отдельно проверим конечную клетку на легальность
            if (ChessTable::get_figure(to.index)->color == color) {
                return false;
            }
        }
        return true;
    } else if (((from.y == to.y) && (from.x != to.x)) && (from.y >= 0) &&
               (from.y < 8) && (to.x >= 0) && (to.x < 8)) {
        if (from.x < to.x) {
            for (int i = from.x + 1; i < to.x; i++) {
                if (ChessTable::get_figure(i + from.y * 8) != NULL) {
                    return false;
                }
            }
            if (ChessTable::get_figure(to.index) !=
                NULL) {  // отдельно проверим конечную клетку на легальность
                if (ChessTable::get_figure(to.index)->color == color) {
                    return false;
                }
            }
            return true;
        } else if (from.x > to.x) {
            for (int i = from.x - 1; i > to.x; i--) {
                if (ChessTable::get_figure(i + from.y * 8) != NULL) {
                    return false;
                }
            }
            if (ChessTable::get_figure(to.index) !=
                NULL) {  // отдельно проверим конечную клетку на легальность
                if (ChessTable::get_figure(to.index)->color == color) {
                    return false;
                }
            }
            return true;
        }
    }

    return false;
}

bool ChessTable::Queen_move(figure_color color, coords from, coords to) {
    bool result = ChessTable::Bishop_move(color, from, to) ||
                  ChessTable::Rock_move(color, from, to);
    return result;
}

bool ChessTable::check_cast(Figure *fig_actor, figure_color enemy, coords from,
                            coords to) {
    King *actor = static_cast<King *>(fig_actor);
    bool result = false;
    if (actor->color == BLACK) {
        if ((from.index == 60) && (to.index == 62) &&
            actor->cast_k) {  // рокировка
            coords empty_cell = data_to_coords(61);
            result = (ChessTable::get_figure(61) == NULL) &&
                     (!ChessTable::get_hit_table(enemy, empty_cell)) &&
                     (!ChessTable::get_hit_table(enemy, to)) &&
                     (!ChessTable::get_hit_table(enemy, from));
            return result;
        } else if ((from.index == 60) && (to.index == 58) && actor->cast_q) {
            coords empty_cell_1 = data_to_coords(59);
            coords empty_cell_2 = data_to_coords(58);
            result = (ChessTable::get_figure(59) == NULL) &&
                     (ChessTable::get_figure(58) == NULL) &&
                     (!ChessTable::get_hit_table(enemy, empty_cell_2)) &&
                     (!ChessTable::get_hit_table(enemy, empty_cell_1)) &&
                     (!ChessTable::get_hit_table(enemy, to)) &&
                     (!ChessTable::get_hit_table(enemy, from));
            return result;
        }
    }
    if (actor->color == WHITE) {
        if ((from.index == 4) && (to.index == 6) &&
            actor->cast_k) {  // рокировка
            coords empty_cell = data_to_coords(5);
            result = (ChessTable::get_figure(5) == NULL) &&
                     (!ChessTable::get_hit_table(enemy, empty_cell)) &&
                     (!ChessTable::get_hit_table(enemy, to)) &&
                     (!ChessTable::get_hit_table(enemy, from));
            return result;
        } else if ((from.index == 4) && (to.index == 2) && actor->cast_q) {
            coords empty_cell_1 = data_to_coords(3);
            coords empty_cell_2 = data_to_coords(2);
            result = (ChessTable::get_figure(3) == NULL) &&
                     (ChessTable::get_figure(2) == NULL) &&
                     (!ChessTable::get_hit_table(enemy, empty_cell_2)) &&
                     (!ChessTable::get_hit_table(enemy, empty_cell_1)) &&
                     (!ChessTable::get_hit_table(enemy, to)) &&
                     (!ChessTable::get_hit_table(enemy, from));
            return result;
        }
    }
    return result;
}

bool ChessTable::get_hit_table(figure_color color, coords to) {
    bool result = false;

    for (int i = 0; i < BOARD_SIZE; i++) {
        Figure *actor = ChessTable::table[i];
        if (actor != NULL) {
            if (actor->color == color) {
                coords from = data_to_coords(i);
                if (actor->name == "King") {
                    result =
                        (abs(from.x - to.x) <= 1) && (abs(from.y - to.y) <= 1);
                } else if (actor->name == "Queen") {
                    result = ChessTable::Queen_move(color, from, to);
                } else if (actor->name == "Bishop") {
                    result = ChessTable::Bishop_move(color, from, to);
                } else if (actor->name == "Knight") {
                    result = (((abs(from.x - to.x) == 2) &&
                               (abs(from.y - to.y) == 1)) ||
                              ((abs(from.x - to.x) == 1) &&
                               (abs(from.y - to.y) == 2)));
                } else if (actor->name == "Rock") {
                    result = ChessTable::Rock_move(color, from, to);
                } else if (actor->name == "Pawn") {
                    result =
                        ((to.y - from.y) == color) && (abs(to.x - from.x) == 1);
                }
                if (result) {
                    return result;
                }
            }
        }
    }
    return result;
}
void ChessTable::FEN_parser(const std::string FEN) {
    delete_table();
    Figure **ttable = new Figure *[BOARD_SIZE];  // temp_table
    table = ttable;
    for (int i = 0; i <= BOARD_SIZE; i++) {
        ttable[i] = NULL;
    }
    bool err_flag = false;
    const char *data = FEN.c_str();
    int cursor = 56;  // FEN нотация идёт от черных к белым, слева направо
    int data_cursor = 0;
    King *Black_king = NULL;
    King *White_king = NULL;

    while (data[data_cursor] != ' ') {  // считываем нотацию доски
        switch (data[data_cursor]) {
            case 'k':
                if (Black_king == NULL) {
                    Black_king = new King(BLACK);
                    ttable[cursor] = Black_king;
                    ChessTable::Black_king_ind = cursor;
                    cursor++;
                }
                break;
            case 'K':
                if (White_king == NULL) {
                    White_king = new King(WHITE);
                    ttable[cursor] = White_king;
                    ChessTable::White_king_ind = cursor;
                    cursor++;
                }

                break;

            case 'q':
                ttable[cursor] = new Queen(BLACK);
                cursor++;
                break;
            case 'Q':
                ttable[cursor] = new Queen(WHITE);
                cursor++;
                break;

            case 'r':
                ttable[cursor] = new Rock(BLACK);
                cursor++;
                break;
            case 'R':
                ttable[cursor] = new Rock(WHITE);
                cursor++;
                break;

            case 'b':
                ttable[cursor] = new Bishop(BLACK);
                cursor++;
                break;
            case 'B':
                ttable[cursor] = new Bishop(WHITE);
                cursor++;
                break;

            case 'n':
                ttable[cursor] = new Knight(BLACK);
                cursor++;
                break;
            case 'N':
                ttable[cursor] = new Knight(WHITE);
                cursor++;
                break;

            case 'p':
                ttable[cursor] = new Pawn(BLACK);
                cursor++;
                break;
            case 'P':
                ttable[cursor] = new Pawn(WHITE);
                cursor++;
                break;
            case '1' ... '8':
                cursor +=
                    data[data_cursor] - '0';  // преобразование из char  в int
                break;
            case '/':
                cursor -= 16;
                break;
            default:
                break;
        }
        data_cursor++;
    }

    data_cursor++;

    ChessTable::turn = (data[data_cursor] == 'w') ? WHITE : BLACK;

    data_cursor++;
    data_cursor++;

    while (data[data_cursor] != ' ') {  // считываем нотацию Рокировок
        switch (data[data_cursor]) {
            case 'k':
                Black_king->cast_k = 1;
                break;
            case 'K':
                White_king->cast_k = 1;
                break;

            case 'q':
                Black_king->cast_q = 1;
                break;
            case 'Q':
                White_king->cast_q = 1;
                break;
        }
        data_cursor++;
    }

    data_cursor++;

    if (data[data_cursor] != '-') {
        while (data[data_cursor] !=
               ' ') {  // считываем нотацию взятий на проходе
            ChessTable::set_enpassant(data[data_cursor], data[data_cursor + 1]);
            data_cursor += 2;
        }
    }
};

std::string ChessTable::get_FEN() const {
    std::string FEN = "";
    Figure **ttable = ChessTable::get_table();  // temp_table

    for (int i = 7; i >= 0; i--) {
        int null_counter = 0;
        for (int j = 0; j < 8; j++) {
            if (ttable[i * 8 + j] != NULL) {
                if (null_counter > 0) {
                    FEN += null_counter + '0';
                    null_counter = 0;
                }
                FEN += ttable[i * 8 + j]->FEN_name;
            } else {
                null_counter++;
            }
        };
        if (null_counter > 0) {
            FEN += null_counter + '0';
            null_counter = 0;
        }
        FEN += "/";
    };

    FEN += " ";

    King *White_King = static_cast<King *>(ttable[ChessTable::White_king_ind]);
    King *Black_King = static_cast<King *>(ttable[ChessTable::Black_king_ind]);
    FEN += (White_King->cast_k) ? "K" : "";
    FEN += (White_King->cast_q) ? "Q" : "";
    FEN += (Black_King->cast_k) ? "k" : "";
    FEN += (Black_King->cast_q) ? "q" : "";
    FEN += (!White_King->cast_k && !White_King->cast_q && !Black_King->cast_k &&
            !Black_King->cast_q)
               ? "-"
               : "";

    FEN += " ";
    bool nado = true;

    for (int i = 0; i < 8; i++) {
        if (ttable[i + 16] != NULL) {
            if (static_cast<Pawn *>(ttable[i + 16])->enPassant) {
                FEN += index_to_str(8 + i);
                nado = false;
            };
        }
        if (ttable[i + 40] != NULL) {
            if (static_cast<Pawn *>(ttable[i + 40])->enPassant) {
                FEN += index_to_str(48 + i);
                nado = false;
            }
        }
    }
    if (nado) {
        FEN += "-";
    }
    return FEN;
}

ChessTable::ChessTable() {
    ChessTable::table = new Figure *[64];
    for (int i = 0; i <= BOARD_SIZE; i++) {
        table[i] = NULL;
    }
}

void ChessTable::delete_table() {
    if (table != NULL) {
        // for(int i =0;i<=BOARD_SIZE;i++){
        //     if(table[i] != NULL){
        //         delete[] table[i];
        //     }
        // }
        delete[] ChessTable::table;
        table = NULL;
    }
    white_enpassant.clear();
    black_enpassant.clear();
}
ChessTable::~ChessTable() { delete_table(); }

Figure **ChessTable::get_table() const {
    Figure **temp = ChessTable::table;
    return temp;
}

bool ChessTable::check_en_passant(int from, int to) {
    Pawn *actor = static_cast<Pawn *>(ChessTable::table[from]);
    if (actor->color == WHITE) {
        if ((from / 8) == 4) {
            Figure *enemy = ChessTable::table[to - 8];
            if (enemy != NULL) {
                Pawn *enemy_pawn =
                    static_cast<Pawn *>(ChessTable::table[to - 8]);
                if (enemy_pawn != NULL) {
                    if (enemy_pawn->color == BLACK) {
                        return enemy_pawn->enPassant;
                    }
                }
            }
        }
    } else if (actor->color == BLACK) {
        if ((from / 8) == 3) {
            Figure *enemy = ChessTable::table[to + 8];
            if (enemy != NULL) {
                Pawn *enemy_pawn =
                    static_cast<Pawn *>(ChessTable::table[to + 8]);
                if (enemy_pawn != NULL) {
                    if (enemy_pawn->color == WHITE) {
                        return enemy_pawn->enPassant;
                    }
                }
            }
        }
    }
    return false;
}