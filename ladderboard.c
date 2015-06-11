/*
========================================================================
Autor       : GruppeB (Kevin Schorn)
Firma       : HHBK
Dateiname   : ladderboard.c
Datum       : 08.06.15	
Beschreibung: Darstellung der gesamten sowie der Persönlichen 
: Bestenlisten
Version     : 1.0
Programmschnittstelle: 
========================================================================
*/

/*
========================================================================
Include Dateien
========================================================================
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include "ladderboard.h"
#include "menue.h"
/*
========================================================================
Preprozesse und Konstanten
========================================================================
*/
void DatenAnzeige(GAMERANKING Daten[], int iLength);
void PersonAnzeigen(GAMERANKING pDaten[], int iLength);
/*
========================================================================
Funktion Bestenliste()
========================================================================
*/
void Bestenliste()
{
	//Variablen-Definition
	GAMERANKING Daten [10];
	int iMode = 0;
	int iLength = 0; 
	int iError = 0;

	while (iMode < 1 || iMode > 3)
	{
		//Ausgabe welche Anzeige gewünscht ist
		system ("cls");
		printf("*************************************");
		printf("******************************************\n\n");
		printf("\t\t\t\t Bestenliste\n\n");
		printf("*************************************");
		printf("******************************************\n\n");
		printf ("Bitte W\204hlen Sie den Schwierigkeitsgrad aus\n");
		printf ("Geben Sie (1) f\201r Schwer ein.\n");
		printf ("Geben Sie (2) f\201r Mittel ein.\n");
		printf ("Geben Sie (3) f\201r Leicht ein.\n");
		printf ("Bitte geben Sie die Zahl f\201r die ");
		printf(" gew\201nschte Bestenliste ein: ");
		fflush (stdin);
		iMode = 0;
		//Abfrage der Eingabe
		iError = scanf ("%i", &iMode);
	}

		//Aufruf der get_best_games Funktion zur ermittlung der besten Spiele
		iLength = get_best_games(iMode, Daten, 10);

		//Auf der DatanAnzeigen Funktion mit übergabe des Daten Array´s 
		//und der Anzahl der gefundenen Einträge
		DatenAnzeige(Daten, iLength);
		system ("pause");
	
}
/*
========================================================================
Funktion DatenAnzeigen()
========================================================================
*/
void DatenAnzeige(GAMERANKING Daten[],int iLength)
{
	//Variablen-Definition
	int iZaehler = 0;

	//Löschen des Bildschirm und anzeige der Überschriften
	system("cls");
	printf("*************************************");
	printf("******************************************\n\n");
	printf("\t\t\t\tBestenliste\n\n");
	printf("*************************************");
	printf("******************************************\n\n");
	printf("Rang |\t\tName   | ben\224t. Hilfen | Punktzahl |\tDatum\n");
	printf("*************************************");
	printf("******************************************\n\n");

	if (iLength > 0)
	{
	//Schleichen zum Anzeigen der Besten Spiele 
	while (iZaehler < iLength)
	{
		printf("%3i", iZaehler+1);
		printf("%20s ", Daten[iZaehler].sUserName);
		printf("\t%3i\t  ", Daten[iZaehler].iHelps);
		printf("%5i\t\t", Daten[iZaehler].iScore);
		printf("%s", Daten[iZaehler].sGameDate);
		printf("\n");
		printf("*************************************");
		printf("******************************************\n\n");
		iZaehler++;
	} 
	printf("\n");
	}
	else
	{
		//Ausgabe einer Fehlermeldung bei nicht vorhanden Daten
		printf ("es sind noch keine Daten Vorhanden\n\n");
	}
}
/*
========================================================================
Funktion PersListe()
========================================================================
*/
void PersListe()
{
	//Variablen-Definition
	GAMERANKING pDaten [9];
	int iZaehler = 0;
	int iMode = 0;
	int iLength; 
	int iError = 0;


	while (iMode < 1 || iMode > 3)
	{
	//Ausgabe welche Anzeige gewünscht
	system ("cls");
	printf("*************************************");
	printf("******************************************\n\n");
	printf ("\t\t\t\tPers\224nliche Bestenliste\n\n");
	printf("*************************************");
	printf("******************************************\n\n");
	printf ("Bitte W\204hlen Sie den Schwierigkeitsgrad aus\n");
	printf ("Geben Sie (1) f\201r Schwer ein.\n");
	printf ("Geben Sie (2) f\201r Mittel ein.\n");
	printf ("Geben Sie (3) f\201r Leicht ein.\n");
	printf ("Bitte geben Sie die Zahl f\201r die ");
	printf(" gew\201nschte Bestenliste ein: ");
	fflush (stdin);
	//Abfrage der Eingabe
	iMode = 0; 
	iError = scanf ("%i", &iMode);
	}


	//Aufruf der get_best_user_games Funktion zur ermittlung der besten Spiele
	//anhand des gewählten Schwierigkeitgrads
	iLength = get_best_user_games(iMode,iGUserID,pDaten, 9);

	//Auf der PersonAnzeigen Funktion mit übergabe des pDaten Array´s 
	//und der Anzahl der gefundenen Einträge
	PersonAnzeigen(pDaten, iLength);
	system ("pause");
}
/*
========================================================================
Funktion PersonAnzeigen()
========================================================================
*/
void PersonAnzeigen(GAMERANKING pDaten[],int iLength)
{
	//Variablen-Definition
	int iZaehler = 0;

	//Löschen des Bildschirms und anzeige der Überschriften
	system("cls");
	printf("*************************************");
	printf("******************************************\n\n");
	printf("\t\t\t\t Bestenliste\n\n");
	printf("*************************************");
	printf("******************************************\n\n");
	printf("Rang | ben\224t. Hilfen | Punktzahl |\tDatum\n");
	printf("*************************************");
	printf("******************************************\n\n");

	//Schleifen zum Anzeigen der Besten Spiele 
	//anhand des Schwierigkeitsgrad
	if (iLength > 0)
	{
		while (iZaehler < iLength || iZaehler == 5)
		{
		printf("%3i", iZaehler+1);
		printf("%20s ", pDaten[iZaehler].sUserName);
		printf("\t%3i\t  ", pDaten[iZaehler].iHelps);
		printf("%5i\t\t", pDaten[iZaehler].iScore);
		printf("%s", pDaten[iZaehler].sGameDate);
		printf("\n");
		printf("*************************************");
		printf("******************************************\n\n");
			iZaehler++;
		}
		printf("\n");
	}
	else
	{
		//Ausgabe einer Fehlermeldung bei nicht vorhanden Daten
		printf ("es sind noch keine Daten Vorhanden\n\n");
	}

}
