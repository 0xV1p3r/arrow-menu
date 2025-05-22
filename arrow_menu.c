#include "arrow_menu.h"

#if _WIN32
    #include <conio.h>
#else
    #include <stdlib.h>
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define ANSI_COLOR_CYAN  "\033[0;36m"
#define ANSI_COLOR_RESET   "\033[0m"

#define ANSI_ERASE_ENTIRE_SCREEN "\x1b[2J"
#define ANSI_ERASE_IN_DISPLAY "\x1b[J"
#define ANSI_CURSOR_HOME "\x1b[H"

#if _WIN32
    #define CLEAR_SCREEN system("cls");
#else
    #define CLEAR_SCREEN system("clear");
#endif

#define ERASE_DISPLAY printf("%s%s", ANSI_ERASE_IN_DISPLAY, ANSI_CURSOR_HOME);

#define ENTER_KEY 13

#if _WIN32
    #define UP_ARROW_KEY 72
    #define DOWN_ARROW_KEY 80
    #define LEFT_ARROW_KEY 75
    #define RIGHT_ARROW_KEY 77
#else
    #define UP_ARROW_KEY 65
    #define DOWN_ARROW_KEY 66
    #define LEFT_ARROW_KEY 68
    #define RIGHT_ARROW_KEY 67
#endif

int get_ch();
void print_menu(char*[], uint8_t, uint8_t, char*);

int arrow_menu(char *options[], const uint8_t option_count, char *header, const char *help) {
    int pressed_key = 0;
    int position = 0;
    bool refresh_screen = true;

    CLEAR_SCREEN
    while (pressed_key != ENTER_KEY
        && pressed_key != 'q'
        && pressed_key != LEFT_ARROW_KEY
        && pressed_key != RIGHT_ARROW_KEY
        ) {

        if (position >= option_count) position = 0;
        if (position < 0) position = option_count - 1;

        if (refresh_screen) print_menu(options, option_count, position, header);

        pressed_key = get_ch();

        if (pressed_key == DOWN_ARROW_KEY) {
            position++;
            refresh_screen = true;
        } else if (pressed_key == UP_ARROW_KEY) {
            position--;
            refresh_screen = true;
        } else if (pressed_key >= '1' && pressed_key <= '9') {
            const int pos_delta = pressed_key - '1';
            if (position + pos_delta < option_count) {
                return position + pos_delta;
            }
            refresh_screen = false;
        } else if (pressed_key == 'h') {
            CLEAR_SCREEN
            if (help != NULL) {
                printf("%s", help);
            } else {
                printf("No help available!\n");
            }
            wait_for_user();
            CLEAR_SCREEN
            refresh_screen = true;
        } else {
            refresh_screen = false;
        }
    }
    if (pressed_key == 'q' || pressed_key == LEFT_ARROW_KEY) return -1;
    return position;
}

int get_ch() {
#if _WIN32
    return getch;
#else
    system("stty raw -echo");
    int c = getchar();
    if (c == 27) {
        getchar();
        c = getchar();
    }
    system("stty -raw echo");
    return c;
#endif
}

void print_menu(char *options[], const uint8_t option_count, const uint8_t position, char *header) {

    ERASE_DISPLAY

    if (header != NULL) {
        printf("%s", header);
    }

    printf("\n");
    for (uint8_t i = 0; i < option_count; i++) {

        if (i == position) {
            printf("%s > %s%s\n", ANSI_COLOR_CYAN, options[i], ANSI_COLOR_RESET);
        } else {
            printf("   %s\n", options[i]);
        }
    }
    printf("\n");
}

void wait_for_user() {
    printf("\nPress any key to continue...");
    get_ch();
}
