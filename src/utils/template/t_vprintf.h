#ifndef PRINTF_H
#define PRINTF_H

#include "../../kernel/global.h"


static enum printf_state {
    PS_NORMAL,         
    PS_PERCENT,   
    PS_TYPE
} state;

static enum printf_size {
    PS_DEFAULT,
    PS_LONG,
    PS_LONG_LONG
} size;

static void virtual_putc(char c);


static void puts(char *str){
    while(*str){
        virtual_putc(*str);
        str++;
    }
}

static int t_vprintf(const char *fmt, va_list args){    
    while(*fmt){
        if(state==PS_NORMAL){
            if(*fmt == '%'){
                state = PS_PERCENT;
            }else{
                virtual_putc(*fmt);
            }
            fmt++;
        }else if(state==PS_PERCENT){
            if(*fmt == 'l'){
                fmt++;
                size = PS_LONG;
                if(*fmt == 'l'){  // long long
                    fmt++;
                    size = PS_LONG_LONG;
                }
            }
            state = PS_TYPE;

        }else if(state==PS_TYPE){
            char number_buffer[32];
            int temp;
            bool is_negative = false;
            bool is_unsigned = false;
            unsigned long long value;

            switch (*fmt){
                case 'u':   is_unsigned = true;
                case 'i':
                case 'd':   if(size == PS_LONG_LONG)
                                temp = va_arg(args, long long int);
                            else 
                                temp = va_arg(args, int);

                            if(!is_unsigned && temp<0)
                                is_negative = true;
                            
                            if(is_negative)
                                value = -temp;
                            else
                                value = temp;

                            u_numberToStr(value, 10, is_negative, number_buffer);
                            puts(number_buffer);
                            break;

                case 'X':
                case 'x':   value = va_arg(args, int);
                            u_numberToStr(value, 16, false, number_buffer);
                            puts(number_buffer);
                            break;

                case 'b':   value = va_arg(args, int);
                            for(int i=0;i<32;i++){
                                virtual_putc((value & (1 << (31 - i))) ? '1' : '0');
                            }
                            break;

                case 'p':   uintptr_t ptr = va_arg(args, uintptr_t);
                            addrToStr(ptr, number_buffer);
                            puts(number_buffer);
                            break;

                case 's':   puts(va_arg(args, char *));
                            break;

                case 'c':   virtual_putc((char)va_arg(args, int));
                            break;

                default:    break;
            }
            state = PS_NORMAL;
            size = PS_DEFAULT;
            fmt++;
        }
    }
}



static int t_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = t_vprintf(fmt, args);
    va_end(args);
    return ret;
}

#endif