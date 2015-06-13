#include "os.h"

#ifdef OS_LINUX
void pause(void) {
   printf("Press any key to continue...\n");
   getchar();
}
#endif