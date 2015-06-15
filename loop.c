/*
   ============================================================================
   Programmname: loop.c
   Autor : Jan Gützlaff
           Tristan Lucas
           Lennard Malessa
   Heinrich-Hertz-Berufskolleg
   Datum : 08.06.2015
   Thema : Sudoku
   Version : 1.0
   Beschreibung: Aufrufen und Benutzung des UI
   ============================================================================
*/



//INCLUDE
#include "os.h"
#include <time.h>
#include "db.h"
#include "tableui.h"
#include "menue.h"
#include "generate.h"
#include "fancy_box.h"
#include "help_for_field.h"

//Präprozessorkonstanten

#define KEY_RETURN 13

#define NUM_1 '0'+1
#define NUM_2 '0'+2
#define NUM_3 '0'+3
#define NUM_4 '0'+4
#define NUM_5 '0'+5
#define NUM_6 '0'+6
#define NUM_7 '0'+7
#define NUM_8 '0'+8
#define NUM_9 '0'+9

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

   int iPlaying = 1,
       iHelp = 0,
       iX,
       iY;

   char cTaste = 0,
        cTempTaste = 0;

   char* cHelpText = 0;
   char cBuffer[40];

   time_t tBeg,
          tEnd;

   GAME this_game;
   this_game.iUserId = iUserId;
   
   //Zuweisung von User ID und Schwierigkeitsgrad in die Gamedata-Struktur

   this_game.iHelps = 0;
   this_game.iFilled	= 0;
   this_game.iUserId	= iUserId;
   this_game.iMode = iSchwierigkeit;


   //Spielaufbau
   startup_sudoku();
   //gelöstes Sudoku erstellen
   create_sudoku(iSchwierigkeit-1);
   //lösbare Sudoku aus gelöstem Sudoku erstellen
   //easy_sudoku();
   //Start-Sudoku merken
   prepare_start_sudoku();

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
      get_cursor_pos(&iX,&iY);
      if (!(check_input(iX,iY))) {
         char* cTemp;
         if (cHelpText == 0) {
            cHelpText = enhanced_infotext(0,"\n   DIESES FELD IST VORGEGEBEN!!");
         } else {
            cTemp = enhanced_infotext(cHelpText,"\n   DIESES FELD IST VORGEGEBEN!");
            free(cHelpText);
            cHelpText = cTemp;
         }
      }

      //Info das alle Felder ausgefüllt sind und das Spiel beendet werden kann
      if(isfullfilled() == 1)
      {
         char* cTemp;
         cTemp = enhanced_infotext(cHelpText,"\n Alle Felder ausgefuellt.\n"
            " Spiel mit Q beenden.");
         free(cHelpText);
         cHelpText = cTemp;
      }


      show_ui(iHelp, 0, cHelpText);
      if (cHelpText != 0) {
         free(cHelpText);
         cHelpText = 0;
      }

      //Abfangen der Tastendrücke
      cTaste = get_input();

      //Hier werden den einzelnen Tastendrücken Anweisungen zugewiesen
      switch(cTaste)
      {
         //Tasten K für Kandidaten, Q zum Beenden, F um das aktuelle Feld zu lösen, R für die Regeln, Enter für die Eingabe
      case 'k':
         if (check_input(iX, iY)) {
            iHelp = 1;
         }
         MakeValuesForHelp(iX,iY);
         this_game.iHelps++;
         break;

      case 'q':
         iPlaying = 0;
         break;

      case 'f':
         if (check_input(iX,iY)) {
            this_game.iFilled++;
            cShownSudoku[iY][iX] = cSudoku[iY][iX];
         }
         clear_help(-1,-1);
         iHelp = 0;
         move_cursor(cTaste);
         cTempTaste = 0;
         break;
      case 'r':
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
         sprintf(cBuffer,"\n%7c mit ENTER eintragen", cTaste);
         cHelpText = enhanced_infotext(0,cBuffer);
         cTempTaste = cTaste;
         break;

         //Navigation durch Pfeiltasten

      case KEY_RETURN:
         if(cTempTaste != 0 && check_input(iX, iY)){

            cShownSudoku[iY][iX] = cTempTaste - '0';
            cTempTaste = 0;
         }
      case CURS_UP:
      case CURS_DOWN:
      case CURS_LEFT:
      case CURS_RIGHT:
         clear_help(-1,-1);
         iHelp = 0;
         move_cursor(cTaste);
         cTempTaste = 0;
         break;

      }

   }
   // Berechnung der Spieldauer
   tEnd = time(&tEnd);
   this_game.iSeconds = (int) difftime(tEnd, tBeg);

   this_game.cFinished = show_result(this_game.iHelps,
                                     this_game.iFilled,
                                     this_game.iSeconds);
   shutdown_sudoku();
   // Insert in Bestenliste
   if(this_game.iUserId != 0){
      insert_game_data(&this_game);
   }
}