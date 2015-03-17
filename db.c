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

   /*
      Das alte Array freigeben und den �bergebenen Zeiger auf das neue zeigen
      lassen
   */
   free(*ippArray);
   *ippArray = ipTemp;
   
   /* Die neue L�nge zur�ckgeben */
   return iLength;
}

char* encrypt(char* cpClear)
/*
   ============================================================================
   Verschl�sselt einen String mit einer einfachen Vigen�re-Verschl�sselung. Der
   Schl�ssel ist der Head-Datei zu entnehmen
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
   char* cpEncrypted;

   iSecretLen = strlen(ENCRYPTION_SECRET);
   iClearLen = strlen(cpClear);

   /* 
      Der verschl�sselte Text wird immer genau doppelt so lang sein wie der
      Klartext, da jedes Zeichen durch 2 Stellen im hexadezimalen System dar-
      gestellt werden kann. Ein Zeichen wird zus�tzlich f�r das \0 alloziert.
   */
   cpEncrypted = (char*)calloc(iClearLen * 2 + 1,sizeof(char));
   
   for (iStrIdx = 0; iStrIdx < iClearLen; iStrIdx++)
   {
      char cTemp;
      /*
         Auf jedes Zeichen im Klartext wird das (forlaufende) Zeichen des
         Schl�ssels addiert und das Resultat anschlie�end auf den Wertebereich
         f�r 8-Bit begrenzt
      */
      cTemp = ((cpClear[iStrIdx] + ENCRYPTION_SECRET[iEncIdx++]) % 255);

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
      
      strcat(cpEncrypted, cBuf);
   }

   /* Der verschl�sselte String wird zur�ckgegeben */
   return cpEncrypted;
}

sqlite3* get_db(void)
/*
   ============================================================================
   �ffnet eine Datenbank und speichert diese zur Wiederverwendung als statische
   Variable zwischen
      1. R�ckgabewert: Der Zeiger auf die SQLite-Datenbank
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   static sqlite3 *dbpDatabase = NULL;
   int iReturnCode;

   if (dbpDatabase == NULL)
   {
      /* 
         Falls diese Programminstanz noch keine Datenbank ge�ffnet hat, �ffne
         eine neue
      */
      iReturnCode = sqlite3_open(DATABASE_FILE, &dbpDatabase);
      if (iReturnCode != SQLITE_OK)
      {
         /* Wenn beim �ffnen ein Fehler auftrat, setze den Zeiger auf NULL */
         sqlite3_close(dbpDatabase);
         dbpDatabase = NULL;
      }
   }

   /* Der g�ltige Datenbankzeiger oder NULL wird zur�ckgegeben */
   return dbpDatabase;
}

int authenticate(char* cpNickname, char* cpPassword)
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
   char* cpSql;
   char* cpErrMsg;
   sqlite3_stmt *stmt;
   char* cpEncrypted = encrypt(cpPassword);

   /* Query mit Parametern vorbereiten */
   cpSql = sqlite3_mprintf("SELECT user_id FROM userdata WHERE spielername='%s"\
                           "' AND passwort='%s';", cpNickname, cpEncrypted);

   /* Query auf der Datenbank ausf�hren und anschlie�end den String freigeben */
   iReturnCode = sqlite3_prepare_v2(get_db(),
                                    cpSql,
                                    strlen(cpSql),
                                    &stmt,
                                    &cpErrMsg);
   free(cpSql);

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf("Ein Fehler ist aufgetreten: \"%s\"\n", cpErrMsg);
      free(cpErrMsg);
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

char register_user(char* cpNickname,
                   char* cpName,
                   char* cpLastname,
                   char* cpPassword)
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
   char* cpSql;
   int iReturnCode;
   char* cpEncrypted;
   char* cpErrMsg;

   /* 
      Das Passwort sollte nicht im Klartext gespeichert werden, es wird
      verschl�sselt. Wie: Siehe Kommentare zu encrypt().
   */
   cpEncrypted = encrypt(cpPassword);
   

   /* Query mit Parametern vorbereiten */
   cpSql = sqlite3_mprintf("INSERT INTO userdata(passwort,spielername,vorname,na"\
      "chname,anmeldedatum) VALUES('%s','%s','%s','%s',date('now'));",
      cpEncrypted, cpNickname, cpName, cpLastname);

   /* Den verschl�sselten String wieder l�schen*/
   free(cpEncrypted);

   /* Query auf der Datenbank ausf�hren und anschlie�end den String freigeben */
   iReturnCode = sqlite3_exec(get_db(),
                              cpSql,
                              NULL,
                              NULL,
                              &cpErrMsg);
   free(cpSql);

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf("Ein Fehler ist aufgetreten: \"%s\"\n", cpErrMsg);
      free(cpErrMsg);
      return 0;
   }
   return 1;
}

char get_userdata(int iUserId,
                  char** cppNickname,
                  char** cppName,
                  char** cppLastname,
                  char** cppDate)
/*
   ============================================================================
   Holt die Nutzerdaten aus der Datenbank und schreibt diese an die �bergebenen
   Adressen
      1. Parameter: Die User-Id
      2. Parameter: Die Adresse, an die der Spielername geschrieben werden soll
      3. Parameter: Die Adresse, an die der Vorname geschrieben werden soll
      4. Parameter: Die Adresse, an die der Nachname geschrieben werden soll
      5. Parameter: Die Adresse, an die das Anmeldedatum geschrieben werden
                    soll
      6. R�ckgabewert: Der Status (0 = Fehler, 1 = Erfolg)
   ============================================================================
*/
{
   /* Variablen-Definition */
   int iReturnCode;
   char* cpSql;
   char* cpErrMsg;
   sqlite3_stmt *stmt;
   
   /* Query mit Parametern vorbereiten */
   cpSql = sqlite3_mprintf("SELECT spielername, vorname, nachname, anmeldedatu"\
                           "m FROM userdata WHERE user_id = %d;", iUserId); 


   /* Query auf Datenbank ausf�hren, Query l�schen */
   iReturnCode = sqlite3_prepare_v2(get_db(),
                                    cpSql,
                                    strlen(cpSql),
                                    &stmt,
                                    &cpErrMsg);
   free(cpSql);

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf("Ein Fehler ist aufgetreten: \"%s\"\n", cpErrMsg);
      free(cpErrMsg);
      return 0;
   }

   if (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      /* 
         Wenn eine Zeile zur gew�nschten User-Id gefunden wurde, lies die Werte
         aus den Spalten
      */
      *cppNickname = (char*)sqlite3_column_text(stmt, 0);
      *cppName = (char*)sqlite3_column_text(stmt, 1);
      *cppLastname= (char*)sqlite3_column_text(stmt, 2);
      *cppDate = (char*)sqlite3_column_text(stmt, 3);
      /* Wie kennen die Abfolge der Spalten, da wir sie selbst angeben */

      /* 1 ( = Erfolg ) zur�ckgeben*/
      return 1;
   }
   else
   {
      /* Falls keine Zeile gefunden wurde gib 0 ( = Fehler ) zur�ck */
      return 0;
   }
}

char insert_game_data(int iUserId,
                      int iSeconds,
                      int iHelps,
                      int iFilled,
                      char cFinished,
                      char cRanked)
/*
   ============================================================================
   Speichert Spiel-Daten in der Datenbank, abh�ngig von der �begebenen User-Id,
   gibt den Erfolgsstatus( 0=Fehler, 1=Erfolg ) zur�ck
      1. Parameter: Die User-Id des Spielers
      2. Parameter: Die Zeit die das Spiel in Sekunden dauerte
      3. Parameter: Anzahl der gezeigten Hilfestellungen
      4. Parameter: Anzahl der gef�llten Felder
      5. Parameter: Angabe, ob das Spiel gewonnen wurde (1 Ja, 0 Nein)
      6. Parameter: Angabe, ob das Spiel gewertet wurde (1 Ja, 0 Nein)
      7. R�ckgabewert: Der Status (0 = Fehler, 1 = Erfolg)
   ============================================================================
*/
{
   /* Variablen-Definition */
   char* cpSql;
   int iReturnCode;
   char* cpErrMsg;

   /* Verbereitung der Query mit Parametern */
   cpSql = sqlite3_mprintf("INSERT INTO games(user, spielzeitpunkt, zeit, "\
      "angezeigte_hilfen, gefuellte_felder, abgeschlossen, gewertet) VALUES(%d"\
      ",datetime('now','localtime'),%d,%d,%d,%d,%d);",
      iUserId, iSeconds, iHelps, iFilled, cFinished, cRanked);

   /* Query auf der Datenbank ausf�hren und anschlie�end den String freigeben */
   iReturnCode = sqlite3_exec(get_db(),
                              cpSql,
                              NULL,
                              NULL,
                              &cpErrMsg);
   free(cpSql);
   
   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf("Ein Fehler ist aufgetreten: \"%s\"\n", cpErrMsg);
      free(cpErrMsg);
      return 0;
   }
   return 1;
}

int get_user_games(int iUserId, int **ippGames)
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
   char* cpSql;
   char* cpErrMsg;
   int iLastLength = 0;
   sqlite3_stmt *stmt;
   
   /* Query mit Parameter vorbereiten */
   cpSql = sqlite3_mprintf("SELECT game_id FROM games WHERE user_id = %d;",
                           iUserId); 


   /* Query auf Datenbank ausf�hren, Query l�schen */
   iReturnCode = sqlite3_prepare_v2(get_db(),
                                    cpSql,
                                    strlen(cpSql),
                                    &stmt,
                                    &cpErrMsg);
   free(cpSql);

   if (iReturnCode != SQLITE_OK)
   {
      /*
         Falls ein Fehler auftrat, den Fehlertext ausgeben, freigeben und die
         Funktion mit 0-R�ckgabe beenden
      */
      printf("Ein Fehler ist aufgetreten: \"%s\"\n", cpErrMsg);
      free(cpErrMsg);
      return 0;
   }

   while (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      /* Schreibe die gefundenen Spiel-Ids an die �bergebene Adresse */
      iLastLength = append_to_intarray(ippGames,
                                       atoi((char*)sqlite3_column_text(stmt, 0)),
                                       iLastLength);
   }

   /* Gib die Anzahl der gefundenen Spiele zur�ck */
   return iLastLength;
}
