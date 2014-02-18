#include "InputUtils.h"

#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

bool InputUtils::hasInput() {
   struct timeval tv;
   fd_set fds;
   tv.tv_sec = 0;
   tv.tv_usec = 0;
   FD_ZERO(&fds);
   FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
   select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
   return FD_ISSET(STDIN_FILENO, &fds) > 0;
}

void InputUtils::setNonBlock(bool state) {
   struct termios ttystate;

   //get the terminal state
   tcgetattr(STDIN_FILENO, &ttystate);

   if (state)
   {
      //turn off canonical mode
      ttystate.c_lflag &= ~ICANON;
      //minimum of number input read.
      ttystate.c_cc[VMIN] = 1;
   }
   else if (state)
   {
      //turn on canonical mode
      ttystate.c_lflag |= ICANON;
   }
   //set the terminal attributes.
   tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}
