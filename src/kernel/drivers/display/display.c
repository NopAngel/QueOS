#include "display.h"

ScreenInfo _SYS_ScreenInfo;

typedef struct {
    multiboot_tag tag;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    uint16_t reserved;
} __attribute__((packed)) multiboot_tag_framebuffer;


/*
    ,--------------------,
    ; Exported functions ;
    '--------------------'
*/


void _sys_init_display(multiboot_tag* tag) {
    while (tag->type != MULTIBOOT_TAG_TYPE_END) {
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {

            multiboot_tag_framebuffer* fb = (multiboot_tag_framebuffer*)tag;
            uint32_t* framebuffer = (uint32_t*)(uintptr_t)fb->framebuffer_addr;
            _SYS_ScreenInfo.framebuffer = framebuffer;
            _SYS_ScreenInfo.width = fb->framebuffer_width;
            _SYS_ScreenInfo.height = fb->framebuffer_height;
            _SYS_ScreenInfo.pitch = fb->framebuffer_pitch / 4; 
            break;

        }
        tag = (multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }
}



void _sys_clear_screen(){
    for (int y = 0; y < _SYS_ScreenInfo.height; y++) {
        for (int x = 0; x < _SYS_ScreenInfo.width; x++) {
            _sys_put_pixel(x, y, 0x00000000); // Clear with black color
        }
    }
}


void _sys_put_pixel(int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || x >= _SYS_ScreenInfo.width || y >= _SYS_ScreenInfo.height) {
        return; // Out of bounds, do nothing
    }
    _SYS_ScreenInfo.framebuffer[y * _SYS_ScreenInfo.pitch + x] = color;
}


void _sys_fill_rect ( int x, int y, int width, int height, uint32_t color ){
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            _sys_put_pixel(x + j, y + i, color);
        }
    }
}


void _sys_put_char(int x, int y, uint32_t color, char c) {
    if (c < 0 || c >= 128) return; 
    for (int i = 0; i < 16; i++) {
        for (int j = 8; j < 16; j++) {
            if (font16[c*16 + i] & (1 << (15 - j))) {
                _sys_put_pixel(x + j - 8, y + i, color);
            }
        }
    }
}


void _sys_put_string(int x, int y, uint32_t color, const char* str) {
    int offsetX = 0;
    while (*str) {
        _sys_put_char(x + offsetX, y, color, *str++);
        offsetX += 8;
    }
}