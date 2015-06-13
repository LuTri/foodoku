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
   Includedateien
   ============================================================================ 
*/
#include "sqlite3.h"

/* 
   ============================================================================
   Präprozessor-Anweisungen: globale defines
   ============================================================================ 
*/

#define DATABASE_FILE "sudoku.sqlite3"

/* 
   Das ENCRYPTION_SECRET wird als Schlüssel für eine einfache Vignère-
   Verschlüsselung verwendet
*/
#define ENCRYPTION_SECRET "o78f9a#+.12A.9f*13-;2!"
#define _CRT_SECURE_NO_WARNINGS
/*
   Der HELP_FACTOR bestimmt die Gewichtung von angezeigten Hilfen in der
   Bestenliste
*/
#define HELP_FACTOR 20
#define FILLED_FACTOR 50

#define QUERY_ERROR_NOSTAT "In der Datenbank ist ein Fehler aufgetreten.\n"

#define DB_USE 1
#define DB_CLOSE 0

/* 
   ============================================================================
   Typdefinitionen
   ============================================================================ 
*/

/* Userdata-Struktur */
typedef struct {
   char* sNickname;
   char* sName;
   char* sLastname;
   char* sPassword;
   char* sRegisterdate;
   int iUserId;
} USER;

/* Gamedata-Struktur */
typedef struct {
   int iUserId;
   int iMode;
   int iSeconds;
   int iHelps;
   int iFilled;
   char cFinished;
   char cRanked;
   char* sDate;
} GAME;

/* Ranked-Gamedata-Struktur */
typedef struct {
   char* sUserName;
   char* sGameDate;
   int iGameId;
   int iUserId;
   int iSeconds;
   int iMode;
   int iHelps;
   int iFilled;
   int iScore;
} GAMERANKING;

/* 
   ============================================================================
   Nach außen verfügbare Schnittstellen
   ============================================================================ 
*/

/*
   ============================================================================ 
   Funktionalitäten
   ============================================================================ 
*/

sqlite3* handle_db(char cUsage);
int   authenticate(char* sNickname, char* sPassword);

char  register_user(USER* uUser);
char  insert_game_data(GAME* upGameData);

USER* get_user_data(int iUserId);
int   get_best_user_games(int iMode, int iUserId, GAMERANKING* rpGames, int iLength);
int   get_best_games(int iMode, GAMERANKING* rpGames, int iLength);

/*
   ============================================================================ 
   Speicherverwaltung
   ============================================================================ 
*/
void delete_gameranking_data(GAMERANKING* rpGame, char cWholeStruct);
void delete_user_data(USER* upUser, char cWholeStruct);

#endif

/* EOF */
