/* 
   ============================================================================
   Programmname : os.h
   Autor        : Tristan Lucas
                  Heinrich-Hertz-Berufskolleg
   Datum        : 09.06.2015
   Thema        : Sudoku
   Version      : 1.0
   Beschreibung : Regelt die Cross-Plattform Fähigkeit des Sudoku-Spiels
   Schnittstelle: Auf Unix-System: pause();
   ============================================================================ 
*/

#ifndef _CURSES_OS_H
#define _CURSES_IS_H

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_NO_WARNINGS
/* Microsoft hat alle gängigen Ein- und Ausgabefunktionen
   als gefährlich eingestuft und durch neue Varianten ersetzt.
   Mit den neuen Funktionen sind die Programme aber nicht
   kompatible zu anderen Entwicklungsumgebungen.
   Bei Verwendung der alten Funktionen wird eine Warnung ausgegeben.
   Diese Warnung kann durch die obige Definition unterdrückt werden.
*/

#include <stdlib.h>
#include "curses.h"

#define FREE_SQL(x) free(x);
#define CLEAR_SCREEN system("cls"); fflush(stdin); fflush(stdout);
#define SYSTEM_PAUSE system("pause");

/*
   ============================================================================
   Funktionsprototypen
   ============================================================================
*/

void passwd_input(char cBuff[200]);

#endif

