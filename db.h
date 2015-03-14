#ifndef _DB_H
#define _DB_H

#define DATABASE_FILE "sudoku.sqlite3"
#define ENCRYPTION_SECRET "o78f9a#+.12A.9f*13-;2!"
#define _CRT_SECURE_NO_WARNINGS

int authenticate(char* nickname, char* password);
char register_user(char* nickname, char* name, char* lastname, char* password);
char insert_game_data(int user_id,
                      int seconds,
                      int helps,
                      int filled,
                      char finished,
                      char ranked);

#endif