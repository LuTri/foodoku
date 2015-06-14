/* 
   ============================================================================
   Dateiname   : sudoku.h
   Autor       : Tristan Lucas
                 Heinrich-Hertz-Berufskolleg
   Datum       : 07.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Implementierung von Basis Sudoku Regeln
   ============================================================================ 
*/

#ifndef _SUDOKU_H
#define _SUDOKU_H

/* 
   ============================================================================
   Funktionsprototypen
   ============================================================================ 
*/

char can_be_on_coord(int iX, int iY, char cVal, char cUseRootSudoku);

#endif
