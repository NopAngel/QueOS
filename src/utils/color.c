#include "color.h"

static struct color_code{
    enum ColorName name;
    Color color;
};
typedef struct color_code ColorCode;


static ColorCode color_codes[] = {
    {COLOR_BLACK, {0, 0, 0}},
    {COLOR_WHITE, {255, 255, 255}},
    {COLOR_RED, {255, 0, 0}},
    {COLOR_GREEN, {0, 255, 0}},
    {COLOR_BLUE, {0, 0, 255}},
    {COLOR_LIGHT_BLUE, {173, 216, 230}}, 
    {COLOR_YELLOW, {255, 255, 0}},
    {COLOR_CYAN, {0, 255, 255}},
    {COLOR_MAGENTA, {255, 0, 255}}
};



/*
    ,--------------------,
    ; Exported functions ;
    '--------------------'
*/

Color get_color(enum ColorName color_name) {
    for (int i = 0; i < sizeof(color_codes) / sizeof(color_codes[0]); i++) {
        if (color_codes[i].name == color_name) {
            return color_codes[i].color;
        }
    }
    return (Color){0, 0, 0}; // Default to black if not found
}

Color create_color(int r, int g, int b) {
    Color color;
    color.r = (unsigned char)r;
    color.g = (unsigned char)g;
    color.b = (unsigned char)b;
    return color;
}

int hex_color(Color color) {
    return (color.r << 16) | (color.g << 8) | color.b;
}
