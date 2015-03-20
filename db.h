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
#define HELP_FACTOR 30

#define QUERY_ERROR_NOSTAT "In der Datenbank ist ein Fehler aufgetreten.\n"
#define QUERY_ERROR "Ein Fehler ist aufgetreten: \"%s\".\n"

/* 
   ============================================================================
   Typdefinition "rankedgame"
   ============================================================================ 
*/

typedef struct {
   char* sUserName;
   char* sGameDate;
   int iGameId;
   int iUserId;
   int iSeconds;
   int iMode;
   int iHelps;
   int iScore;
} rankedgame;

/* 
   ============================================================================
   Nach außen verfügbare Schnittstellen
   ============================================================================ 
*/

int authenticate(char* sNickname, char* sPassword);

char register_user(   char* sNickname,
                      char* sName,
                      char* sLastname,
                      char* sPassword);
char get_userdata(    int iUserId,
                      char** spNickname,
                      char** spName,
                      char** spLastname,
                      char** spDate);

char insert_game_data(int iUserId,
                      int iMode,
                      int iSeconds,
                      int iHelps,
                      int iFilled,
                      char cFinished,
                      char cRanked);

/* ALLOZIERT FREI ZU GEBENDEN SPEICHER */
int get_user_games(int iUserId, int **ippGames);

char get_game_data(int iGameId,
                   int* ipUserId,
                   int* ipMode,
                   int* ipSeconds,
                   int* ipHelps,
                   int* ipFilled,
                   char* cpFinished,
                   char* cpRanked,
                   char** spDate);

/* ALLOZIERT FREI ZU GEBENDEN SPEICHER */
int get_best_games(int iMode, rankedgame* ugGames, int iLength);
void delete_game_data_texts(rankedgame* ugGame);


#endif

/* EOF */