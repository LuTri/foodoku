/*
========================================================================
   Autor          : GruppeB (Lennard Malessa)
   Firma          : HHBK
   Dateiname      : menue.c
   Datum          : 08.06.2015
   Beschreibung   : Nutzerführung
   Compiler       : Visual Studio
   Version        : 1.0
   Programmschnittstelle: int main(void)
   Das Programm gibt den Wert 0 oder 1 zurück.
   Das Programm erwartet beim Aufruf keine Argumente
========================================================================
*/
#define _CRT_SECURE_NO_DEPRECATE 1
/* Microsoft hat alle gängigen Ein- und Ausgabefunktionen
als gefährlich eingestuft und durch neue Varianten ersetzt.
Mit den neuen Funktionen sind die Programme aber nicht
kompatible zu anderen Entwicklungsumgebungen.
Bei Verwendung der alten Funktionen wird eine Warnung ausgegeben.
Diese Warnung kann durch die obige Definition unterdrückt werden.
*/

#include "os.h"
#include "db.h"
#include "menue.h"
#include <stdio.h>
#include <string.h>
#include "ladderboard.h"
#include "loop.h"

int iGUserID;
/*
==================================
   Auswahl der einzelnen Menüpunkte
   1. Paramter : iAuswahl(Auswahl der einzelnen Menüs)
==================================
*/
void menue (void)
{
	//Speicherdeklaration
	int iAuswahl = 0;
	int iError;
	int iSchleife = 0;
	//Schleife zur Abfrage der Auswahl(Eingabe zwischen 1 und 5)
	do
	{
		do
		{
			CLEAR_SCREEN
			printf("*************************************");
			printf("******************************************\n\n");
			printf("\t\t\t\tSUDOKU TASKFORCE\n\n");
			printf("*************************************");
			printf("******************************************\n\n");
			//Auswahl der einzelnen Menüs und Eingabe der Auswahl
			printf("1: Einloggen\n");
			printf("2: Registrieren\n");
			printf("3: Freies Spiel\n");
			printf("4: Spieleanleitung\n");
			printf("5: Beenden\n\n");
			printf("Auswahl: ");
			fflush(stdin);
			iError = scanf("%i", &iAuswahl);
		}
		while(iError == 0 || iAuswahl < 1 || iAuswahl > 5);
		//Aufruf der einzelnen Funktionen je nach getroffener Auswahl
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
/*
==================================
   Starten eines Spieles(gewertet/ungewertet je nach Übergae
   des Wertes 1 oder 0.
   1 Parameter: iAuswahlSpiel(Auswahl der Schwierigkeit oder SChritt ins
   vorherige Menü
==================================
*/
int iSpieleauswahl()
{
	//Speicherdeklaration
	int iAuswahlSpiel;
	printf("1: Anf\204nger\n"
		"2: Normal\n"
		"3: Profi\n"
		"4: zur\201ck\n\n");
	printf("Auswahl: ");
	scanf("%i", &iAuswahlSpiel);
	fflush(stdin);
	return iAuswahlSpiel;
}
/*
==================================
   Anzeigen der Spielregeln für das Sodokuspiel
==================================
*/
void Spieleregeln(void)
{
	CLEAR_SCREEN
	printf("*************************************");
	printf("******************************************\n\n");
	printf("\t\t\tSpieleregeln & Anleitung\n\n");
	printf("*************************************");
	printf("******************************************\n\n");
	//Anzeige der Sodokuregeln aus #define REGELN
	printf(REGELN);
	SYSTEM_PAUSE
}
/*
==================================
   Registrierung eines neuen Nutzers
   1 Parameter: cNickname(Der Accountname des Spielers)
   2 Parameter: cName(Der Vorname des Spielers)
   3 Parameter: cLastname(der Nachname des Spielers)
   4 Parameter: cPassword(Das Password des Spielers)
==================================
*/
void Registrieren(void)
{
	//Speicherdeklaration
	USER myUser;
	//Speicher für die Registrierungsangaben
	char cNickname[20];
	char cName[20];
	char cLastname[20];
	char cPassword[200];
	CLEAR_SCREEN
	printf("*************************************");
	printf("******************************************\n\n");
	printf("\t\t\t\tRegistrierung\n\n");
	printf("*************************************");
	printf("******************************************\n\n");
	//Eingabe des Spielernamens
	printf("Gew\201nschter Spielername: ");
	scanf("%s", cNickname);
	printf("\n");
	myUser.sNickname =(char*)malloc(sizeof(char)*strlen(cNickname)+1);
	if (strcmp(cNickname, "abbruch") != 0)
	{
		//Eingabe des Vornamens
		printf("Vorname: ");
		scanf("%s", cName);
		printf("\n");
		myUser.sName =(char*)malloc(sizeof(char)*strlen(cName)+1);
		if (strcmp(cName, "abbruch") != 0)
		{
			//Eingabe des Nachnamens
			printf("Nachname: ");
			scanf("%s", cLastname);
			printf("\n");
			myUser.sLastname =(char*)malloc(sizeof(char)*strlen(cLastname)+1);
			if (strcmp(cLastname, "abbruch") != 0)
			{
				//Eingabe des Passwortes
				printf("Gew\201nschtes Password: ");
				passwd_input(cPassword);
				printf("\n");
				myUser.sPassword =(char*)malloc(sizeof(char)*strlen(cPassword)+1);
			}
		}
	}
	if (strcmp(cNickname, "abbruch") != 0 && strcmp(cName, "abbruch") != 0 &&
		strcmp(cLastname, "abbruch") != 0 && strcmp(cPassword, "abbruch") != 0)
	{
		printf("Registrierung abgeschlossen\n\n");
		strcpy(myUser.sNickname, cNickname);
		strcpy(myUser.sName, cName);
		strcpy(myUser.sLastname, cLastname);
		strcpy(myUser.sPassword, cPassword);
		//Übergabe der Werte an die Funktion register_user
		register_user(&myUser);
	}
	/*
	Wenn "abbruch" bei Name, Nachname, Nickname oder Password eingegeben wird,
	wird die Registrierung automatisch abgebrochen
	*/
	if (strcmp(cNickname, "abbruch") == 0 || strcmp(cName, "abbruch") == 0 ||
		strcmp(cLastname, "abbruch") == 0 || strcmp(cPassword, "abbruch") == 0)
	{
		printf("Registrierung abgebrochen!\n\n");
	}
	SYSTEM_PAUSE
}
/*
==================================
   Einloggen eines vorhandenen Nutzers
   1 Parameter: cNutzername(Der Nutzername des Spielers)
   2 Parameter: cPassword(Das Password des Nutzers)
==================================
*/
void Einloggen(void)
{
	//Speicherdeklaration
	char cNutzername[20];
	char cPassword[20];
	CLEAR_SCREEN
	printf("*************************************");
	printf("******************************************\n\n");
	printf("\t\t\t\tAnmelden\n\n");
	printf("*************************************");
	printf("******************************************\n\n");
	//Eingabe des Nutzernamens
	printf("Nutzername: ");
	scanf("%s", cNutzername);
	//Eingabe des Passwortes
	printf("Password ein: ");
	passwd_input(cPassword);
	//Aufrufen der Funktion authenticate und übergabe des Rückgabewertes
	//an den Speicher iUserID
	iGUserID = authenticate(cNutzername, cPassword);
	//Abfragen der UserID und Aufrufen der Funktion eingeloggt
	if(iGUserID == 0)
	{
		printf("\nIhr Nutzername oder ihr Password ist falsch\n\n");
		SYSTEM_PAUSE
	}
	if(iGUserID >= 1)
	{
		eingeloggt();
	}
}
/*
==================================
   Starten eines ungewertete Spieles und auswählen der Schwierigkeit
   1 Parameter: iAuswahl(Uberprüfung der Eingabe aus iSPieleauswahl()
==================================
*/
void FreiesSpiel(void)
{
	//Speicherdeklaration
	int iAuswahl;
	int iSpieltyp = 0;
	do
	{
		CLEAR_SCREEN
		//Ausgabe der Auswahl und Eingabe der Auswahl
		printf("*************************************");
		printf("******************************************\n\n");
		printf("\t\t\t\tFreies Spiel\n\n");
		printf("*************************************");
		printf("******************************************\n\n");
		iAuswahl = iSpieleauswahl();
	}
	while(iAuswahl > 4 || iAuswahl < 1);
	
	//Aufrufen des Sodokus je nach Schwierigkeitsstufe
	if (iAuswahl != 4) {
		game_loop(iAuswahl,0,iGUserID);

	}
}
/*
==================================
   Starten eines Gewerteten Spieles und auswählen der Schwierigkeit
   1 Parameter: iAuswahl(Uberprüfung der Eingabe aus iSPieleauswahl()
==================================
*/
void GewertetesSpiel(void)
{
	//Speicherdeklaration
	int iAuswahl;
	int iSpieltyp = 1;
	//Schleife solange Auswahl > 4 oder < 1
	do{
		CLEAR_SCREEN
		//Ausgabe der Auswahl und Eingabe der Auswahl
		printf("*************************************");
		printf("******************************************\n\n");
		printf("\t\t\t\tGewertetes Spiel\n\n");
		printf("*************************************");
		printf("******************************************\n\n");
		iAuswahl = iSpieleauswahl();
	}
	while(iAuswahl > 4 || iAuswahl < 1);

	//Aufrufen des Sodokus je nach Schwierigkeitsstufe
	if (iAuswahl != 4) {
		game_loop(iAuswahl,1,iGUserID);

	}
}
/*
==================================
   Menüanzeige für eingeloggte Spieler
   1 Parameter: iAuswahl_e(Auswahl der einzelnen Menüs)
==================================
*/
void eingeloggt(void)
{
	//Speicherdeklaration
	int iAuswahl_e;
	USER* myuser;
	//Schleife, solange Auswahl ungleich 5 bleib in der Funktion
	do{
		//Holen der Userinformationen anhand der übergeben UserID
		myuser = get_user_data(iGUserID);
		//Schleife für die Auswahl
		do{
			CLEAR_SCREEN
			printf("*************************************");
			printf("******************************************\n\n");
			printf("Angemeldet als: %s\t\tHauptmen\201\n\n", myuser->sNickname);
			printf("*************************************");
			printf("******************************************\n\n");
			//Anzeigen und Eingabe der Auswahl
			printf("1: Gewertetes Spiel\n"
				"2: Freies Spiel\n"
				"3: Globale Bestenliste\n"
				"4: Pers\224hnliche Bestenliste\n"
				"5: Ausloggen\n\n");
			printf("Auswahl: ");
			scanf("%i", &iAuswahl_e);
			fflush(stdin);
		}
		while(iAuswahl_e > 5 || iAuswahl_e < 1);
		//Aufrufen der Funktionen je nach getroffener Auswahl
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
			Bestenliste();
		}
		if(iAuswahl_e == 4)
		{
			PersListe();
		}
		if(iAuswahl_e == 5)
		{
			//Löschen der Userinformationen
			delete_user_data(myuser, 1);
		}
	}
	while(iAuswahl_e != 5);
}