/* 
   ============================================================================
   Programmname: fancy_box.c
   Autor       : Erick Seifert
   Schule	   : Heinrich-Hertz-Berufskolleg
   Datum       : 08.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Generierung eines/r seitlich angezeigten Feldes/Box zur 
				 Darstellung von Hinweisen und Hilfen

   Programmschnittstelle: Siehe zugehörige Header-Datei  
   ============================================================================
*/

/*
	includes
*/
#include "fancy_box.h"
#include "curses.h"
#include "tableui.h"
#include "menue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char show_fancy_box(char* fancy_output, int page)
/*
	anzeigen des Feldes für Hilfen und Hinweise
	1. Parameter	: Inhalt des Feldes
	2. Rückgabewert	: Inhalt des Felders innerhalb der Box
*/
{
	// initislisiere Variabeln
	int iHoehe, iBreite, iPosition;

	// erstellen der Box oder des Feldes
	for(iHoehe = 0; iHoehe < MAX_HEIGHT; iHoehe++ )// Schleife zur Erstellung der maximalen Höhe
	{
		for(iBreite = 0; iBreite < MAX_WIDTH; iBreite++)//Schleife zur Erstellung der maximalen Breite
		{
			if(iHoehe == 0 || iBreite == 0 || iHoehe == MAX_HEIGHT -1 || iBreite == MAX_WIDTH -1)
			{
				mvaddch( 0 + iHoehe, 55 + iBreite , BORDERS);// Abfrage, wenn dann füge '#' ein
			}
			help_text(fancy_output);//Aufruf Funktion help_text()
		}
	}

	//Positionierung des Textes
	for(iPosition = 0; (iPosition < (int)strlen(fancy_output) 
		&& iPosition < ( (MAX_WIDTH - 4) * (MAX_HEIGHT -2))); iPosition++)
	{
		int posx, posy;
		posx = 57 + (iPosition%(MAX_WIDTH - 4));
		posy = 2 + (iPosition/(MAX_WIDTH - 4));
		if (iPosition + page * ((MAX_HEIGHT - 2) * (MAX_WIDTH - 4)) < strlen(fancy_output)) {
			mvaddch(posy,posx
				,fancy_output[iPosition + page * ((MAX_HEIGHT - 2) * (MAX_WIDTH - 4))] );
		} else {
			break;
		}
	}
}

void fancy_loop(void)
/*
	Funktion zum Abfangen von Fehleingaben und Curserbewegung
	1. Parameter	:		
	2. Rückgabewert	:	
*/
{
	char cInput;

	do
	{
		cInput = get_input();

		show_fancy_box(REGELN, 0);

	}while(cInput != 'q' );
}

char will_fit(int iColumn, int iMax, char* fancy_output, int iStringpos)
/*
	Lorme Ipsum
		1. Parameter	:	
		2. Parameter	:	
		3. Parameter	:	
		4. Parameter	:	
		5. Rückegabewert:
*/
{
	iMax = MAX_WIDTH - 2; // ALs Parameter eintragen

	for(iColumn = 0; iColumn < (MAX_HEIGHT -2); iColumn++)
	{
		for(iStringpos = 0; (iStringpos < (int)strlen(fancy_output) 
			&& iStringpos < (MAX_WIDTH - 4)); iStringpos++)
		{
			if( iStringpos == '\n' || iStringpos == '\t')
			{
				printw("foo");
			}
		}
	}
}

void clear_fancy_box(void)
{
		// initislisiere Variabeln
	int iHoehe_v2, iBreite_v2;

	for(iHoehe_v2 = 0; iHoehe_v2 < MAX_HEIGHT; iHoehe_v2++ )// Schleife zur Erstellung der maximalen Höhe
	{
		for(iBreite_v2 = 0; iBreite_v2 < MAX_WIDTH; iBreite_v2++)//Schleife zur Erstellung der maximalen Breite
		{
			if(iHoehe_v2 == 0 || iBreite_v2 == 0 || iHoehe_v2 == MAX_HEIGHT -1 || iBreite_v2 == MAX_WIDTH -1)
			{
				mvaddch( 0 + iHoehe_v2, 55 + iBreite_v2 , ' ');// Abfrage, wenn dann füge ' ' ein
			}
		}
	}
}