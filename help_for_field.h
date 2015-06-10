/* 
   ============================================================================
   Programmname: help_for_field.h
   Autor       : Marian Klemm
                 Heinrich-Hertz-Berufskolleg
   Datum       : 10.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung:  Generierung von validen Zahlen f�r ein Feld im
                  Sudoku als Vorschlag/Hilfe f�r den Nutzer.
   ============================================================================ 
*/

/* 
   ============================================================================
   Pr�prozessor-Anweisungen, globale defines
   ============================================================================ 
*/

/* 
   ============================================================================
   Nach au�en verf�gbare Schnittstellen
   ============================================================================ 
*/

int showValuesForHelp(int iCurrentArray[9][9], int x, int y);