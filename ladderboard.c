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
void DatenAnzeige(GAMERANKING Daten[]);
void PersonAnzeigen(GAMERANKING pDaten[]);
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
	int iLenght; 
	int iError = 0;

	//Ausgabe welche Anzeige gewünscht
	system ("cls");
	printf ("\t\tBestenliste\n\n");
	printf ("Bitte W\204hlen Sie den Schwierigkeitsgrad aus\n");
	printf ("Geben Sie (1) f\201r Schwer ein.\n");
	printf ("Geben Sie (2) f\201r Mittel ein.\n");
	printf ("Geben Sie (3) f\201r Leicht ein.\n");
	printf ("Bitte geben Sie die Zahl f\201r die ");
	printf(" gew\201nschte Bestenliste ein: ");
	fflush (stdin);
	//Abfrage der Eingabe
	iError = scanf ("%i", &iMode);
	if (iMode < 1 || iMode > 3 ||iError != 1)
	{
		system ("cls");

		Bestenliste();
	}
	//Aufruf der get_best_games Funktion zur ermittlung der besten Spiele
	iLenght = get_best_games(iMode, Daten, 10);

	//Auf der DatanAnzeigen Funktion mit übergabe des Daten Array´s 
	//und der Anzahl der gefundenen Einträge
	DatenAnzeige(Daten, iLenght);
	system ("pause");

}
/*
========================================================================
Funktion DatenAnzeigen()
========================================================================
*/
void DatenAnzeige(GAMERANKING Daten[],int iLenght)
{
	//Variablen-Definition
	int iZaehler = 0;

	//Löschen des Bildschirm und anzeige der Überschriften
	system("cls");
	printf("*************************************");
	printf("******************************************\n\n");
	printf("\t\t\t\t Bestenliste\n\n");
	printf("*************************************");
	printf("******************************************\n\n");
	printf("Rang |\t\tName   | ben\224t. Hilfen | Punktzahl |\tDatum\n");
	printf("*************************************");
	printf("******************************************\n\n");

	//Schleichen zum Anzeigen der Besten Spiele 
	do
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
	} while (iZaehler < iLenght);
	printf("\n");

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
	int iLenght; 
	int iError = 0;

	//Ausgabe welche Anzeige gewünscht
	system ("cls");
	printf ("\t Pers\224nliche Bestenliste\n\n");
	printf ("Bitte W\204hlen Sie den Schwierigkeitsgrad aus\n");
	printf ("Geben Sie (1) f\201r Schwer ein.\n");
	printf ("Geben Sie (2) f\201r Mittel ein.\n");
	printf ("Geben Sie (3) f\201r Leicht ein.\n");
	printf ("Bitte geben Sie die Zahl f\201r die ");
	printf(" gew\201nschte Bestenliste ein: ");
	fflush (stdin);
	//Abfrage der Eingabe
	iError = scanf ("%i", &iMode);
	if (iMode < 1 || iMode > 3 ||iError != 1)
	{
		system ("cls");

		PersListe();
	}
	//Aufruf der get_best_user_games Funktion zur ermittlung der besten Spiele
	//anhand des gewählten Schwierigkeitgrads
	iLenght = get_best_user_games(iMode,iGUserID,pDaten, 9);

	//Auf der PersonAnzeigen Funktion mit übergabe des pDaten Array´s 
	//und der Anzahl der gefundenen Einträge
	PersonAnzeigen(pDaten, iLenght);
	system ("pause");
}
/*
========================================================================
Funktion PersonAnzeigen()
========================================================================
*/
void PersonAnzeigen(GAMERANKING pDaten[],int iLenght)
{
	//Variablen-Definition
	int iZaehler = 0;

	//Löschen des Bildschirm und anzeige der Überschriften
	system("cls");
	printf("*************************************");
	printf("******************************************\n\n");
	printf("\t\t\t\t Bestenliste\n\n");
	printf("*************************************");
	printf("******************************************\n\n");
	printf("Rang | ben\224t. Hilfen | Punktzahl |\tDatum\n");
	printf("*************************************");
	printf("******************************************\n\n");

	//Schleichen zum Anzeigen der Besten Spiele 
	//anhand des Schwierigkeitsgrad
	if (iLenght > 0)
	{
		do
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
		while (iZaehler < iLenght || iZaehler == 5);
		printf("\n");
	}
	else
	{
		printf ("es sind noch keine Daten Vorhanden\n\n");
	}

}