#include "cliutils.h"
#include "utils/template/t_vprintf.h"
#include <stdarg.h>


static void set_color(Color color) {
    scr.fg_color = color;
}





void cli_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    t_vprintf(fmt, args);
    va_end(args);
    return;
}

void cli_colored_printf(Color color, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    set_color(color);
    t_vprintf(fmt, args);
    va_end(args);
    set_color(scr.default_fg_color);
}

void cli_warning(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    set_color(get_color(COLOR_YELLOW));
    t_vprintf(fmt, args);
    va_end(args);
    set_color(scr.default_fg_color);
}

void cli_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    set_color(get_color(COLOR_RED));
    t_vprintf(fmt, args);
    va_end(args);
    set_color(scr.default_fg_color);
}



void cli_clear_current_char(){
    _sys_fill_rect(scr.x * FONT_WIDTH, scr.y * FONT_HEIGHT, FONT_WIDTH, FONT_HEIGHT, hex_color(scr.bg_color));
}



