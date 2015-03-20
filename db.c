/* 
   ============================================================================
   Programmname: db.c
   Autor       : Tristan Lucas
                 Heinrich-Hertz-Berufskolleg
   Datum       : 
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Modularisierung von Datenbank-Funktionen als Schnittstelle zu
                 einer SQLite-Datebank

   Programmschnittstelle: Siehe zugehörige Header-Datei  
   ============================================================================ 
*/

/*
   ============================================================================
   Präprozessor-Anweisungen: Genutzte SQL-Queries
   ============================================================================ 
*/

/* Query zur Benutzer / Passwort Validierung */
#define QUERY_AUTHENTICATE ""\
   "SELECT user_id "\
   "FROM userdata "\
   "WHERE spielername='%s' AND passwort='%s';"

/* Query zum Eintragen neuer Benutzer*/
#define QUERY_INSERT_USER ""\
   "INSERT INTO userdata"\
      "(passwort,spielername,vorname,nachname,anmeldedatum) "\
   "VALUES"\
      "('%s','%s','%s','%s',date('now'));"

/* Query zum Erhalten der Benutzerdaten zu einer ID */
#define QUERY_GET_USERDATA ""\
   "SELECT spielername, vorname, nachname, anmeldedatum "\
   "FROM userdata WHERE user_id = %d;"

/* Query zum Eintragen eines Spiels */
#define QUERY_INSERT_GAMEDATA ""\
   "INSERT INTO games"\
      "(user, spielzeitpunkt, zeit, angezeigte_hilfen, gefuellte_felder, abges"\
      "chlossen, gewertet, schwierigkeit) "\
   "VALUES"\
      "(%d,datetime('now','localtime'),%d,%d,%d,%d,%d,%d);"

/* Query zum Erhalten aller Spiele-Ids von einem Benutzer */
#define QUERY_GET_USER_GAMES ""\
   "SELECT game_id FROM games WHERE user = %d;"

/* Query zum Erhalten aller Spieldaten zu einer ID */
#define QUERY_GET_GAMEDATA ""\
   "SELECT user, spielzeitpunkt, zeit, angezeigte_hilfen, gefuellte_felder, ab"\
      "geschlossen, gewertet, schwierigkeit "\
   "FROM games WHERE game_id = %d;"

/*
   Query zum Erhalten aller Top Spiele zu einem Schwierigkeitsgrad, mit einem
   Limit.
   Die Spiele werden aufsteigend sortiert - dabei wird die tatsächliche Wertung
   mit der Formel
      score = zeit + Hilfen * HELP_FACTOR
   bestimmt.
   Ein niedrigerer Score ist besser.
*/
#define QUERY_GET_TOPRANKED ""\
      "SELECT "\
         "game.user, game.game_id, game.zeit, game.schwierigkeit, game.angezei"\
         "gte_hilfen, game.spielzeitpunkt, user.spielername, game.zeit + game."\
         "angezeigte_hilfen * %d AS 'score' "\
      "FROM games game INNER JOIN userdata user "\
      "WHERE user.user_id = game.user AND game.schwierigkeit = %d "\
      "ORDER BY score ASC LIMIT %d;"

/* 
   ============================================================================
   Includedateien
   ============================================================================ 
*/

#include "db.h"
#include "sqlite3.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int append_to_intarray(int **ippArray, int iValue, int iLength)
/*
   ============================================================================
   Fügt einen Wert an das Array an, das hinter dem übergebenen Zeiger liegt.
      1. Parameter: Der Zeiger auf den Array-Zeiger
      2. Parameter: Der Wert, der eingefügt werden soll
      3. Parameter: Die aktuelle Länge des Arrays
      4. Rückgabewert: Die neue Länge der Arrays
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   int i;
   /*
      Das neue Array wird genau eins größer sein als das alte, entsprechend
      Speicher allozieren
   */
   int* ipTemp = (int*)malloc(sizeof(int) * ++iLength);

   for (i = 0; i < iLength - 1; i++) {
      /* Bereits vorhandene Werte kopieren */
      ipTemp[i] = (*ippArray)[i];
   }

   ipTemp[iLength - 1] = iValue;

   /*
      Das alte Array freigeben und den übergebenen Zeiger auf das neue zeigen
      lassen
   */
   if (iLength != 1)
      free(*ippArray);
   *ippArray = ipTemp;
   
   /* Die neue Länge zurückgeben */
   return iLength;
}

char* encrypt(char* sClear)
/*
   ============================================================================
   Verschlüsselt einen String mit einer einfachen Vigenère-Verschlüsselung. Der
   Schlüssel ist der Head-Datei zu entnehmen
      1. Parameter: Der zu verschlüsselnde Text
      2. Rückgabewert: Der verschlüsselte Text in hexadezimaler Darstellung
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   char cBuf[3] = {0,0,0};
   int iClearLen;
   int iSecretLen;
   int iEncIdx = 0;
   int iStrIdx;
   char* sEncrypted;

   iSecretLen = strlen(ENCRYPTION_SECRET);
   iClearLen = strlen(sClear);

   /* 
      Der verschlüsselte Text wird immer genau doppelt so lang sein wie der
      Klartext, da jedes Zeichen durch 2 Stellen im hexadezimalen System dar-
      gestellt werden kann. Ein Zeichen wird zusätzlich für das \0 alloziert.
   */
   sEncrypted = (char*)calloc(iClearLen * 2 + 1,sizeof(char));
   
   for (iStrIdx = 0; iStrIdx < iClearLen; iStrIdx++)
   {
      char cTemp;
      /*
         Auf jedes Zeichen im Klartext wird das (forlaufende) Zeichen des
         Schlüssels addiert und das Resultat anschließend auf den Wertebereich
         für 8-Bit begrenzt
      */
      cTemp = ((sClear[iStrIdx] + ENCRYPTION_SECRET[iEncIdx++]) % 255);

      if (iEncIdx >= iSecretLen)
      {
         /* 
            Falls alle verfügbaren Zeichen aus dem Schlüssel benutzt wurden,
            fang von vorne an
         */
         iEncIdx = 0;
      }

      /* 
         Das verschlüsselte Zeichen wird als Hexadezimal-Wert an das Ergebnis
         angefügt
      */
      sprintf(cBuf, "%x", (unsigned char)cTemp);
      
      strcat(sEncrypted, cBuf);
   }

   /* Der verschlüsselte String wird zurückgegeben */
   return sEncrypted;
}

sqlite3* get_db(void)
/*
   ============================================================================
   Öffnet eine Datenbank und speichert diese zur Wiederverwendung als statische
   Variable zwischen
      1. Rückgabewert: Der Zeiger auf die SQLite-Datenbank
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   static sqlite3 *dbpDatabase = NULL;
   int iReturnCode;

   if (dbpDatabase == NULL)
   {
      /* 
         Falls diese Programminstanz noch keine Datenbank geöffnet hat, öffne
         eine neue
      */
      iReturnCode = sqlite3_open(DATABASE_FILE, &dbpDatabase);
      if (iReturnCode != SQLITE_OK)
      {
         /* Wenn beim öffnen ein Fehler auftrat, setze den Zeiger auf NULL */
         printf(QUERY_ERROR_NOSTAT);
         sqlite3_close(dbpDatabase);
         dbpDatabase = NULL;
      }
   }

   /* Der gültige Datenbankzeiger oder NULL wird zurückgegeben */
   return dbpDatabase;
}

int authenticate(char* sNickname, char* sPassword)
/*
   ============================================================================
   Überprüft die Kombination aus Spielername / Passwort und gibt die User-Id
   des authentifizierten Benutzers zurück
      1. Parameter: Der eingegebene Spielername
      2. Parameter: Das eingegebene Passwort
      3. Rückgabewert: Die User-Id des authentifizierten Benutzers
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   int iReturnCode;
   int iUserId = 0;
   char* sSql;
   sqlite3_stmt *stmt;
   char* sEncrypted = encrypt(sPassword);

   /* Query mit Parametern vorbereiten */
   sSql = sqlite3_mprintf(QUERY_AUTHENTICATE, sNickname, sEncrypted);

   /* Query auf der Datenbank ausführen und anschließend den String freigeben */
   iReturnCode = sqlite3_prepare_v2(get_db(),
                                    sSql,
                                    strlen(sSql),
                                    &stmt,
                                    NULL);
   free(sSql);

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-Rückgabe beenden
      */
      printf(QUERY_ERROR_NOSTAT);
      return 0;
   }

   if (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      /* 
         Wenn ein Ergebnis vorhanden ist (Es kann aufgrund des
         Unique-Constraints entweder nur ein oder kein Ergebnis entstehen)
         ist die User-Id der einzige Wert im Ergebnis.
      */
      iUserId = atoi((const char*)sqlite3_column_text(stmt, 0));
   }

   /* Erhaltene User-Id oder 0 zurückgeben */
   return iUserId;
}

char register_user(char* sNickname,
                   char* sName,
                   char* sLastname,
                   char* sPassword)
/*
   ============================================================================
   Registriert einen neuen Benutzer in der Datenbank, gibt den Erfolgsstatus
   ( 0=Fehler, 1=Erfolg ) zurück
      1. Parameter: Der gewünschte Spielername
      2. Parameter: Der Vorname des Spielers
      3. Parameter: Der Nachname des Spielers
      4. Parameter: Das gewünschte Passwort
      5. Rückgabewert: Der Status (0 = Fehler, 1 = Erfolg)
   ============================================================================
*/
{
   /* Variablen-Definition */
   char* sSql;
   int iReturnCode;
   char* sEncrypted;
   char* sErrMsg;

   /* 
      Das Passwort sollte nicht im Klartext gespeichert werden, es wird
      verschlüsselt. Wie: Siehe Kommentare zu encrypt().
   */
   sEncrypted = encrypt(sPassword);
   

   /* Query mit Parametern vorbereiten */
   sSql = sqlite3_mprintf(QUERY_INSERT_USER,
      sEncrypted, sNickname, sName, sLastname);

   /* Den verschlüsselten String wieder löschen*/
   free(sEncrypted);

   /* Query auf der Datenbank ausführen und anschließend den String freigeben */
   iReturnCode = sqlite3_exec(get_db(),
                              sSql,
                              NULL,
                              NULL,
                              &sErrMsg);
   free(sSql);

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-Rückgabe beenden
      */
      printf(QUERY_ERROR, sErrMsg);
      free(sErrMsg);
      return 0;
   }
   return 1;
}

char get_userdata(int iUserId,
                  char** spNickname,
                  char** spName,
                  char** spLastname,
                  char** spDate)
/*
   ============================================================================
   Holt die Nutzerdaten aus der Datenbank und schreibt diese an die übergebenen
   Adressen
      1. Parameter: Die User-Id
      2. Parameter: Die Adresse, an die der Spielername geschrieben werden soll
      3. Parameter: Die Adresse, an die der Vorname geschrieben werden soll
      4. Parameter: Die Adresse, an die der Nachname geschrieben werden soll
      5. Parameter: Die Adresse, an die das Anmeldedatum geschrieben werden
                    soll
      6. Rückgabewert: Der Status (0 = Fehler, 1 = Erfolg)
   ============================================================================
*/
{
   /* Variablen-Definition */
   int iReturnCode;
   char* sSql;
   sqlite3_stmt *stmt;
   
   /* Query mit Parametern vorbereiten */
   sSql = sqlite3_mprintf(QUERY_GET_USERDATA, iUserId); 


   /* Query auf Datenbank ausführen, Query löschen */
   iReturnCode = sqlite3_prepare_v2(get_db(),
                                    sSql,
                                    strlen(sSql),
                                    &stmt,
                                    NULL);
   free(sSql);

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-Rückgabe beenden
      */
      printf(QUERY_ERROR_NOSTAT);
      return 0;
   }

   if (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      /* 
         Wenn eine Zeile zur gewünschten User-Id gefunden wurde, lies die Werte
         aus den Spalten
      */
      *spNickname = (char*)sqlite3_column_text(stmt, 0);
      *spName = (char*)sqlite3_column_text(stmt, 1);
      *spLastname= (char*)sqlite3_column_text(stmt, 2);
      *spDate = (char*)sqlite3_column_text(stmt, 3);
      /* Wir kennen die Abfolge der Spalten, da wir sie selbst angeben */

      /* 1 ( = Erfolg ) zurückgeben*/
      return 1;
   }
   else
   {
      /* Falls keine Zeile gefunden wurde gib 0 ( = Fehler ) zurück */
      return 0;
   }
}

char insert_game_data(int iUserId,
                      int iMode,
                      int iSeconds,
                      int iHelps,
                      int iFilled,
                      char cFinished,
                      char cRanked)
/*
   ============================================================================
   Speichert Spiel-Daten in der Datenbank, abhängig von der übegebenen User-Id,
   gibt den Erfolgsstatus( 0=Fehler, 1=Erfolg ) zurück
      1. Parameter: Die User-Id des Spielers
      2. Parameter: Die Zeit die das Spiel in Sekunden dauerte
      3. Parameter: Anzahl der gezeigten Hilfestellungen
      4. Parameter: Anzahl der gefüllten Felder
      5. Parameter: Angabe, ob das Spiel gewonnen wurde (1 Ja, 0 Nein)
      6. Parameter: Angabe, ob das Spiel gewertet wurde (1 Ja, 0 Nein)
      7. Rückgabewert: Der Status (0 = Fehler, 1 = Erfolg)
   ============================================================================
*/
{
   /* Variablen-Definition */
   char* sSql;
   int iReturnCode;
   char* sErrMsg;

   /* Verbereitung der Query mit Parametern */
   sSql = sqlite3_mprintf(QUERY_INSERT_GAMEDATA,
      iUserId, iSeconds, iHelps, iFilled, cFinished, cRanked, iMode);

   /* Query auf der Datenbank ausführen und anschließend den String freigeben */
   iReturnCode = sqlite3_exec(get_db(),
                              sSql,
                              NULL,
                              NULL,
                              &sErrMsg);
   free(sSql);
   
   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-Rückgabe beenden
      */
      printf(QUERY_ERROR, sErrMsg);
      free(sErrMsg);
      return 0;
   }
   return 1;
}

int get_user_games(int iUserId, int **ippGames)
/*
   ============================================================================
   Holt die Spiel-Ids des übergebenen Benutzers aus der Datenbank und schreibt
   die Adresse der Ergebnisse in den übergebenen Zeiger
      1. Parameter: Die User-Id
      2. Parameter: Der Zeiger, der auf die Ergebnisse zeigen soll
      3. Rückgabewert: Die Ergebnis-Länge
   ============================================================================
*/
{
   /* Variablen-Definition */
   int iReturnCode;
   char* sSql;
   int iLastLength = 0;
   sqlite3_stmt *stmt;
   
   /* Query mit Parameter vorbereiten */
   sSql = sqlite3_mprintf(QUERY_GET_USER_GAMES, iUserId); 


   /* Query auf Datenbank ausführen, Query löschen */
   iReturnCode = sqlite3_prepare_v2(get_db(),
                                    sSql,
                                    strlen(sSql),
                                    &stmt,
                                    NULL);
   free(sSql);

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-Rückgabe beenden
      */
      printf(QUERY_ERROR_NOSTAT);
      return 0;
   }

   while (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      /* Schreibe die gefundenen Spiel-Ids an die übergebene Adresse */
      iLastLength = append_to_intarray(ippGames,
                                       atoi((char*)sqlite3_column_text(stmt, 0)),
                                       iLastLength);
   }

   /* Gib die Anzahl der gefundenen Spiele zurück */
   return iLastLength;
}

char get_game_data(int iGameId,
                   int* ipUserId,
                   int* ipMode,
                   int* ipSeconds,
                   int* ipHelps,
                   int* ipFilled,
                   char* cpFinished,
                   char* cpRanked,
                   char** spDate)
/*
   ============================================================================
   Holt die Spiel-Daten zu einer Spiel-Id aus der Datenbank
      1. Parameter: Die User-Id
      2. Parameter: Die Adresse, an die der Spielername geschrieben werden soll
      3. Parameter: Die Adresse, an die der Vorname geschrieben werden soll
      4. Parameter: Die Adresse, an die der Nachname geschrieben werden soll
      5. Parameter: Die Adresse, an die das Anmeldedatum geschrieben werden
                    soll
      6. Rückgabewert: Der Status (0 = Fehler, 1 = Erfolg)
   ============================================================================
*/
{
   /* Variablen-Definition */
   int iReturnCode;
   char* sSql;
   sqlite3_stmt *stmt;
   
   /* Query mit Parametern vorbereiten */
   sSql = sqlite3_mprintf(QUERY_GET_GAMEDATA, iGameId); 


   /* Query auf Datenbank ausführen, Query löschen */
   iReturnCode = sqlite3_prepare_v2(get_db(),
                                    sSql,
                                    strlen(sSql),
                                    &stmt,
                                    NULL);
   free(sSql);

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-Rückgabe beenden
      */
      printf(QUERY_ERROR_NOSTAT);
      return 0;
   }

   if (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      /* 
         Wenn eine Zeile zur gewünschten User-Id gefunden wurde, lies die Werte
         aus den Spalten
      */
      /* Wir kennen die Abfolge der Spalten, da wir sie selbst angeben */
      *ipUserId = atoi((char*)sqlite3_column_text(stmt, 0));
      *spDate = (char*)sqlite3_column_text(stmt, 1);
      *ipSeconds = atoi((char*)sqlite3_column_text(stmt, 2));
      *ipHelps = atoi((char*)sqlite3_column_text(stmt, 3));
      *ipFilled = atoi((char*)sqlite3_column_text(stmt, 4));
      *cpFinished = atoi((char*)sqlite3_column_text(stmt, 5));
      *cpRanked = atoi((char*)sqlite3_column_text(stmt, 6));
      *ipMode = atoi((char*)sqlite3_column_text(stmt, 7));

      /* 1 ( = Erfolg ) zurückgeben*/
      return 1;
   }
   else
   {
      /* Falls keine Zeile gefunden wurde gib 0 ( = Fehler ) zurück */
      return 0;
   }
}

int get_best_games(int iMode, rankedgame* ugGames, int iLength)
/*
   ============================================================================
   Holt die besten Spiele zu einem Schwierigkeitsgrad, samt relevanter Daten
   aus der Datenbank.
      1. Parameter: Der Schwierigkeitsgrad
      2. Parameter: Das Array aus Spieldaten
      3. Parameter: Die Länge des Arrays
      4. Rückgabewert: Die Anzahl der Ergebnisse
   ============================================================================
*/
{
   /* Variablen-Definition */
   int iResultLength = 0;
   int iReturnCode;
   char* sSql;
   rankedgame* ugActGame = ugGames;
   sqlite3_stmt *stmt;

   
   /* Query mit Parametern vorbereiten */
   sSql = sqlite3_mprintf(QUERY_GET_TOPRANKED,HELP_FACTOR, iMode, iLength);


   /* Query auf Datenbank ausführen, Query löschen */
   iReturnCode = sqlite3_prepare_v2(get_db(),
                                    sSql,
                                    strlen(sSql),
                                    &stmt,
                                    NULL);
   free(sSql);

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-Rückgabe beenden
      */
      printf(QUERY_ERROR_NOSTAT);
      return 0;
   }

   while (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      /*
         sText dient als Zwischenspeicher für einen Zeiger auf Text aus der
         Datenbank
      */
      char* sText;

      /* Wir kennen die Abfolge der Spalten, da wir sie selbst angeben */
      ugGames[iResultLength].iGameId = atoi((char*)sqlite3_column_text(stmt, 0));
      ugGames[iResultLength].iUserId = atoi((char*)sqlite3_column_text(stmt, 1));
      ugGames[iResultLength].iSeconds = atoi((char*)sqlite3_column_text(stmt, 2));
      ugGames[iResultLength].iMode = atoi((char*)sqlite3_column_text(stmt, 3));
      ugGames[iResultLength].iHelps = atoi((char*)sqlite3_column_text(stmt, 4));

      /*
         Der aus der Datenbank erhaltene Text muss kopiert werden.
         Dazu muss entsprechender Speicher alloziert werden.
      */
      sText = (char*)sqlite3_column_text(stmt, 5);
      ugGames[iResultLength].sGameDate = (char*)malloc(strlen(sText) + 1);
      strcpy(ugGames[iResultLength].sGameDate, sText);

      /* Der aus der Datenbank erhaltene Text muss kopiert werden */
      sText = (char*)sqlite3_column_text(stmt, 6);
      ugGames[iResultLength].sUserName = (char*)malloc(strlen(sText) + 1);
      strcpy(ugGames[iResultLength].sUserName, sText);

      ugGames[iResultLength].iScore = atoi((char*)sqlite3_column_text(stmt, 7));
      iResultLength++;
   }

   /* Die erhaltene Anzahl an Spielen wird zurückgegeben */
   return iResultLength;
}

void delete_game_data_texts(rankedgame* ugGame)
/*
   ============================================================================
   Löscht die aus der Datenbank kopierten String in der rankedgame-Struktur
      1. Parameter: das Usergame
   ============================================================================
*/
{
   /* Datums-text löschen */
   free(ugGame->sGameDate);
   /* Spielernamen löschen */
   free(ugGame->sUserName);
}

/* EOF */