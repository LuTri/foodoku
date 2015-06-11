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
<<<<<<< HEAD
#include "curses.h"
=======
#include "curses_os.h"
>>>>>>> a8648ecefca005caf7ba8cce9219169e04ec47ac
#include "tableui.h"
#include "menue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char will_fit(int iColumn, int iMax, char* fancy_output, int iStringpos)
/*
	Eine Funktion zum "klären" des Textes in der Box. Soll \n, \t und Leerzeichen
	erkennen und automatisch in die neue Zeile verschoben werden.
		1. Parameter	:	die aktuelle Länge des Strings
		2. Parameter	:	die Maximale länge des Strings in einer Reihe
		3. Parameter	:	Texteingabe
		4. Parameter	:	aktuelle Position des Strings
		5. Rückegabewert:
*/
{
	// Variabl Deklaration
	int iPos, iLength;
	iMax = MAX_WIDTH - 2; // ALs Parameter eintragen
	
	iPos = iStringpos;

	while( fancy_output[iPos] != '\n' && fancy_output[iPos] != '\t' && fancy_output[iPos] != ' ' && iPos < strlen(fancy_output))
	{	
		iPos++;
	}

	iLength = iPos - iStringpos;

	if(iLength < iMax - iColumn)
	{
		return iLength;
	}
	else
	{
		return -1;
	}
	// Maybe return fitting length
}

void clear_fancy_box(void)
/*
	Funktion zum füllen/klären des derzeitigen Feldes -> optionale Funktion
		1. Parameter	:	keiner
		2. Rückgabewert	:	ein mit Leerzeichen gefülltes Feld
*/
{
	// initialisierte Variabeln
	int iHoehe_v2, iBreite_v2;


	//Begin mit füllen der Box 
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

	/*
		while()
	*/
	for(iPosition = 0; (iPosition < (int)strlen(fancy_output) 
		&& iPosition < ( (MAX_WIDTH - 4) * (MAX_HEIGHT -2))); iPosition++)
	{
		int posx, posy;
		posx = 57 + (iPosition%(MAX_WIDTH - 4));
		posy = 2 + (iPosition/(MAX_WIDTH - 4));
		if (iPosition + page * ((MAX_HEIGHT - 2) * (MAX_WIDTH - 4)) < strlen(fancy_output)) {
			int i_to_write = will_fit(posx, 0, fancy_output, iPosition);

			// i_to_write > 0 -> write the fitting word
			// i_to_write = -1 -> new line WILL_FIT
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
	2. Rückgabewert	:	Eingabe CUrsor Bewegung oder Abbruch Sequenz
*/
{
	char cInput;

	do
	{
		cInput = get_input();

		show_fancy_box(REGELN, 0);

	}while(cInput != 'q' );
<<<<<<< HEAD
}
=======
}
>>>>>>> a8648ecefca005caf7ba8cce9219169e04ec47ac
