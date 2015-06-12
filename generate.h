/* 
   ============================================================================
   Programmname: generate.h
   Autor       : Tristan Lucas
                 Heinrich-Hertz-Berufskolleg
   Datum       : 
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Modularisierung der Erstellung eines gel�sten Sudokus mit Hilfe
                 eines "backtracking"-Algorithmus
   ============================================================================ 
*/
#ifndef _GENERATE_H
#define _GENERATE_H

/* 
   ============================================================================
   Nach au�en verf�gbare Schnittstellen
   ============================================================================ 
*/

void create_sudoku();
void easy_sudoku(int iDifficulty);

#endif

