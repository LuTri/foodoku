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