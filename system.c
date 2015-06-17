/* 
   ============================================================================
   Programmname: system.c
   Autor       : Tristan Lucas
                 Heinrich-Hertz-Berufskolleg
   Datum       : 09.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Stellt eine Eingabe-verschleiernde Funktion zur Passwort-
                 Eingabe bereit
   Programmschnittstelle: Siehe zugeh�rige Header-Datei  
   ============================================================================ 
*/

#include "conio.h"

void passwd_input(char cBuff[500])
/* 
   ============================================================================
   Liest ein maximal 200 Zeichen langes Passwort ein und verschleiert dabei die
   Ausgabe mit '*'
      1. Parameter: Das Array, in dem das Passwort gespeichert werden soll
   ============================================================================ 
*/
{
   /* Variablen-Definition und Initialisierung */
   char cCharacter;
   int i=0;

   /* Erste Abfang einer Tastatureingabe */
   cCharacter = _getch();
   /* F�r jede gedr�ckte Taste wirde ein '*' auf dem Bildschirm ausgegeben */
   _putch('*');
   while (cCharacter != '\n' && cCharacter != '\r' && i < 499)
   {
      /*
         Solange der Nutzer nicht die Enter-Taste dr�ckt oder das Passwort zu
         lang wird, wird die Eingabe fortgef�hrt
      */
      cBuff[i++] = cCharacter;
      cCharacter = _getch();
         _putch('*');
   }
   /* Der String wird terminiert */
   cBuff[i] = '\0';

   while (cCharacter != '\n' && cCharacter != '\r') {
      /* 
         Gruppe C wollte die M�glichkeit haben, dass der Nutzer mehr Zeichen
         eingeben kann als der Puffer Speicher hat. Wir haben uns entschieden
         die �berlappende Eingabe zu simulieren und zu ignorieren
      */
      cCharacter = _getch();
      _putch('*');
   }
}
