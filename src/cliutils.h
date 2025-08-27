#ifndef CLI_UTILS_H
#define CLI_UTILS_H

#include "kernel/global.h"
#include "kernel/drivers/display/display.h"
#include "kernel/drivers/keyboard/keyboard.h"

struct screen{
    int rows;
    int cols;
    int x;
    int y;
    Color fg_color;
    Color bg_color;
    Color default_fg_color;
    Color default_bg_color;
};
extern struct screen scr;

void cli_printf(const char *fmt, ...);
void cli_colored_printf(Color color, const char *fmt, ...);
void cli_warning(const char *fmt, ...);
void cli_error(const char *fmt, ...);

// void cli_scanf(char *buffer);

void cli_clear_current_char();

#endif
