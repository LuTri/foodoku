/* 
   ============================================================================
   Programmname: password.c
   Autor       : Tristan Lucas
                 Heinrich-Hertz-Berufskolleg
   Datum       : 12.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Stellt eine Eingabe-verschleiernde Funktion zur Passwort-
                 Eingabe bereit

   Programmschnittstelle: Siehe zugeh�rige Header-Datei  
   ============================================================================ 
*/
#include "password.h"
#include <conio.h>

void passwd_input(char cBuff[200])
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
   while (cCharacter != '\n' && cCharacter != '\r' && i < 200)
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
}
