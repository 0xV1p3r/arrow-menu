#ifndef ARROW_MENU_H
#define ARROW_MENU_H

#include <stdint.h>

int arrow_menu(char *options[], uint8_t option_count, char *header, const char *help);
void wait_for_user();

#endif //ARROW_MENU_H
