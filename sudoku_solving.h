/* 
   ============================================================================
   Programmname: sudoku_solving.h
   Autor       : Marian Klemm
                 Heinrich-Hertz-Berufskolleg
   Datum       : 09.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Generierung des vollst�ndigen Sudokus mittels Backtracking.
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

<<<<<<< HEAD
int checkIfOk(int iCurrentArray[9][9], int x, int y);

int setValueForField(int iCurrentArray[9][9], int x, int y);
=======
char checkIfOk(char cCurrentArray[9][9], int x, int y);

char setValueForField(char cCurrentArray[9][9], char cCompleteArray[9][9], int x, int y);
>>>>>>> a8648ecefca005caf7ba8cce9219169e04ec47ac
