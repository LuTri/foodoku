/* 
   ============================================================================
   Programmname: help_for_field.h
   Autor       : Marian Klemm
                 Heinrich-Hertz-Berufskolleg
   Datum       : 10.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung:  Generierung von validen Zahlen für ein Feld im
                  Sudoku als Vorschlag/Hilfe für den Nutzer.
   ============================================================================ 
*/

/* 
   ============================================================================
   Präprozessor-Anweisungen, globale defines
   ============================================================================ 
*/

/* 
   ============================================================================
   Nach außen verfügbare Schnittstellen
   ============================================================================ 
*/

int showValuesForHelp(int iCurrentArray[9][9], int x, int y);