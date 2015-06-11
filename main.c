/*
   ========================================================================
   Autoren     : GruppeB
                 Lennard Malessa
                 Marian Klemm
                 Kevin Schorn
                 Jan Gützlaff
                 Erick Seifert
                 Tristan Lucas
   Firma       : HHBK
   Dateiname   : menue.c
   Datum       : 08.06.2015
   Beschreibung: Nutzerführung
   Compiler    : Visual Studio & gcc
   Version     : 1.0
   Programmschnittstelle: int main(void)
   Das Programm gibt den Wert 0 oder 1 zurueck.
   Das Programm erwartet beim Aufruf keine Argumente
   ========================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include "menue.h"
#include "tableui.h"

int main(void)
/*
   ==========================================================================
   Programmeinstiegspunkt, ruft das Hauptmenü auf
   Rückgabewert: 0   
   ==========================================================================
*/
{
   if (getTerminalSize()) {
      menue();
   } else {
      printf("Fehler: Ihr Terminal ist zu klein.\n"\
             "Bitte vergoessern sie die Groesse Ihres Terminals auf mindestens"\
             " %d x %d Zeichen.", MIN_WIDTH, MIN_HEIGHT);
      system("pause");
   }
	return 0;
}
