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
#include "tableui.h"
#include "menue.h"

//Präprozessorkonstanten

#define KEY_K 107
#define KEY_Q 11
#define KEY_F 102
#define KEY_R 114
#define KEY_ENTER 13
#define NUM_1 49
#define NUM_2 50
#define NUM_3 51
#define NUM_4 52
#define NUM_5 53
#define NUM_6 54
#define NUM_7 55
#define NUM_8 56
#define NUM_9 57



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
			cTaste,
			cTempTaste;

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



	
	//Zuweisung von User ID und Schwierigkeitsgrad in die Gamedata-Struktur

	this_game.iUserId	= iUserId;
	this_game.iMode		= iSchwierigkeit;


//Spielaufbau

		startup_sudoku();

		tBeg = time(&tBeg);

//Rendering-Loop
		while(iPlaying){

			//Prüfung ob Ranglisten-Spiel oder nicht
			if(iSpielart == 1 && iUserId != 0){
				this_game.cRanked = 1;
			}else{
				this_game.cRanked = 0;
			}

//Spielfeld anzeigen
		show_ui(iHelp);
			
	
//Abfangen der Tastendrücke
		cTaste = get_input();

				if(cTaste == KEY_K)
				//Vorschläge für Feld (K)
				{
					get_cursor_pos(&iX, &iY);
					showValuesForHelp(cSudoku, iX, iY);
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
						set_sudoku_pos(iX, iY, cTempTaste);
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

				if(		cTaste == NUM_1
					||	cTaste == NUM_2
					||	cTaste == NUM_3
					||	cTaste == NUM_4
					||	cTaste == NUM_5
					||	cTaste == NUM_6
					||	cTaste == NUM_7
					||	cTaste == NUM_8
					||	cTaste == NUM_9
				)
				//ZAHLEN(1-9)
				{
					cTempTaste = cTaste;
				}


				if(		cTaste == CURS_UP 
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
		this_game.iSeconds = (int) difftime(tBeg, tEnd);

// Insert in Bestenliste
		if(this_game.iUserId != 0){
			insert_game_data(&this_game);
		}


}

