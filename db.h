/* 
   ============================================================================
   Programmname: db.h
   Autor       : Tristan Lucas
                 Heinrich-Hertz-Berufskolleg
   Datum       : 
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Modularisierung von Datenbank-Funktionen als Schnittstelle zu
                 einer SQLite-Datebank
   ============================================================================ 
*/

#ifndef _DB_H
#define _DB_H

/* 
   ============================================================================
   Präprozessor-Anweisungen: globale defines
   ============================================================================ 
*/

#define DATABASE_FILE "sudoku.sqlite3"
#define ENCRYPTION_SECRET "o78f9a#+.12A.9f*13-;2!"
#define _CRT_SECURE_NO_WARNINGS

/* 
   ============================================================================
   Nach außen verfügbare Schnittstellen
   ============================================================================ 
*/

int authenticate(char* cpNickname, char* cpPassword);

char register_user(   char* cpNickname,
                      char* cpName,
                      char* cpLastname,
                      char* cpPassword);
char get_userdata(    int iUserId,
                      char** cppNickname,
                      char** cppName,
                      char** cppLastname,
                      char** cppDate);

char insert_game_data(int iUserId,
                      int iSeconds,
                      int iHelps,
                      int iFilled,
                      char cFinished,
                      char cRanked);

int get_user_games(int iUserId, int **ippGames);

#endif
