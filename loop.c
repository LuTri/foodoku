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

//Präprozessorkonstanten

#define KEY_K 'r'
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

   int		iPlaying,
      iHelp = 0,
      iX,
      iY;

   char	cRanked,
      cTaste = 0,
      cTempTaste = 0;

   time_t	tBeg,
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



   //Zuweisung von User ID und Schwierigkeitsgrad in die Gamedata-Struktur

   this_game.iUserId	= iUserId;
   this_game.iMode		= iSchwierigkeit;


   //Spielaufbau

   startup_sudoku();

   tBeg = time(&tBeg);

      //Prüfung ob Ranglisten-Spiel oder nicht
      if(iSpielart == 1 && iUserId != 0){
         this_game.cRanked = 1;
      }else{
         this_game.cRanked = 0;
      }

   //Rendering-Loop
   while(1){
      //Spielfeld anzeigen
      show_ui(iHelp);

      //Abfangen der Tastendrücke
      cTaste = get_input();

      if(cTaste == KEY_K)
         //Vorschläge für Feld (K)
      {
         get_cursor_pos(&iX, &iY);
         //	showValuesForHelp(cSudoku, iX, iY);
         this_game.iHelps++;
      }


      if(cTaste == KEY_Q)
         //Quit (Q)
      {
         iPlaying = 0;
         break;
      }

      if(cTaste == KEY_F)
         //Einzelnes Feld füllen (F)
      {
         this_game.iFilled++;
      }


      if(cTaste == KEY_ENTER)
         //Eingabe (ENTER)
      {
         if(cTempTaste != 0){
            get_cursor_pos(&iX, &iY);
            cSudoku[iY][iX] = cTempTaste - '0';
            cTempTaste = 0;
         }
      }


      if(cTaste == KEY_R)
         //Regeln (R)
      {
         if(iHelp == 0){
            iHelp = 1;
         }else{
            iHelp = 0;
         }
      }

      if(cTaste >= NUM_1 || cTaste <= cTaste == NUM_9)
         ///// Überleg dir ob die die Bedingung oben so benutzen möchtest
         ///// Wenn man mehrmals hintereinander die Enter-Taste drückt wird die
         ///// letzte Zahl eingetragen -> Vielleicht nach eintragen auf 0 setzen?
         //ZAHLEN(1-9)
      {
         cTempTaste = cTaste;
      }


      if(cTaste == CURS_UP 
         ||	cTaste == CURS_DOWN
         ||	cTaste == CURS_LEFT
         ||	cTaste == CURS_RIGHT
         ||	cTaste == CURS_RIGHT
         )
         //PFEILTASTEN
      {
         move_cursor(cTaste);
      }
   }

   // Berechnung der Spieldauer
   tEnd = time(&tEnd);
   this_game.iSeconds = (int) difftime(tEnd, tBeg);
   ///// Es fehlen noch die angezeigten Hilfe + die automatisch gefüllten Felder

   shutdown_sudoku();
   // Insert in Bestenliste
   if(this_game.iUserId != 0){
      insert_game_data(&this_game);
   }
}

