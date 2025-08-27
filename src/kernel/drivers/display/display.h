#ifndef DISPLAY_H
#define DISPLAY_H

#include "../../global.h"
#include "font.h"

extern struct ScreenInfo{
    uint32_t* framebuffer;
    int width;
    int height;
    int pitch;
} _SYS_ScreenInfo;
typedef struct ScreenInfo ScreenInfo;


void _sys_init_display(multiboot_tag* tag);

void _sys_clear_screen();
void _sys_put_pixel  ( int x, int y, uint32_t color );

void _sys_draw_line   ( int x1, int y1, int x2, int y2, uint32_t color );
void _sys_draw_rect   ( int x, int y, int width, int height, uint32_t color );
void _sys_fill_rect   ( int x, int y, int width, int height, uint32_t color );
void _sys_draw_circle ( int x, int y, int radius, uint32_t color );
void _sys_fill_circle ( int x, int y, int radius, uint32_t color );

void _sys_put_char   ( int x, int y, uint32_t color, char c );
void _sys_put_string ( int x, int y, uint32_t color, const char* str );



#endif 