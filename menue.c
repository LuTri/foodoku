/*
========================================================================
Autor       : GruppeB (Lennard Malessa)
Firma       : HHBK
Dateiname   : men\201.c
Datum       : 08.06.2015
Beschreibung: Nutzerführung
Compiler    : Visual Studio
Version     : 1.0
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

#include <stdlib.h>
#include "db.h"
#include "menue.h"
#include <stdio.h>
#include <string.h>
#include "ladderboard.h"

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
		system("cls");
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
Anzeigen der Spielregeln für das Sodokuspiel
==================================
*/
void Spieleregeln(void)
{
	system("cls");

	printf("*************************************");
	printf("******************************************\n\n");
	printf("\t\t\tSpieleregeln & Anleitung\n\n");
	printf("*************************************");
	printf("******************************************\n\n");
	//Anzeige der Sodokuregeln aus #define REGELN
	printf(REGELN);

   

	system("pause");

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
	char cPassword[20];

	system("cls");

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
	scanf("%s", cPassword);
	printf("\n");
	myUser.sPassword =(char*)malloc(sizeof(char)*strlen(cPassword)+1);
	}
	}
	}

	if (strcmp(cNickname, "abbruch") != 0 && strcmp(cName, "abbruch") != 0 && 
		strcmp(cLastname, "abbruch") != 0 && strcmp(cPassword, "abbruch") != 0)
	{
	printf("Registrierung abgeschlossen\n");
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

	

	system("pause");
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
	int iUserID;

	system("cls");
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
	scanf("%s", cPassword);

	//Aufrufen der Funktion authenticate und übergabe des Rückgabewertes
	//an den Speicher iUserID
	iUserID = authenticate(cNutzername, cPassword);

	//Abfragen der UserID und Aufrufen der Funktion eingeloggt
	if(iUserID == 0)
	{
		printf("\nIhr Nutzername oder ihr Password ist falsch\n\n");
		system("pause");
	}
	if(iUserID >= 1)
	{
		eingeloggt(iUserID);
	}


}
/*
==================================
Starten eines ungewertete Spieles und auswählen der Schwierigkeit
1 Parameter: iAuswahlFS(Auswahl der Schwierigkeitsstufe)
==================================
*/
void FreiesSpiel(void)
{
	//Speicherdeklaration
	int iAuswahlFS;

	//Schleife solange Auswahl > 4 oder < 1
	do{
		system("cls");

		//Ausgabe der Auswahl und Eingabe der Auswahl
		printf("*************************************");
		printf("******************************************\n\n");
		printf("\t\t\t\tFreies Spiel\n\n");
		printf("*************************************");
		printf("******************************************\n\n");

		printf("1: Anf\204nger\n"
			   "2: Normal\n"
			   "3: Profi\n"
			   "4: zur\201ck\n\n");
		printf("Auswahl: ");
		scanf("%i", &iAuswahlFS);
		fflush(stdin);
	}
	while(iAuswahlFS > 4 || iAuswahlFS < 1);

	//Aufrufen des Sodokus je nach Schwierigkeitsstufe
	if(iAuswahlFS == 1)
	{
		//randomfunktion(iGUserID, iAuswahlFS, 0);
	}

	if(iAuswahlFS == 2)
	{
		//randomfunktion(iGUserID, iAuswahlFS, 0);
	}

	if(iAuswahlFS == 3)
	{
		//randomfunktion(iGUserID, iAuswahlFS, 0,);
	}

	if(iAuswahlFS == 4)
	{
	}
}
/*
==================================
Starten eines Gewerteten Spieles und auswählen der Schwierigkeit
1 Parameter: iAuswahlGS(Auswahl der Schwierigkeitsstufe)
==================================
*/
void GewertetesSpiel(void)
{
	//Speicherdeklaration
	int iAuswahlGS;
	

	//Schleife solange Auswahl > 4 oder < 1
	do{
		system("cls");

		//Ausgabe der Auswahl und Eingabe der Auswahl
		printf("*************************************");
		printf("******************************************\n\n");
		printf("\t\t\t\tGewertetes Spiel\n\n");
		printf("*************************************");
		printf("******************************************\n\n");

		printf("1: Anf\204nger\n"
			   "2: Normal\n"
			   "3: Profi\n"
			   "4: zur\201ck\n\n");
		printf("Auswahl: ");
		scanf("%i", &iAuswahlGS);
		fflush(stdin);
	}
	while(iAuswahlGS > 4 || iAuswahlGS < 1);

	//Aufrufen des Sodokus je nach Schwierigkeitsstufe
	if(iAuswahlGS == 1)
	{
		game_loop(0,1,iGUserID);
	}

	if(iAuswahlGS == 2)
	{
		game_loop(1,1,iGUserID);
	}

	if(iAuswahlGS == 3)
	{
		game_loop(2,1,iGUserID);
	}
}
/*
==================================
Menüanzeige für eingeloggte Spieler
1 Parameter: iAuswahl_e(Auswahl der einzelnen Menüs)
==================================
*/
void eingeloggt(int iUserID)
{
	//Speicherdeklaration
	int iAuswahl_e;
	USER* myuser;
	int iGUserID = iUserID;

	//Schleife, solange Auswahl ungleich 5 bleib in der Funktion
	do{

		//Holen der Userinformationen anhand der übergeben UserID
		myuser = get_user_data(iUserID);

		//Schleife für die Auswahl
		do{

			system("cls");
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
