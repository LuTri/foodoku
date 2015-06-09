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
		if (iPosition + page * ((MAX_HEIGHT - 2) * (MAX_WIDTH - 4)) < strlen(fancy_output)) {
			mvaddch( 2 + (iPosition/(MAX_HEIGHT -2)),57 + (iPosition%(MAX_WIDTH - 4))
				,fancy_output[iPosition + page * ((MAX_HEIGHT - 2) * (MAX_WIDTH - 4))] );
		} else {
			break;
		}
	}

	for(iPosition = 0; (iPosition < (int)strlen(fancy_output) 
		&& iPosition < ( (MAX_WIDTH - 4) * (MAX_HEIGHT -2))); iPosition++)
	{

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