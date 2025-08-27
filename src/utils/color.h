#ifndef COLOR_H
#define COLOR_H

typedef struct {
    unsigned char r, g, b;
} Color;

enum ColorName {
    COLOR_BLACK,
    COLOR_WHITE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_LIGHT_BLUE,
    COLOR_YELLOW,
    COLOR_CYAN,
    COLOR_MAGENTA
};

Color get_color(enum ColorName color_name);
Color create_color(int r, int g, int b);
int hex_color(Color color);

#endif 