/* 
   ============================================================================
   Programmname: sudoku_solving.h
   Autor       : Marian Klemm
                 Heinrich-Hertz-Berufskolleg
   Datum       : 09.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Generierung des vollständigen Sudokus mittels Backtracking.
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

int checkIfOk(int iCurrentArray[9][9], int x, int y);

int setValueForField(int iCurrentArray[9][9], int x, int y);