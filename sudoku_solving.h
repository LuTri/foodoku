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

char checkIfOk(char cCurrentArray[9][9], int x, int y);

char setValueForField(char cCurrentArray[9][9], char cCompleteArray[9][9], int x, int y);