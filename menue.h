/*
========================================================================
Autor       : GruppeB (Lennard Malessa)
Firma       : HHBK
Dateiname   : menue.h
Datum       : 09.06.2015
Beschreibung: Header Datei für mit den Funktionsprototypen für menue.c
Compiler    : Visual Studio
Version     : 1.0
Programmschnittstelle: int main(void)
Das Programm gibt den Wert 0 oder 1 zurueck.
Das Programm erwartet beim Aufruf keine Argumente
========================================================================
*/

#include "stdio.h"
#include <stdlib.h>

#define  REGELN ""\
	"Regeln:\n"\
   "Sudoku ist ein Zahlenraetsel. Das quadratische Spielfeld ist "\
	"in drei mal drei Bloecke unterteilt. Jeder dieser Bloecke besteht wiederum "\
	"aus 9 Kaestchen. Das gesamte Spielfeld besteht somit aus 81 Kaestchen die "\
	"sich 9 Spalten und 9 Reihen zuordnen lassen. Von diesen 81 Feldern sind "\
	"typischerweise 22 bis 36 Felder vorgegeben, in denen Zahlen zwischen 1 bis 9 "\
	"stehen.\n"\
	"\n"\
	"Das Ziel des Spiels ist, das Spielfeld zu vervollstaendigen. Dabei sind "\
	"die vom Spiel vorgegebenen Zahlen nicht veraenderbar. Die leeren Kaestchen "\
	"muessen mit Ziffern gefuellt werden. Dabei gelten folgende drei Regeln:\n"\
	"- In Jeder Zeile duerfen die Ziffern von 1 bis 9 nur einmal vorkommen\n"\
	"- in jeder Spalte duerfen die Ziffern von 1 bis 9 nur einmal vorkommen\n"\
	"- In jedem Block duerfen die Ziffern von 1 bis 9 nur einmal vorokmmen\n"\
	"Das Spiel ist beendet, wenn alle Kaestchen korrekt gefuellt sind."

/* 
   ============================================================================
   Funktionsprototypen
   ============================================================================ 
*/

void menue (void);
int iSpieleauswahl(void);
void eingeloggt(void);
void GewertetesSpiel(void);
void FreiesSpiel(void);
void Einloggen(void);
void Registrieren(void);
void Spieleregeln(void);

//Globale Variable der UserID
extern int iGUserID;
