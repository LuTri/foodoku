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
#include "os.h"
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
}
