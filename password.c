#include "password.h"
#include <conio.h>

void passwd_input(char cBuff[200])
{
   char cCharacter;
   int i=0;
   cCharacter = getch();
   putch('*');
   while (cCharacter != '\n' && cCharacter != '\r' && i < 200)
   {
      cBuff[i++] = cCharacter;
      cCharacter = getch();
      putch('*');
   }
   cBuff[i] = '\0';
}
