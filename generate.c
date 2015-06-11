#include "generate.h"
#include "curses_os.h"
#include "tableui.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
   int x;
   int y;
} COORDINATE;

void testprint(void) {
   int i,j;
   for (i = 0; i < BOUNDARY; i++) {
      for (j = 0; j < BOUNDARY; j++) {
         mvaddch(i,j,'0' + cSudoku[j][i]);
      }
   }
   refresh();
}

char can_be_in_square(int iX, int iY, char cVal)
{
   int iXStart, iYStart;
   int i, j;
   iXStart = iX / BOUNDARY_ROOT;
   iYStart = iY / BOUNDARY_ROOT;

   for (i = 0; i < BOUNDARY_ROOT; i++) {
      for (j = 0; j < BOUNDARY_ROOT; j++) {
         if (cSudoku[i + (iYStart * BOUNDARY_ROOT)][j + (iXStart * BOUNDARY_ROOT)] == cVal) {
            return 0;
         }
      }
   }
   return 1;
}

char can_be_in_line(int iX, int iY, char cVal) {
   int i;
   for (i = 0; i < BOUNDARY; i++) {
      if ((cSudoku[i][iX] == cVal) || (cSudoku[iY][i] == cVal)) {
         return 0;
      }
   }
   return 1;
}

int possible_positions(char cValue, COORDINATE coordPositions[BOUNDARY_SQUARE])
{
   int iX,iY;
   int iNumPositions = 0;
   for (iY = 0; iY < BOUNDARY; iY++)
   {
      for (iX = 0; iX < BOUNDARY; iX++)
      {
         if (can_be_in_square(iX,iY,cValue) && can_be_in_line(iX,iY,cValue) && cSudoku[iY][iX] == 0)
         {
            coordPositions[iNumPositions].x = iX;
            coordPositions[iNumPositions].y = iY;
            iNumPositions++;
         }
      }
   }

   return iNumPositions;
}

void shift(int iStart, int iEnd, char *cArray) {
   int i;
   for (i = iStart; i < iEnd - 1; i++) {
      cArray[i] = cArray[i + 1];
   }
}

void randomize_coords(int iLength, COORDINATE coordArray[BOUNDARY_SQUARE]) {
   char cPositions[BOUNDARY_SQUARE];
   char cRandomPositions[BOUNDARY_SQUARE];
   COORDINATE coordRandom[BOUNDARY_SQUARE];
   int iActLength = iLength;
   int i;
   int iPos;

   for (i = 0; i < iLength; i++) {
      cPositions[i] = i;
   }

   for (i = 0; i < iLength; i++)
   {
      iPos = rand() % iActLength;
      cRandomPositions[i] = cPositions[iPos];
      shift(iPos, iActLength--, cPositions);
   }

   for (i = 0; i < iLength; i++) {
      coordRandom[i].x = coordArray[cRandomPositions[i]].x;
      coordRandom[i].y = coordArray[cRandomPositions[i]].y;
   }

   for (i = 0; i < iLength; i++)
   {
      coordArray[i].x = coordRandom[i].x;
      coordArray[i].y = coordRandom[i].y;
   }
}

char fill_backtrack(int iDepth) {
   static int iCounter = 0;
   COORDINATE coordPossiblePositions[BOUNDARY_SQUARE];
   int iActPos = 0;
   int iX, iY;
   char cSuccess = 0;
   int iNumPossible;
   char cValue;

   if (iDepth == (BOUNDARY_SQUARE)) {
      return 1;
   }

   iCounter++;

   if (iCounter > 20000) {
      iCounter = 0;
      return -1;
   }

   cValue = (iDepth / BOUNDARY) + 1;
   iNumPossible = possible_positions(cValue, coordPossiblePositions);

   if (iNumPossible == 0)
   {
      return 0;
   }

   randomize_coords(iNumPossible, coordPossiblePositions);

   while (!cSuccess && iActPos < iNumPossible) {
      iX = coordPossiblePositions[iActPos].x;
      iY = coordPossiblePositions[iActPos].y;

      cSudoku[iY][iX] = cValue;
      cSuccess = fill_backtrack(iDepth + 1);
      
      if (cSuccess != 1) {
         cSudoku[iY][iX] = 0;
         iActPos++;
      }
   }
   if (cSuccess!= 1)
   {
      cSudoku[iY][iX] = 0;
   }
   return cSuccess;
}

char create_sudoku(char cNotFinished, int seed)
{
   char cResult;
   srand(time(NULL));
   do
   {
      null_sudoku();
      cResult = fill_backtrack(0);
   } while (cResult != 1);
}
