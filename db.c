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

   Programmschnittstelle: Siehe zugeh�rige Header-Datei  
   ============================================================================ 
*/

/*
   ============================================================================
   Pr�prozessor-Anweisungen: Genutzte SQL-Queries
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

/* Query zum Auflisten aller registrierten User-IDs */
#define QUERY_GET_USERS ""\
   "SELECT user_id FROM userdata;"

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
   Die Spiele werden aufsteigend sortiert - dabei wird die tats�chliche Wertung
   mit der Formel
      score = zeit + Hilfen * HELP_FACTOR + gef�llte Felder * FILLED_FACTOR
   bestimmt.
   Ein niedrigerer Score ist besser.
*/
#define QUERY_GET_TOPRANKED ""\
      "SELECT "\
         "game.user, game.game_id, game.zeit, game.schwierigkeit, game.angezei"\
         "gte_hilfen, game.gefuellte_felder, game.spielzeitpunkt, user.spiele"\
         "rname, game.zeit + game.angezeigte_hilfen * %d + game.gefuellte_feld"\
         "er * %d AS 'score' "\
      "FROM games game INNER JOIN userdata user "\
      "WHERE user.user_id = game.user AND game.schwierigkeit = %d "\
         "AND game.gewertet = 1 AND game.abgeschlossen = 1 "\
      "ORDER BY score ASC LIMIT %d;"

/*
   Query zum Erhalten aller der besten Spiele eines Users, analog zu
   QUERY_GET_TOPRANKED
*/
#define QUERY_GET_USER_TOP ""\
      "SELECT "\
         "game.user, game.game_id, game.zeit, game.schwierigkeit, game.angezei"\
         "gte_hilfen, game.gefuellte_felder, game.spielzeitpunkt, user.spiele"\
         "rname, game.zeit + game.angezeigte_hilfen * %d + game.gefuellte_feld"\
         "er * %d AS 'score' "\
      "FROM games game INNER JOIN userdata user "\
      "WHERE user.user_id = game.user AND game.schwierigkeit = %d "\
         "AND game.abgeschlossen = 1 "\
         "AND user.user_id = %d "\
      "ORDER BY score ASC LIMIT %d;"


/* 
   ============================================================================
   Includedateien
   ============================================================================ 
*/

#include "db.h"
#include "os.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int append_to_intarray(int **ippArray, int iValue, int iLength)
/*
   ============================================================================
   F�gt einen Wert an das Array an, das hinter dem �bergebenen Zeiger liegt.
      1. Parameter: Der Zeiger auf den Array-Zeiger
      2. Parameter: Der Wert, der eingef�gt werden soll
      3. Parameter: Die aktuelle L�nge des Arrays
      4. R�ckgabewert: Die neue L�nge der Arrays
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   int i;
   /*
      Das neue Array wird genau eins gr��er sein als das alte, entsprechend
      Speicher allozieren
   */
   int* ipTemp = (int*)malloc(sizeof(int) * ++iLength);

   for (i = 0; i < iLength - 1; i++) {
      /* Bereits vorhandene Werte kopieren */
      ipTemp[i] = (*ippArray)[i];
   }

   ipTemp[iLength - 1] = iValue;

   /*
      Das alte Array freigeben und den �bergebenen Zeiger auf das neue zeigen
      lassen
   */
   if (iLength != 1)
      free(*ippArray);
   *ippArray = ipTemp;
   
   /* Die neue L�nge zur�ckgeben */
   return iLength;
}

char* encrypt(char* sClear)
/*
   ============================================================================
   Verschl�sselt einen String mit einer einfachen Vigen�re-Verschl�sselung. Der
   Schl�ssel ist der Header-Datei zu entnehmen
      1. Parameter: Der zu verschl�sselnde Text
      2. R�ckgabewert: Der verschl�sselte Text in hexadezimaler Darstellung
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
      Der verschl�sselte Text wird immer genau doppelt so lang sein wie der
      Klartext, da jedes Zeichen durch 2 Stellen im hexadezimalen System dar-
      gestellt werden kann. Ein Zeichen wird zus�tzlich f�r das \0 alloziert.
   */
   sEncrypted = (char*)calloc(iClearLen * 2 + 1,sizeof(char));
   
   for (iStrIdx = 0; iStrIdx < iClearLen; iStrIdx++)
   {
      char cTemp;
      /*
         Auf jedes Zeichen im Klartext wird das (forlaufende) Zeichen des
         Schl�ssels addiert und das Resultat anschlie�end auf den Wertebereich
         f�r 8-Bit begrenzt
      */
      cTemp = ((sClear[iStrIdx] + ENCRYPTION_SECRET[iEncIdx++]) % 255);

      if (iEncIdx >= iSecretLen)
      {
         /* 
            Falls alle verf�gbaren Zeichen aus dem Schl�ssel benutzt wurden,
            fang von vorne an
         */
         iEncIdx = 0;
      }

      /* 
         Das verschl�sselte Zeichen wird als Hexadezimal-Wert an das Ergebnis
         angef�gt
      */
      sprintf(cBuf, "%x", (unsigned char)cTemp);
      
      strcat(sEncrypted, cBuf);
   }

   /* Der verschl�sselte String wird zur�ckgegeben */
   return sEncrypted;
}

char init_db(sqlite3* dbpDatabase)
/*
   ============================================================================
   Liest SQL zum Erstellen der notwendigen Tabellen aus einer Datei aus und
   f�hrt dieses dann auf der Datenbank aus.
      1. Parameter: Die ge�ffnete Datenbank
      2. R�ckgabewert: Ob die Datenbank erfolgreich initialisert
         wurde
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   char* sSql = NULL;
   int iReturnCode;
   int iFileLength;
   int iReadLength;
   char cFileRead = 0;

   FILE *fSqlData = fopen("sql/tables.sql", "r");
   if (fSqlData != NULL) {
      /* Zum Ende der Datei springen */
      if (fseek(fSqlData, 0L, SEEK_END) == 0) {
         /* Die Gr��e der Datei ( = L�nge des SQL ) auslesen */
         iFileLength = ftell(fSqlData);
         if (iFileLength != -1) {
            /* Speicherplatz f�r das gesamte SQL anlegen */
            sSql = (char*)malloc(sizeof(char) * (iFileLength + 1));

            /* Zur�ck zum Anfang der Datei springen */
            if (fseek(fSqlData, 0L, SEEK_SET) == 0) {

               /* Das gesamte SQL in den Speicher (sSql) lesen */
               iReadLength = fread(sSql, sizeof(char), iFileLength, fSqlData);

               if (iReadLength != 0) {
                  /* An dieser Stelle wurde die Datei erfolgreich gelesen */
                  cFileRead = 1;
                  /* Der String muss noch terminiert werden */
                  sSql[iReadLength] = '\0';
               }
            }
         }
      }
      /* SQL-Datei wieder schlie�en */
      fclose(fSqlData);
   }

   if (cFileRead) {
      /* Nur wenn das SQL erfolreicht gelesen wurde darf es ausgef�hrt werden */
      iReturnCode = sqlite3_exec(dbpDatabase, sSql, 0, 0, 0);
      free(sSql);

      if (iReturnCode != SQLITE_OK) {
         printf(sqlite3_errmsg(dbpDatabase));
      }

      /*
         Wenn der sqlite3_exec erfolgreich ausgef�hrt wurde, wurde auch
         die Datenbank erfolgreich mit Tabellen gef�llt
      */
      return iReturnCode == SQLITE_OK;
   }
   /*
      Wenn die Ausf�hrung bis zu diesem Punkt kommt wurden keine Tabellen
      angelegt
   */
   return 0;
}

char check_db(sqlite3 *dbpDatabase)
/*
   ============================================================================
   Pr�ft, ob die Datenbank g�ltige Tabellen hat oder tr�gt diese in die
   Datebank ein. Das SQL zum erstellen der Tabellen wird dabei aus einer Datei
   gelesen
      1. Parameter: Die ge�ffnete Datenbank
      2. R�ckgabewert: Status, ob die Datenbank initialisiert ist
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   const char* sSql = "SELECT * FROM sqlite_master WHERE type='table'";
   int iReturnCode;
   int iNumTables = 0;
   sqlite3_stmt *stmt;


   /* Query auf der Datenbank ausf�hren */
   iReturnCode = sqlite3_prepare_v2(dbpDatabase, sSql, -1, &stmt, NULL);
   if (iReturnCode == SQLITE_OK) {
      while (sqlite3_step( stmt ) == SQLITE_ROW)
      {
         /* Merken der Anzahl der Tabellen in der Datenbank */
         iNumTables++;
      }

      /* 
         Wenn keine Tabelle in der Datenbank vorhanden ist m�ssen die n�tigen
         Eintr�ge angelegt werden
      */
      if (iNumTables == 0) {
         return init_db(dbpDatabase);
      } else {
         /* Ansonsten ist die Datenbank initialisiert */
         return 1;
      }
   }

   /* Wenn die Ausf�hrung hier ankommt ist ein Fehler aufgetreten */
   return 0;
}

sqlite3* handle_db(char cUsage)
/*
   ============================================================================
   �ffnet eine Datenbank und speichert diese zur Wiederverwendung als statische
   Variable zwischen - oder schlie�t die ge�ffnete Datenbank
      1. Parameter: Der Parameter ob die Datenbank genutzt oder geschlossen
                    werden soll
      1. R�ckgabewert: Der Zeiger auf die SQLite-Datenbank
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   static sqlite3 *dbpDatabase = NULL;
   int iReturnCode;

   if (dbpDatabase == NULL && cUsage == DB_USE)
   {
      /* 
         Falls diese Programminstanz noch keine Datenbank ge�ffnet hat, �ffne
         eine neue
      */
      iReturnCode = sqlite3_open(DATABASE_FILE, &dbpDatabase);
      if (iReturnCode != SQLITE_OK)
      {
         /* Wenn beim �ffnen ein Fehler auftrat, setze den Zeiger auf NULL */
         printf(QUERY_ERROR_NOSTAT);
         sqlite3_close(dbpDatabase);
         dbpDatabase = NULL;
      }
   }
   else if (cUsage == DB_CLOSE)
   {
      sqlite3_close(dbpDatabase);
      dbpDatabase = NULL;
   }

   if (!check_db(dbpDatabase)) {
      /*
         Wenn die Datenbank nicht initialisiert ist oder wurde wird ein Fehler
         ausgegeben
      */
      printf("EIN KRITISCHER FEHLER IST AUFGETRETEN!\n");
      sqlite3_close(dbpDatabase);
      dbpDatabase = NULL;
   }

   /* Der g�ltige Datenbankzeiger oder NULL wird zur�ckgegeben */
   return dbpDatabase;
}
 
int authenticate(char* sNickname, char* sPassword)
/*
   ============================================================================
   �berpr�ft die Kombination aus Spielername / Passwort und gibt die User-Id
   des authentifizierten Benutzers zur�ck
      1. Parameter: Der eingegebene Spielername
      2. Parameter: Das eingegebene Passwort
      3. R�ckgabewert: Die User-Id des authentifizierten Benutzers
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

   /* Query auf der Datenbank ausf�hren und anschlie�end den String freigeben */
   iReturnCode = sqlite3_prepare_v2(handle_db(DB_USE),
                                    sSql,
                                    strlen(sSql),
                                    &stmt,
                                    NULL);
   FREE_SQL(sSql)

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
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

   /* Erhaltene User-Id oder 0 zur�ckgeben */
   return iUserId;
}

char register_user(USER* uUser)
/*
   ============================================================================
   Registriert einen neuen Benutzer in der Datenbank, gibt den Erfolgsstatus
   ( 0=Fehler, 1=Erfolg ) zur�ck
      1. Parameter: Der gew�nschte Spielername
      2. Parameter: Der Vorname des Spielers
      3. Parameter: Der Nachname des Spielers
      4. Parameter: Das gew�nschte Passwort
      5. R�ckgabewert: Der Status (0 = Fehler, 1 = Erfolg)
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
      verschl�sselt. Wie: Siehe Kommentare zu encrypt().
   */
   sEncrypted = encrypt(uUser->sPassword);
   

   /* Query mit Parametern vorbereiten */
   sSql = sqlite3_mprintf(QUERY_INSERT_USER,
      sEncrypted, uUser->sNickname, uUser->sName, uUser->sLastname);

   /* Den verschl�sselten String wieder l�schen*/
   free(sEncrypted);

   /* Query auf der Datenbank ausf�hren und anschlie�end den String freigeben */
   iReturnCode = sqlite3_exec(handle_db(DB_USE),
                              sSql,
                              NULL,
                              NULL,
                              &sErrMsg);
   FREE_SQL(sSql)

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf("Der Benutzername ist bereits vorhanden, " 
		     "bitte anderen Namen w\204hlen\n");
      free(sErrMsg);
      return 0;
   }
   return 1;
}

int get_users(int** ippUserIds)
/*
   ============================================================================
   Holt die IDs aller Nutzer aus der Datenbank und schreibt diese in ein Array.
   Die Adresse des erhaltenen Arrays wird an die �bergebene Adresse geschrieben.
      1. Parameter: Die Adresse auf die geschrieben werden soll
      2. R�ckgabewert: Die Anzahl der erhaltenen IDs
   ============================================================================
*/
{   /* Variablen-Definition */
   int iReturnCode;
   int iLastLength = 0;
   sqlite3_stmt *stmt;
   
   /* Query auf Datenbank ausf�hren, Query l�schen */
   iReturnCode = sqlite3_prepare_v2(handle_db(DB_USE),
                                    QUERY_GET_USERS,
                                    strlen(QUERY_GET_USERS),
                                    &stmt,
                                    NULL);

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf(QUERY_ERROR_NOSTAT);
      return 0;
   }

   while (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      /* Schreibe die gefundenen Spiel-Ids an die �bergebene Adresse */
      iLastLength = append_to_intarray(ippUserIds,
                                       atoi((char*)sqlite3_column_text(stmt, 0)),
                                       iLastLength);
   }

   /* Gib die Anzahl der gefundenen Spiele zur�ck */
   return iLastLength;

}

USER* get_user_data(int iUserId)
/*
   ============================================================================
   Holt die Nutzerdaten aus der Datenbank und schreibt diese an die �bergebenen
   Adressen
      1. Parameter: Die User-Id
      2. R�ckgabewert: Der Zeiger auf die Nutzerdaten-Struktur
   ============================================================================
*/
{
   /* Variablen-Definition */
   int iReturnCode;
   char* sSql;
   sqlite3_stmt *stmt;

   USER* upUserData;
   
   /* Query mit Parametern vorbereiten */
   sSql = sqlite3_mprintf(QUERY_GET_USERDATA, iUserId); 


   /* Query auf Datenbank ausf�hren, Query l�schen */
   iReturnCode = sqlite3_prepare_v2(handle_db(DB_USE),
                                    sSql,
                                    strlen(sSql),
                                    &stmt,
                                    NULL);
   FREE_SQL(sSql)

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf(QUERY_ERROR_NOSTAT);
      return 0;
   }

   if (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      char* sTemp;

      upUserData = (USER*)malloc(sizeof(USER));
      /* 
         Wenn eine Zeile zur gew�nschten User-Id gefunden wurde, lies die Werte
         aus den Spalten.
         Wir kennen die Abfolge der Spalten, da wir sie selbst angeben.
      */
      sTemp = (char*)sqlite3_column_text(stmt, 0);
      upUserData->sNickname = (char*)calloc(strlen(sTemp) + 1, sizeof(char));
      strcpy(upUserData->sNickname, sTemp);
      /*
         Jeder String aus der Datenbank muss einzeln neu kopiert werden.
         Dazu wird Speicherplatz entsprechend der String-L�nge + 1 (f�r das
         abschlie�ende \0) alloziert und der String entsprechend hinein kopiert.

         Gilt auch f�r die folgende Anweisungen
      */


      sTemp = (char*)sqlite3_column_text(stmt, 1);
      upUserData->sName = (char*)calloc(strlen(sTemp) + 1, sizeof(char));
      strcpy(upUserData->sName, sTemp);

      sTemp = (char*)sqlite3_column_text(stmt, 2);
      upUserData->sLastname = (char*)calloc(strlen(sTemp) + 1, sizeof(char));
      strcpy(upUserData->sLastname, sTemp);

      sTemp = (char*)sqlite3_column_text(stmt, 3);
      upUserData->sRegisterdate = (char*)calloc(strlen(sTemp) + 1, sizeof(char));
      strcpy(upUserData->sRegisterdate,sTemp);

      upUserData->sPassword = NULL;
      /* Abschlie�end die User-ID eintragen */
      upUserData->iUserId = iUserId;
   }

   /* NULL oder den Zeiger auf die neuen Userdaten zur�ckgeben */
   return upUserData;
}

char insert_game_data(GAME* upGameData)
/*
   ============================================================================
   Speichert Spiel-Daten in der Datenbank, abh�ngig von der �begebenen User-Id,
   gibt den Erfolgsstatus( 0=Fehler, 1=Erfolg ) zur�ck
      1. Parameter: Der Zeiger auf die Spieldaten-Struktur
      2. R�ckgabewert: Der Status (0 = Fehler, 1 = Erfolg)
   ============================================================================
*/
{
   /* Variablen-Definition */
   char* sSql;
   int iReturnCode;
   char* sErrMsg;

   /* Verbereitung der Query mit Parametern */
   sSql = sqlite3_mprintf(QUERY_INSERT_GAMEDATA,
      upGameData->iUserId, upGameData->iSeconds, upGameData->iHelps,
      upGameData->iFilled, upGameData->cFinished, upGameData->cRanked,
      upGameData->iMode);

   /* Query auf der Datenbank ausf�hren und anschlie�end den String freigeben */
   iReturnCode = sqlite3_exec(handle_db(DB_USE),
                              sSql,
                              NULL,
                              NULL,
                              &sErrMsg);
   FREE_SQL(sSql)
   
   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf("Daten konnten nicht gespeichert werden\n");
      free(sErrMsg);
      return 0;
   }
   return 1;
}

int get_user_games(int iUserId, int **ippGamesIdList)
/*
   ============================================================================
   Holt die Spiel-Ids des �bergebenen Benutzers aus der Datenbank und schreibt
   die Adresse der Ergebnisse in den �bergebenen Zeiger
      1. Parameter: Die User-Id
      2. Parameter: Der Zeiger, der auf die Ergebnisse zeigen soll
      3. R�ckgabewert: Die Ergebnis-L�nge
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


   /* Query auf Datenbank ausf�hren, Query l�schen */
   iReturnCode = sqlite3_prepare_v2(handle_db(DB_USE),
                                    sSql,
                                    strlen(sSql),
                                    &stmt,
                                    NULL);
   FREE_SQL(sSql)

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf(QUERY_ERROR_NOSTAT);
      return 0;
   }

   while (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      /* Schreibe die gefundenen Spiel-Ids an die �bergebene Adresse */
      iLastLength = append_to_intarray(ippGamesIdList,
                                       atoi((char*)sqlite3_column_text(stmt, 0)),
                                       iLastLength);
   }

   /* Gib die Anzahl der gefundenen Spiele zur�ck */
   return iLastLength;
}

GAME* get_game_data(int iGameId)
/*
   ============================================================================
   Holt die Spiel-Daten zu einer Spiel-Id aus der Datenbank
      1. Parameter: Die User-Id
      2. R�ckgabewert: Der Zeiger auf die Spieldaten
   ============================================================================
*/
{
   /* Variablen-Definition */
   int iReturnCode;
   char* sSql;
   sqlite3_stmt *stmt;

   GAME* gSpieldaten = NULL;
   
   /* Query mit Parametern vorbereiten */
   sSql = sqlite3_mprintf(QUERY_GET_GAMEDATA, iGameId); 


   /* Query auf Datenbank ausf�hren, Query l�schen */
   iReturnCode = sqlite3_prepare_v2(handle_db(DB_USE),
                                    sSql,
                                    strlen(sSql),
                                    &stmt,
                                    NULL);
   FREE_SQL(sSql)

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf(QUERY_ERROR_NOSTAT);
      return 0;
   }

   if (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      char* sDate;
      /* 
         Erst wenn tats�chlich Daten in der DB gefunden wurden wird Speicher f�r
         die Spieldaten
      */
      gSpieldaten = (GAME*)malloc(sizeof(GAME));

      /* 
         Wenn eine Zeile zur gew�nschten User-Id gefunden wurde, lies die Werte
         aus den Spalten
      */
      gSpieldaten->iUserId = atoi((char*)sqlite3_column_text(stmt, 0));
      gSpieldaten->iSeconds = atoi((char*)sqlite3_column_text(stmt, 2));
      gSpieldaten->iHelps = atoi((char*)sqlite3_column_text(stmt, 3));
      gSpieldaten->iFilled = atoi((char*)sqlite3_column_text(stmt, 4));
      gSpieldaten->cFinished = atoi((char*)sqlite3_column_text(stmt, 5));
      gSpieldaten->cRanked = atoi((char*)sqlite3_column_text(stmt, 6));
      gSpieldaten->iMode = atoi((char*)sqlite3_column_text(stmt, 7));

      /* Der Datumsstring muss kopiert werden */
      sDate = (char*)sqlite3_column_text(stmt, 1);
      gSpieldaten->sDate = (char*)malloc(strlen(sDate) + 1);
      strcpy(gSpieldaten->sDate,sDate);

      /* Wir kennen die Abfolge der Spalten, da wir sie selbst angeben */
   }

   /* NULL oder den Zeiger auf die geholten Spieldaten zur�ck geben */
   return gSpieldaten;
}

int get_best_games(int iMode, GAMERANKING* rpGames, int iLength)
/*
   ============================================================================
   Holt die besten Spiele zu einem Schwierigkeitsgrad, samt relevanter Daten
   aus der Datenbank.
      1. Parameter: Der Schwierigkeitsgrad
      2. Parameter: Das Array aus Spieldaten
      3. Parameter: Die L�nge des Arrays
      4. R�ckgabewert: Die Anzahl der Ergebnisse
   ============================================================================
*/
{
   /* Variablen-Definition */
   int iResultLength = 0;
   int iReturnCode;
   char* sSql;
   GAMERANKING* ugActGame = rpGames;
   sqlite3_stmt *stmt;

   
   /* Query mit Parametern vorbereiten */
   sSql = sqlite3_mprintf(QUERY_GET_TOPRANKED,
                          HELP_FACTOR,
                          FILLED_FACTOR,
                          iMode,
                          iLength);


   /* Query auf Datenbank ausf�hren, Query l�schen */
   iReturnCode = sqlite3_prepare_v2(handle_db(DB_USE),
                                    sSql,
                                    strlen(sSql),
                                    &stmt,
                                    NULL);
   FREE_SQL(sSql)

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf(QUERY_ERROR_NOSTAT);
      return 0;
   }

   while (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      /*
         sText dient als Zwischenspeicher f�r einen Zeiger auf Text aus der
         Datenbank
      */
      char* sText;

      /* Wir kennen die Abfolge der Spalten, da wir sie selbst angeben */
      rpGames[iResultLength].iGameId = atoi((char*)sqlite3_column_text(stmt, 0));
      rpGames[iResultLength].iUserId = atoi((char*)sqlite3_column_text(stmt, 1));
      rpGames[iResultLength].iSeconds = atoi((char*)sqlite3_column_text(stmt, 2));
      rpGames[iResultLength].iMode = atoi((char*)sqlite3_column_text(stmt, 3));
      rpGames[iResultLength].iHelps = atoi((char*)sqlite3_column_text(stmt, 4));
      rpGames[iResultLength].iFilled = atoi((char*)sqlite3_column_text(stmt, 5));

      /*
         Der aus der Datenbank erhaltene Text muss kopiert werden.
         Dazu muss entsprechender Speicher alloziert werden.
      */
      sText = (char*)sqlite3_column_text(stmt, 6);
      rpGames[iResultLength].sGameDate = (char*)malloc(strlen(sText) + 1);
      strcpy(rpGames[iResultLength].sGameDate, sText);

      /* Der aus der Datenbank erhaltene Text muss kopiert werden */
      sText = (char*)sqlite3_column_text(stmt, 7);
      rpGames[iResultLength].sUserName = (char*)malloc(strlen(sText) + 1);
      strcpy(rpGames[iResultLength].sUserName, sText);

      rpGames[iResultLength].iScore = atoi((char*)sqlite3_column_text(stmt, 8));
      iResultLength++;
   }

   /* Die erhaltene Anzahl an Spielen wird zur�ckgegeben */
   return iResultLength;
}

int get_best_user_games(int iMode, int iUserId, GAMERANKING* rpGames, int iLength)
/*
   ============================================================================
   Holt die besten Spiele eines Spielers zu einem Schwierigkeitsgrad, samt 
   relevanter Daten aus der Datenbank.
      1. Parameter: Der Schwierigkeitsgrad
      2. Parameter: Die UserId
      3. Parameter: Das Array aus Spieldaten
      4. Parameter: Die L�nge des Arrays
      5. R�ckgabewert: Die Anzahl der Ergebnisse
   ============================================================================
*/
{
   /* Variablen-Definition */
   int iResultLength = 0;
   int iReturnCode;
   char* sSql;
   GAMERANKING* ugActGame = rpGames;
   sqlite3_stmt *stmt;

   
   /* Query mit Parametern vorbereiten */
   sSql = sqlite3_mprintf(QUERY_GET_USER_TOP,
                          HELP_FACTOR,
                          FILLED_FACTOR,
                          iMode,
                          iUserId,
                          iLength);


   /* Query auf Datenbank ausf�hren, Query l�schen */
   iReturnCode = sqlite3_prepare_v2(handle_db(DB_USE),
                                    sSql,
                                    strlen(sSql),
                                    &stmt,
                                    NULL);
   FREE_SQL(sSql)

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf(QUERY_ERROR_NOSTAT);
      return 0;
   }

   while (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      /*
         sText dient als Zwischenspeicher f�r einen Zeiger auf Text aus der
         Datenbank
      */
      char* sText;

      /* Wir kennen die Abfolge der Spalten, da wir sie selbst angeben */
      rpGames[iResultLength].iGameId = atoi((char*)sqlite3_column_text(stmt, 0));
      rpGames[iResultLength].iUserId = atoi((char*)sqlite3_column_text(stmt, 1));
      rpGames[iResultLength].iSeconds = atoi((char*)sqlite3_column_text(stmt, 2));
      rpGames[iResultLength].iMode = atoi((char*)sqlite3_column_text(stmt, 3));
      rpGames[iResultLength].iHelps = atoi((char*)sqlite3_column_text(stmt, 4));
      rpGames[iResultLength].iFilled = atoi((char*)sqlite3_column_text(stmt, 5));

      /*
         Der aus der Datenbank erhaltene Text muss kopiert werden.
         Dazu muss entsprechender Speicher alloziert werden.
      */
      sText = (char*)sqlite3_column_text(stmt, 6);
      rpGames[iResultLength].sGameDate = (char*)malloc(strlen(sText) + 1);
      strcpy(rpGames[iResultLength].sGameDate, sText);

      /* Der aus der Datenbank erhaltene Text muss kopiert werden */
      sText = (char*)sqlite3_column_text(stmt, 7);
      rpGames[iResultLength].sUserName = (char*)malloc(strlen(sText) + 1);
      strcpy(rpGames[iResultLength].sUserName, sText);

      rpGames[iResultLength].iScore = atoi((char*)sqlite3_column_text(stmt, 8));
      iResultLength++;
   }

   /* Die erhaltene Anzahl an Spielen wird zur�ckgegeben */
   return iResultLength;
}

void delete_gameranking_data(GAMERANKING* rpGame, char cWholeStruct)
/*
   ============================================================================
   L�scht die aus der Datenbank kopierten Strings in der GAMERANKING-Struktur
   und bei Bedarf die Struktur selbst
      1. Parameter: das Usergame
   ============================================================================
*/
{
   /* Datums-text l�schen */
   free(rpGame->sGameDate);
   /* Spielernamen l�schen */
   free(rpGame->sUserName);

   if (cWholeStruct)
   {
      /* Wenn gew�nscht Struktur l�schen */
      free(rpGame);
   }
}

void delete_game_data(GAME* gpGame, char cWholeStruct)
/*
   ============================================================================
   L�scht die aus der Datenbank kopierten Strings in der GAME-Struktur und bei
   Bedarf die Struktur selbst
      1. Parameter: das Game
   ============================================================================
*/
{
   /* Datums-text l�schen */
   free(gpGame->sDate);

   if (cWholeStruct)
   {
      /* Wenn gew�nscht Game l�schen */
      free(gpGame);
   }
}

void delete_user_data(USER* upUser, char cWholeStruct)
/*
   ============================================================================
   L�scht die aus der Datenbank kopierten Strings in der USER-Struktur und bei
   Bedarf die Struktur selbst
      1. Parameter: Die Userdaten
   ============================================================================
*/
{
   /* Nachnamen-String l�schen */
   free(upUser->sLastname);
   /* Spielernamen-String l�schen */
   free(upUser->sNickname);
   /* Passwort-String l�schen */
   free(upUser->sPassword);
   /* Datums-String l�schen */
   free(upUser->sRegisterdate);

   if (cWholeStruct)
   {
      /* Wenn gew�nscht die Struktur l�schen */
      free(upUser);
   }
}


/* EOF */
