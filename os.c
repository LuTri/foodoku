/* 
   ============================================================================
   Programmname: os.c
   Autor       : Tristan Lucas
                 Heinrich-Hertz-Berufskolleg
   Datum       : 09.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Stellt f�r Unix-System ein �quivalent zu "system("pause")" zur
                 Verf�gung
   ============================================================================ 
*/

#include "os.h"

#ifdef OS_LINUX
void pause(void)
/* 
   ============================================================================
   Imitiert die auf Windows-Plattformen verf�gbare Anweisung 'system("pause")'
   nach
      Keine Parameter
   ============================================================================ 
*/
{
   printf("Press any key to continue...\n");
   getchar();
}
#endif