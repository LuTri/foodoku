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

//Regeln die in der Funktion Spieleregeln angezeigt werden
#define  REGELN ""\
	"Sudoku ist ein Zahlenr\204tsel. Das quadratische Spielfeld ist\n"\
	"in drei mal drei Bl\224cke unterteilt. Jeder dieser Bl\224cke besteht"\
    "wiederum\n"\
	"aus 9 K\204stchen. Das gesamte Spielfeld besteht somit aus 81 K\204stchen"\
    "die\n"\
	"sich 9 Spalten und 9 Reihen zuordnen lassen. Von diesen 81 Feldern sind\n"\
	"typischerweise 22 bis 36 Felder vorgegeben, in denen Zahlen zwischen"\
    "1 bis 9\nstehen\n"\
	"\n"\
	"Das Ziel des Spiels ist, das Spielfeld zu vervollst\204ndigen. Dabei"\
    "sind\n"\
	"die vom Spiel vorgegebenen Zahlen nicht ver\204nderbar. Die leeren"\
	"K\204stchen\n"\
	"muessen mit Ziffern gefuellt werden. Dabei gelten folgende drei"\
    "Regeln\n\n"\
	"- In Jeder Zeile d\201rfen die Ziffern von 1 bis 9 nur einmal"\
	"vorkommen\n"\
	"- in jeder Spalte d\201rfen die Ziffern von 1 bis 9 nur einmal"\
	"vorkommen\n"\
	"- In jedem Block d\201rfen die Ziffern von 1 bis 9 nur einmal"\
	"vorkommen\n"\
	"Das Spiel ist beendet, wenn alle K\204stchen korrekt gef\201llt"\
	"sind\n\n"\

/* 
   ============================================================================
   Funktionsprototypen
   ============================================================================ 
*/

void menue (void);
void eingeloggt(int);
void GewertetesSpiel(void);
void FreiesSpiel(void);
void Einloggen(void);
void Registrieren(void);
void Spieleregeln(void);

//Globale Variable der UserID
extern int iGUserID;
