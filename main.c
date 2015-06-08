#include "db.h"
#include "tableui.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
   int *UserIds;
   int nUsers;
   int i;

   nUsers = get_users(&UserIds);
   for(i = 0; i < nUsers; i++) {
      GAMERANKING games[3];
      USER* user = get_user_data(UserIds[i]);
      int iUserId = user->iUserId;
      int* GameList;
      int nGames;
      int j;

      nGames = get_best_user_games(1,iUserId,games,3);
      printf("Nummer: %d\n",i);
      printf("\tID:                %d\n",user->iUserId);
      printf("\tNick:              %s\n",user->sNickname);
      printf("\tLastname:          %s\n",user->sLastname);
      printf("\tFirstname:         %s\n",user->sName);
      printf("\tRegistration-Date: %s\n",user->sRegisterdate);
      printf("\tNumber of Games:   %d\n",nGames);

      for (j = 0; j < nGames; j++) {
         printf("\t\tNummer:        %d\n",j);
         printf("\t\tDauer:         %d\n",games[j].iSeconds);
         printf("\t\tSchwierigkeit: %d\n",games[j].iMode);
         printf("\t\tScore:         %d\n",games[j].iScore);
         printf("\t\tZeitpunkt:     %s\n",games[j].sGameDate);
//         delete_gameranking_data(&(games[j]),1);
      }
   
      delete_user_data(user,0);
   }
   printf("Gruppe B rockt das!\n");
   system("pause");
   return 0;
}