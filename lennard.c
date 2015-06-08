/*
========================================================================
Autor       : GruppeB (Lennard Malessa)
Firma       : HHBK
Dateiname   : 
Datum       : 01.01.0001
Beschreibung: Sudoku Menü und Subemnüs
Compiler    : Visual Studio
Version     : 1.0
Programmschnittstelle: int main(void)
Das Programm gibt den Wert 0 oder 1 zurueck.
Das Programm erwartet beim Aufruf keine Argumente
========================================================================
*/

#define _CRT_SECURE_NO_DEPRECATE 1
#define  ""\
"Regeln "Sudoku ist ein Zahlenraetsel. Das quadratische Spielfeld ist\n" 
"in drei mal drei Bloecke unterteilt. Jeder dieser Bloecke\n" 
"besteht wiederum aus\n"\
"9 Kaestchen. Das gesamte Spielfeld besteht somit aus 81 Kaestchen die\n"\
"sich 9 Spalten und 9 Reihen zuordnen lassen. Von diesen 81 Feldern\n"\
"sind typischerweise 22 bis 36 Felder vorgegeben, in denen Zahlen\n" \
"zwischen 1 bis 9 stehen.\n"\
      "\n"\
		"Das Ziel des Spiels ist, das Spielfeld zu vervollstaendigen. Dabei sind\n"\
		"die vom Spiel vorgegebenen Zahlen nicht veraenderbar. Die leeren Kaestchen\n"\
		"muessen mit Ziffern gefuellt werden. Dabei gelten folgende drei Regel\n"\
		"- In Jeder Zeile duerfen die Ziffern von 1 bis 9 nur einmal vorkommen\n"\
		"- in jeder Spalte duerfen die Ziffern von 1 bis 9 nur einmal vorkommen\n"\
		"- In jedem Block duerfen die Ziffern von 1 bis 9 nur einmal vorokmmen\n"\
		"Das Spiel ist beendet, wenn alle Kaestchen korrekt gefuellt sind\n"\

/* Microsoft hat alle gängigen Ein- und Ausgabefunktionen
als gefährlich eingestuft und durch neue Varianten ersetzt.
Mit den neuen Funktionen sind die Programme aber nicht
kompatible zu anderen Entwicklungsumgebungen.
Bei Verwendung der alten Funktionen wird eine Warnung ausgegeben.
Diese Warnung kann durch die obige Definition unterdrückt werden.
*/

#include <stdlib.h>
#include "db.h"
#include <stdio.h>
#include <string.h>

/*
==================================

==================================
*/
int main (void)
{
	int iAuswahl = 0;
	int iError;
	int iSchleife = 0;

	do
	{
		system("cls");
		printf("            SUDOKU TASKFORCE            \n");
		printf("            ________________\n");
		printf("\n\n\n");

		printf("1: Einloggen\n");
		printf("2: Registrieren\n");
		printf("3: Freies Spiel\n");
		printf("4: Spieleanleitung\n");
		printf("5: Beenden\n\n");
		do
		{
			printf("Auswahl: ");
			fflush(stdin);
			iError = scanf("%i", &iAuswahl);
		}
		while(iError == 0 || iAuswahl < 1 || iAuswahl > 5);

		if (iAuswahl == 1)
		{
			Einloggen();
		}

		if (iAuswahl == 2)
		{
			Registrieren();
		}

		if (iAuswahl == 3)
		{
			FreiesSpiel();
		}

		if(iAuswahl == 4)
		{
			Spieleregeln();
		}

		if(iAuswahl == 5)
		{
			iSchleife = 1;
		}


	}
	while(iSchleife != 1);

}


int Spieleregeln(void)
{
	system("cls");

	printf("Spieleregeln & Anleitung\n");
	printf("________________________\n\n\n");

	printf("%s", Regeln);

	system("pause");

}
int Registrieren(void)
{

	USER myUser;

	char cNickname[20];
	char cName[20];
	char cLastname[20];
	char cPassword[20];

	system("cls");

	printf("Registrierung\n");
	printf("*************\n\n\n");


	printf("Gewuenschter Spielername: ");
	scanf("%s", cNickname);
	printf("\n");
	myUser.sNickname =(char*)malloc(sizeof(char)*strlen(cNickname)+1);
	strcpy(myUser.sNickname, cNickname);

	printf("Vorname: ");
	scanf("%s", cName);
	printf("\n");
	myUser.sName =(char*)malloc(sizeof(char)*strlen(cName)+1);
	strcpy(myUser.sName, cName);

	printf("Nachname: ");
	scanf("%s", cLastname);
	printf("\n");
	myUser.sLastname =(char*)malloc(sizeof(char)*strlen(cLastname)+1);
	strcpy(myUser.sLastname, cLastname);

	printf("Gewuenschtes Password: ");
	scanf("%s", cPassword);
	printf("\n");
	myUser.sPassword =(char*)malloc(sizeof(char)*strlen(cPassword)+1);
	strcpy(myUser.sPassword, cPassword);

	register_user(&myUser);

	system("pause");
	return 0;
}
int Einloggen(void)
{

	char cNutzername[20];
	char cPassword[20];
	int iUserID;

	system("cls");
	printf("Anmelden\n");
	printf("________\n\n");

	printf("Nutzername: ");
	scanf("%s", cNutzername);
	printf("Password ein: ");
	scanf("%s", cPassword);

	iUserID = authenticate(cNutzername, cPassword);

	if(iUserID == 0)
	{
		printf("Ihr Nutzername oder ihr Password ist falsch\n\n");
		system("pause");
	}
	if(iUserID >= 1)
	{
		eingeloggt(cNutzername);
	}


}
int FreiesSpiel(void)
{
	int iAuswahlFS;
	int iError;
	do{
		system("cls");

		printf("Freies Spiel\n");
		printf("________________________\n\n\n");

		printf("1: Anfaenger\n"
			"2: Normal\n"
			"3: Profi\n"
			"4: zurueck\n\n");
		printf("Auswahl: ");
		iError = scanf("%i", &iAuswahlFS);
		fflush(stdin);
	}
	while(iAuswahlFS > 4 || iAuswahlFS < 1);

	if(iAuswahlFS == 1)
	{
	}

	if(iAuswahlFS == 2)
	{
	}

	if(iAuswahlFS == 3)
	{
	}

	if(iAuswahlFS == 4)
	{
	}
}
int GewertetesSpiel(void)
{
	int iAuswahlGS;
	int iError;
	do{
		system("cls");

		printf("Gewertetes Spiel\n");
		printf("________________________\n\n\n");

		printf("1: Anfaenger\n"
			   "2: Normal\n"
			   "3: Profi\n"
			   "4: zurueck\n\n");
		printf("Auswahl: ");
		iError = scanf("%i", &iAuswahlGS);
		fflush(stdin);
	}
	while(iAuswahlGS > 4 || iAuswahlGS < 1);

	if(iAuswahlGS == 1)
	{
	}

	if(iAuswahlGS == 2)
	{
	}

	if(iAuswahlGS == 3)
	{
	}

	if(iAuswahlGS == 4)
	{
	}
}
int eingeloggt(char cNutzername[20])
{
	int iAuswahl_e;
	do{
		do{
			system("cls");
			printf("Angemeldet als: %s\n", cNutzername);
			printf("_____________________\n\n");

			printf("1: Gewertetes Spiel\n"
				"2: Freies Spiel\n"
				"3: Globale Bestenliste\n"
				"4: Persoehnliche Bestenliste\n"
				"5: Ausloggen\n\n"); 
			printf("Auswahl: ");
			scanf("%i", &iAuswahl_e);
			fflush(stdin);

		}
		while(iAuswahl_e > 5 || iAuswahl_e < 1);

		if(iAuswahl_e == 1)
		{
			GewertetesSpiel();
		}

		if(iAuswahl_e == 2)
		{
			FreiesSpiel();
		}

		if(iAuswahl_e == 3)
		{
			
		}

		if(iAuswahl_e == 4)
		{
			
		}
	}
	while(iAuswahl_e != 5);
}