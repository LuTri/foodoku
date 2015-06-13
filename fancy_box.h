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
   Prototypen
   ============================================================================
*/

int show_fancy_box(const char* fancy_output);
void fancy_loop(void);

char *enhanced_infotext(char* cRootString, char* cAdditionalString);