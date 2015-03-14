#include "db.h"
#include "sqlite3.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* encrypt(char* clear)
{
   char buf[3] = {0,0,0};
   int iClearLen;
   int iSecretLen;
   int enc_idx = 0;
   int str_idx;
   char* tmp;
   char* iRet;

   iRet = (char*)calloc(1,sizeof(char));
   
   iSecretLen = strlen(ENCRYPTION_SECRET);
   iClearLen = strlen(clear);

   for (str_idx = 0; str_idx < iClearLen; str_idx++)
   {
      clear[str_idx] = ((clear[str_idx] + ENCRYPTION_SECRET[enc_idx++]) % 255);
      if (enc_idx >= iSecretLen)
      {
         enc_idx = 0;
      }
      sprintf(buf,"%x",(unsigned char)clear[str_idx]);
      tmp = (char*) calloc(strlen(iRet) + strlen(buf) + 1, sizeof(char));
      strcpy(tmp,iRet);
      strcpy(tmp + strlen(tmp), buf);
      free(iRet);
      iRet = tmp;
   }

   return iRet;
   }

sqlite3* get_db(void)
{
   static sqlite3 *db_handle = NULL;
   int rc;
   if (db_handle == NULL)
   {
      rc = sqlite3_open(DATABASE_FILE, &db_handle);
      if (rc != SQLITE_OK)
      {
         sqlite3_close(db_handle);
         db_handle = NULL;
      }
   }
   return db_handle;
}

int authenticate(char* nickname, char* password)
{
   int rc;
   int user_id;
   char* sql;
   char* zErrMsg;
   sqlite3_stmt *stmt;
   char* encrypted_password = encrypt(password);

   sql = sqlite3_mprintf("SELECT user_id FROM userdata WHERE spielername='%s' "\
                         "AND passwort='%s';", nickname, encrypted_password);

   rc = sqlite3_prepare_v2(get_db(),sql,strlen(sql),&stmt,&zErrMsg);
   free(sql);
   if (rc != SQLITE_OK)
   {
      printf("Ein Fehler ist aufgetreten: \"%s\"\n", zErrMsg);
      free(zErrMsg);
      return -1;
   }

   if (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      user_id = atoi((const char*)sqlite3_column_text(stmt,0));
   }
   else
   {
      return -1;
   }

   return user_id;
}

char get_userdata(int user_id,
                  char** nickname,
                  char** name,
                  char** lastname,
                  char**date)
{
   int rc;
   char* sql;
   char* zErrMsg;
   sqlite3_stmt *stmt;
   
   sql = sqlite3_mprintf("SELECT spielername, vorname, nachname, anmeldedatum "\
                         "FROM userdata WHERE user_id = %d;", user_id); 
   rc = sqlite3_prepare_v2(get_db(),sql,strlen(sql),&stmt,&zErrMsg);
   free(sql);

   if (rc != SQLITE_OK)
   {
      printf("Ein Fehler ist aufgetreten: \"%s\"\n", zErrMsg);
      free(zErrMsg);
      return 0;
   }

   if (sqlite3_step( stmt ) == SQLITE_ROW)
   {
      *nickname = (char*)sqlite3_column_text(stmt,0);
      *name = (char*)sqlite3_column_text(stmt,1);
      *lastname= (char*)sqlite3_column_text(stmt,2);
      *date = (char*)sqlite3_column_text(stmt,3);
      return 1;
   }
   else
   {
      return 0;
   }
}

char register_user(char* nickname, char* name, char* lastname, char* password)
{
   char* sql;
   int rc;
   char* encrypted_password;
   char* zErrMsg;

   encrypted_password = encrypt(password);
   
   sql = sqlite3_mprintf("INSERT INTO userdata(passwort,spielername,vorname,na"\
      "chname,anmeldedatum) VALUES('%s','%s','%s','%s',date('now'));",
      encrypted_password,nickname,name,lastname);

   free(encrypted_password);
   rc = sqlite3_exec(get_db(),sql,NULL,NULL,&zErrMsg);
   free(sql);

   if (rc != SQLITE_OK)
   {
      printf("Ein Fehler ist aufgetreten: \"%s\"\n", zErrMsg);
      free(zErrMsg);
      return 0;
   }
   return 1;
}

char insert_game_data(int user_id,
                      int seconds,
                      int helps,
                      int filled,
                      char finished,
                      char ranked)
{
   char* sql;
   int rc;
   char* zErrMsg;

   sql = sqlite3_mprintf("INSERT INTO games(user, spielzeitpunkt, zeit, "\
      "angezeigte_hilfen, gefuellte_felder, abgeschlossen, gewertet) VALUES(%d,"\
      "datetime('now','localtime'),%d,%d,%d,%d,%d);",
      user_id, seconds,helps,filled,finished,ranked);

   rc = sqlite3_exec(get_db(),sql,NULL,NULL,&zErrMsg);
   free(sql);
   if (rc != SQLITE_OK)
   {
      printf("Ein Fehler ist aufgetreten: \"%s\"\n", zErrMsg);
      free(zErrMsg);
      return 0;
   }
   return 1;
}