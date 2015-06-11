/* 
   ============================================================================
   Programmname: fancy_box.c
   Autor       : Erick Seifert
   Schule	   : Heinrich-Hertz-Berufskolleg
   Datum       : 08.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Generierung eines/r seitlich angezeigten Feldes/Box zur 
				 Darstellung von Hinweisen und Hilfen

   ============================================================================ 
*/

/*
   ============================================================================
   Defines
   ============================================================================
*/          
#define MAX_HEIGHT 37
#define MAX_WIDTH 37
#define BORDERS '@'


/*
   ============================================================================
   Prototypen
   ============================================================================
*/
char will_fit(int iColumn, int iMax, char* fancy_output, int iStringpos);
char show_fancy_box(char* fancy_output, int page);
void clear_fancy_box(void);
void fancy_loop(void);