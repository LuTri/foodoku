/*
============================================================================
Programmname: loop.c
Autor : Jan Gützlaff
Heinrich-Hertz-Berufskolleg
Datum : 08.06.2015
Thema : Sudoku
Version : 1.0
Beschreibung: Aufrufen und Benutzung des UI
============================================================================
*/



//INCLUDE
#include <time.h>
#include "db.h"
#include "tableui.h"
#include "menue.h"
#include "generate.h"
#include "fancy_box.h"

//Präprozessorkonstanten

#define KEY_K 'k'
#define KEY_Q 'q'
#define KEY_F 'f'
#define KEY_R 'r'
#define KEY_ENTER 13
#define NUM_1 '0'+1
#define NUM_2 '0'+2
#define NUM_3 '0'+3
#define NUM_4 '0'+4
#define NUM_5 '0'+5
#define NUM_6 '0'+6
#define NUM_7 '0'+7
#define NUM_8 '0'+8
#define NUM_9 '0'+9

///// Formatierung bitte einmal überarbeiten
///// Keine Tabs benutzen!


///// Vielleicht ein switch-case anstatt der ganzen ifs?

void game_loop(int iSchwierigkeit, int iSpielart, int iUserId)
   /*
   Erstellen des Sudoku-Feldes
   1. Parameter:	iSchwierigkeitsgrad
   2. Parameter:	Spielart (Ranglisten oder freies Spiel)
   3. Parameter:	User ID (für die Bestenliste)
   */
{

   // Variablendeklaration

   /*Aktuelle Position, aktueller Spielstatus(0 oder 1), Hilfeanzeige (0 oder 1),
   Ranglisten-Spiel (0 oder 1), aktueller Tastendruck, Zeitvariablen
   */

   int iPlaying = 1, iHelp = 0, iX, iY;

   int x;
   int y;

   char	cRanked,
      cTaste = 0,
      cTempTaste = 0;

   time_t tBeg,
      tEnd;


   // Gamedata-Struktur

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

   GAME this_game;
   this_game.iUserId = iUserId;
   
   prepare_start_sudoku();

   //Zuweisung von User ID und Schwierigkeitsgrad in die Gamedata-Struktur

   this_game.iHelps = 0;
   this_game.iFilled	= 0;
   this_game.iUserId	= iUserId;
   this_game.iMode = iSchwierigkeit;


   //Spielaufbau

   startup_sudoku();
   create_sudoku();
   easy_sudoku(iSchwierigkeit-1);

   tBeg = time(&tBeg);

   //Prüfung ob Ranglisten-Spiel oder nicht
   if(iSpielart == 1 && iUserId != 0){
      this_game.cRanked = 1;
   }else{
      this_game.cRanked = 0;
   }

   //Rendering-Loop
   while(iPlaying){
      //Spielfeld anzeigen
      show_ui(iHelp, 0);

      //Abfangen der Tastendrücke
      cTaste = get_input();

      //Hier werden den einzelnen Tastendrücken Anweisungen zugewiesen
      switch(cTaste)
      {
         //Tasten K für Kandidaten, Q zum Beenden, F um das aktuelle Feld zu lösen, R für die Regeln, Enter für die Eingabe
      case KEY_K:
         get_cursor_pos(&iY, &iX);
         this_game.iHelps++;
         break;

      case KEY_Q:
         iPlaying = 0;
         break;

      case KEY_F:
         this_game.iFilled++;
         break;

      case KEY_ENTER:

         get_cursor_pos(&iX, &iY);

         if(cTempTaste != 0 && check_input(iX, iY)){

            cShownSudoku[iY][iX] = cTempTaste - '0';
            cTempTaste = 0;
         }
         break;

      case KEY_R:
         fancy_loop();
         break;

         //Eingabe der Zahlen 1-9
      case NUM_1:
      case NUM_2:
      case NUM_3:
      case NUM_4:
      case NUM_5:
      case NUM_6:
      case NUM_7:
      case NUM_8:
      case NUM_9:
         cTempTaste = cTaste;
         break;

         //Navigation durch Pfeiltasten
      case CURS_UP:
      case CURS_DOWN:
      case CURS_LEFT:
      case CURS_RIGHT:
         move_cursor(cTaste);
         cTempTaste = 0;
         break;

      }

   }
   // Berechnung der Spieldauer
   tEnd = time(&tEnd);
   this_game.iSeconds = (int) difftime(tEnd, tBeg);

   show_result(this_game.iHelps, this_game.iFilled, this_game.iSeconds);
   shutdown_sudoku();
   // Insert in Bestenliste
   if(this_game.iUserId != 0){
      insert_game_data(&this_game);
   }
}