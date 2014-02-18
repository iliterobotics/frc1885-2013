#include "InputUtils.h"

#include <stdio.h>
#include <unistd.h>

#include <locale>

int main() {
   bool exit = false;

   InputUtils::setNonBlock(true);
   printf("press 'q' to exit.\n");

   while(!exit) {
      if (InputUtils::hasInput()) {
         char c=fgetc(stdin);
         if (c=='q') {
            exit = true;
         } else if(!isalnum(c)) {
            fprintf(stderr, "%c <%i>\n", c, c);
         }
         printf("Got Input!\n");
      }
      usleep(1);
   }

   printf("\n");
   InputUtils::setNonBlock(false);

   return 0;
}
