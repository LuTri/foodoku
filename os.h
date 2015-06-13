/* 
   ============================================================================
   Programmname: os.h
   Autor       : Tristan Lucas
                 Heinrich-Hertz-Berufskolleg
   Datum       : 09.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Regelt die Cross-Plattform Fähigkeit des Sudoku-Spiels
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
#include <stdio.h>
#include "password.h"

#ifndef OS_LINUX
/*
   ============================================================================
   Standardmäßig wird mit Windows-kompatiblen defines gearbeitet
   ============================================================================
*/

#include "curses.h"

#define FREE_SQL(x) free(x);
#define CLEAR_SCREEN system("cls"); fflush(stdin); fflush(stdout);
#define SYSTEM_PAUSE system("pause");

#else

/*
   ============================================================================
   Dieser Bereich wird vom Präprozessor ausgeführt, wenn das Define "OS_LINUX"
   an den Compiler übergeben wird
   ============================================================================
*/

#include <ncurses.h>

#define FREE_SQL(x)
#define CLEAR_SCREEN system("clear"); fflush(stdin); fflush(stdout);
#define SYSTEM_PAUSE pause();

#endif

/*
   ============================================================================
   Funktionsprototypen
   ============================================================================
*/

#endif

