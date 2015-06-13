/* 
   ============================================================================
   Programmname: os.c
   Autor       : Tristan Lucas
                 Heinrich-Hertz-Berufskolleg
   Datum       : 09.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Stellt für Unix-System ein Äquivalent zu "system("pause")" zur
                 Verfügung
   ============================================================================ 
*/

#include "os.h"

#ifdef OS_LINUX
void pause(void)
/* 
   ============================================================================
   Imitiert die auf Windows-Plattformen verfügbare Anweisung 'system("pause")'
   nach
      Keine Parameter
   ============================================================================ 
*/
{
   printf("Press any key to continue...\n");
   getchar();
}
#endif