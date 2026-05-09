#include "include.h"

void clear_screen () {
  #ifdef _WIN64
    system("cls");
  #else
    system("clear");
  #endif
}

bool valid_move () {
  return false;
}