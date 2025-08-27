#include "keyboard.h"

extern void printf(char *str);

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define BUFFER_SIZE 256

static char keyboard_buffer[BUFFER_SIZE];
static int kb_head=0;
static int kb_tail=0;

static char scancode_to_ascii(uint8_t scancode) {
    const char keymap[128] = {
        0,27,   '1','2','3','4','5','6','7','8','9','0','-','=',   '\b',     // 0x00 - 0x0E
        '\t',     'q','w','e','r','t','y','u','i','o','p','[',']', '\n',     // 0x0F - 0x1C
        0,         'a','s','d','f','g','h','j','k','l',';','\'',    '`',     // 0x1D - 0x29
        0,'\\',     'z','x','c','v','b','n','m',',','.','/',          0,     // 0x2A - 0x35
        '*', 0,                       ' '                                    // 0x36 - 0x39
    };
    return keymap[scancode];
}


static void keyboard_handler(ISR_StackFrame* regs) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    if (scancode < 0x80) {
        char c = scancode_to_ascii(scancode);
        if (c) {
            if(c=='\b'){
                if(kb_tail != kb_head) {
                    kb_tail = (kb_tail - 1 + BUFFER_SIZE) % BUFFER_SIZE; 
                    return;
                }
            } 

            keyboard_buffer[kb_tail]=c;
            kb_tail=(kb_tail+1) % BUFFER_SIZE;

            // overflow handling
            if(kb_tail == kb_head) {
                kb_head = (kb_head + 1) % BUFFER_SIZE;
            }            

        }
    }
}



/*
    ,--------------------,
    ; Exported functions ;
    '--------------------'
*/

char _sys_get_char(){
    if(kb_head==kb_tail) {
        asm volatile("hlt");
        return 0; 
    }

    char c = keyboard_buffer[kb_head];
    kb_head = (kb_head + 1) % BUFFER_SIZE;
    return c;
}


void _sys_init_keyboard() {
    set_IRQ_Handler(1, keyboard_handler);
}