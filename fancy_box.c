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
/*
============================================================================
Defines
============================================================================
*/          
#define MAX_HEIGHT 37 // Wert der maximalen Höhe
#define MAX_WIDTH 37 // Wert der maximalen Breite
#define BORDERS '@' // Rahmen Zeichen

#define INFO_TEXT ""\
   "------- q: Spiel beenden -------- "\
   "------ r: Regeln anzeigen ------- "\
   "---- k: Kandidaten anzeigen ----- "\
   "-- f: Feld automatisch fuellen -- \n\n\n"\

#define BOX_WIDTH 37 // Rahmengröße
#define BOX_HEIGHT 37 // Rahmengröße
#define TEXT_WIDTH BOX_WIDTH - 4 // Textbereich Breite
#define TEXT_HEIGHT BOX_HEIGHT - 4 //Textbereich Höhe
#define START_X 57 // Start X-Achse
#define START_Y 2 // Start Y-Achse
#define MAX_PAGES 5 // Maximale Anzahl an Seiten

#define SPACE -2 // Wert für Leerzeichen
#define NEWLINE -3 // Wert für Zeilenumbruch

int will_fit(int iColumn, const char* fancy_output, int iStringpos)
/*
   Eine Funktion zum "klären" des Textes in der Box. Soll \n, \t und Leerzeichen
   erkennen und automatisch in die neue Zeile verschoben werden.
   1. Parameter	 : die aktuelle Position (Spalte) innerhalb der Box
   2. Parameter	 : der gesamte Text
   3. Parameter	 : die aktuelle Position im Text
   4. Rückegabewert: die Länge die noch in die Box passt oder -1
*/
{
   // Variabel Deklaration
   int iLength = 0;

   //IF-ABfrage , je nach dem was an der Position erkannt wird
   if (fancy_output[iStringpos] == '\n')
   {
      //Rückgabewert: Zeilenumbruch
      return NEWLINE;
   } else if (fancy_output[iStringpos] == ' ')
   {
      //Rückgabewert Leerzeichen
      return SPACE;
   }

   //While-Schleife, wenn Bedingung true, dann eine Position nach vorne
   while( fancy_output[iLength + iStringpos] != '\n'
      && fancy_output[iLength + iStringpos] != ' '
      && (iLength + iStringpos) < (int)strlen(fancy_output))
   {	
      iLength++;
   }

   //IF-Abfrage
   if(iLength <= TEXT_WIDTH - iColumn)
   {
      //Rückgabewert: iLength
      return iLength;
   }
   else
   {
      //Rückgabewert -1
      return -1;
   }
}

void make_page(const char* ccText, char cFancyPage[TEXT_HEIGHT][TEXT_WIDTH + 1])
/*
   ============================================================================
   Funktion zur Generierung des Textbereiches
      1. Parameter:  Zeiger ccText
      2. Parameter:  Textbereich(Textfeld Größe)
      Rückgabewert:  
   ============================================================================
*/
{
   //Variabel Deklaration und Definition
   int iStringPos = 0,
      iPosX = 0,
      iPosY = 0;

   //Variabel INitialisierung
   int iNFitting;
   int i;

   while (iStringPos < (int)strlen(ccText))
   {
      iNFitting = will_fit(iPosX, ccText, iStringPos);
      if (iNFitting == SPACE)
      {
         if (iPosX < TEXT_WIDTH - 1)
         {
            cFancyPage[iPosY][iPosX] = ' ';
         }
         else
         {
            cFancyPage[iPosY][iPosX] = '\0';
         }

         iPosX++;
         iStringPos++;
      }
      else if(iNFitting == NEWLINE)
      {
         iPosX = 0;
         iPosY++;
         iStringPos++;
         cFancyPage[iPosY][iPosX] = '\0';
      }
      else if(iNFitting > 0)
      {
         for (i = 0; i < iNFitting; i++)
         {
            cFancyPage[iPosY][iPosX + i] = ccText[i + iStringPos];
         }
         iPosX += iNFitting;
         iStringPos += iNFitting;
         cFancyPage[iPosY][iPosX] = '\0';
      }
      else
      {
         cFancyPage[iPosY][iPosX] = '\0';
         iPosX = 0;
         iPosY++;
      }
   }
   iPosY++;

   while (iPosY < TEXT_HEIGHT)
   {
      cFancyPage[iPosY++][0] = '\0';
   }
}

void clear_fancy_box(void)
/*
   ============================================================================
   Funktion zum füllen/klären des derzeitigen Feldes -> optionale Funktion
   1. Parameter	:	keiner
   2. Rückgabewert	:	ein mit Leerzeichen gefülltes Feld
   ============================================================================
*/
{
   //Variabel Initialisierung
   int iHoehe_v2, iBreite_v2;


   //Begin mit füllen der Box
   // Schleife zur Erstellung der maximalen Höhe
   for(iHoehe_v2 = 0; iHoehe_v2 < MAX_HEIGHT; iHoehe_v2++ )
   {
      //Schleife zur Erstellung der maximalen Breite
      for(iBreite_v2 = 0; iBreite_v2 < MAX_WIDTH; iBreite_v2++)
      {
         //IF-Abfrage, wenn dann wird ein Leerzeichen eingefügt
         if(iHoehe_v2 >= 1 || iBreite_v2 >= 1 
            || iHoehe_v2 <= MAX_HEIGHT -1 || iBreite_v2 <= MAX_WIDTH -1)
         {
            mvaddch( 0 + iHoehe_v2, 55 + iBreite_v2 , ' ');
         }
      }
   }
}

void show_fancy_box(const char* fancy_output)
/*
   ============================================================================
   anzeigen des Feldes für Hilfen und Hinweise
   1. Parameter	: Inhalt des Feldes
   2. Parameter   : Die angezeigte Seite des Textes
   2. Rückgabewert: Die tatsächlich angezeigte Seite
   ============================================================================
*/
{
   //Variabel Initialisierung
   int iHoehe, iBreite;
   int iPosY;
   const char *cUsedString;

   //Variabel Deklaration und Definition
   char cFancyPage[TEXT_HEIGHT][TEXT_WIDTH + 1];

   //IF-Abfrage, wenn ungleich 0 dann cUsedString, ansonsten INFO__TEXT
   if(fancy_output != 0)
   {
      cUsedString = (char*)fancy_output;
   }
   else
   {
      cUsedString = INFO_TEXT;
   }

   //Funktionsaufruf make_pake()
   make_page(cUsedString, cFancyPage);

   // erstellen des Rahmens
   // Schleife zur Erstellung der maximalen Höhe
   for(iHoehe = 0; iHoehe < MAX_HEIGHT; iHoehe++ )
   {
      //Schleife zur Erstellung der maximalen Breite
      for(iBreite = 0; iBreite < MAX_WIDTH; iBreite++)
      {
         // IF-Abfrage, wenn dann füge '#' ein
         if(iHoehe == 0 || iBreite == 0 || iHoehe == MAX_HEIGHT -1 || iBreite == MAX_WIDTH -1)
         {
            mvaddch( 0 + iHoehe, (START_X - 2) + iBreite , BORDERS);
         }
      }
   }

   for (iPosY = 0; iPosY < TEXT_HEIGHT; iPosY++) {
      mvprintw(START_Y + iPosY, START_X, cFancyPage[iPosY]);
   }
}

void fancy_loop(void)
/*
   ============================================================================
   Zeigt solange die Spiel-Regeln an, bis der Nutzer "q" drückt
      Parameter: keine
      Rückgabewerte: keine
   ============================================================================
*/
{
   //Variabel Initialisierung
   int cInput;

   //DO/WHILE-Schleife, mehrere Funktionsaufrufe, solange Eingabe ungleich 'q'
   do
   {
      clear_fancy_box();
      show_fancy_box(REGELN);
      refresh();
      cInput = get_input();
   }while(cInput != 'q');

   //Funktionsaufrufe zum cleanen der Fancy Box
   clear_fancy_box();
   refresh();
}

char* enhanced_infotext(char* cRootString, char* cAdditionalString)
/*
   ============================================================================
   FUnktion um String auszugeben
      1. Parameter:  Zeiger cRootString
      2. Parameter:  Zeiger cAddiotionalString
      Rückgabewert:  Textausgabe
   ============================================================================
*/
{
   //Variabel Initialisierung
   char* cResult;
   char* cActualRootString = 0;

   //IF-Abfrage, wenn == 0, dann INFOTEXT, ansonsten crootString
   if (cRootString == 0)
   {
      cActualRootString = INFO_TEXT;
   }
   else
   {
      cActualRootString = cRootString;
   }

   //Erstellung des Rückgabewertes
   cResult = (char*)malloc(strlen(cActualRootString)
      + strlen(cAdditionalString) + 1);

   //String zusammenführen
   strcpy(cResult,cActualRootString);
   strcat(cResult,cAdditionalString);

   //Rückgabewert, ist der komplette String
   return cResult;
}