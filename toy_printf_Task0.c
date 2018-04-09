///* caspl-lab-1.c
// * Limited versions of printf
// *
// * Programmer: Mayer Goldberg, 2018
// */
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdarg.h>
//#include <stdbool.h>
//#include <string.h>
//#include "toy_stdio.h"
//
//int print_int_helper(unsigned int n, int radix, const char *digit) {
//    int result;
//
//    if (n < radix) {
//        putchar(digit[n]);
//        return 1;
//    }
//    else {
//        result = print_int_helper(n / radix, radix, digit);
//        putchar(digit[n % radix]);
//        return 1 + result;
//    }
//}
//int print_unsigned_int(unsigned int n, int radix, const char * digit) {
//    if (radix < 2 || radix > 16) {
//        toy_printf("Radix must be in [2..16]: Not %d\n", radix);
//        exit(-1);
//    }
//
//    if (n > 0) {
//        return print_int_helper(n, radix, digit);
//    }
//    if (n == 0) {
//        putchar('0');
//    }
//    return 1;
//
//}
//int print_int(int n, int radix, const char * digit,int padding) {
//    if (radix < 2 || radix > 16) {
//        toy_printf("Radix must be in [2..16]: Not %d\n", radix);
//        exit(-1);
//    }
//
//    if (n > 0) {
//        addPadding(padding,'0');
//        return print_int_helper(n, radix, digit);
//    }
//    if (n == 0) {
//        addPadding(padding,'0');
//        putchar('0');
//        return 1;
//    }
//    else {
//        putchar('-');
//        addPadding(padding,'0');
//        return 1 + print_int_helper(-n, radix, digit);
//    }
//}
//
//int print_string_array(char** array, int size){
//    int ans = 0;
//    putchar('{');
//    for(char**i=array;i<array+size;i++){
//        ans += toy_printf(*i);
//        if (i<array+size-1)
//            putchar(',');
//    }
//    putchar('}');
//    return ans;
//}
//int print_int_Array(int* array, int size,int radix, const char * digit, bool isunSigned){
//    int ans=0;
//    putchar('{');
//    for(int *i=array;i<array+size;i++){
//        if(isunSigned)
//            ans +=print_unsigned_int(*i, radix,digit);
//        else
//            ans +=print_int(*i, radix,digit,0);
//        if(i<array+size-1)
//            putchar(',');
//    }
//    putchar('}');
//    return ans;
//}
//int addToWidthRequired(int curr){//sums up the width
//    static int acc=0;
//    if(curr<0)
//        acc=0;
//    else
//        acc=(acc*10)+curr;
//    return acc;
//}
//void addPadding(int padding,char padd){//assuming padding>0
//    for(int i=0;i<padding;i++){
//        putchar(padd);
//    }
//}
//int integerWidth(unsigned int number){
//    int ans=1;
//    while(number>10){
//        number=number/10;
//        ans++;
//    }
//    return ans;
//}
//
///* SUPPORTED:
// *   %b, %d, %o, %x, %X --
// *     integers in binary, decimal, octal, hex, and HEX
// *   %s -- strings
// */
//const char *digit = "0123456789abcdef";
//const char *DIGIT = "0123456789ABCDEF";
//enum printf_state initHandler(va_list args, int *out_printed_chars, struct state_args *state) {
//    switch (*state->fs) {
//        case '%':
//            return st_printf_percent;
//        default:
//            putchar(*state->fs);
//            state->isArray=false;
//            ++*out_printed_chars;
//            return st_printf_init;
//    }
//}
//
//enum printf_state percentHandler(va_list args, int *out_printed_chars, struct state_args *state) {
//    switch (*state->fs) {
//        case '%':
//            putchar('%');
//            ++*out_printed_chars;
//            return  st_printf_init;
//        case '-':
//            state->isNegative=true;
//            return st_printf_percent;
//        case 48 ... 57:
//            state->padding=addToWidthRequired(*state->fs-'0');
//            return st_printf_percent;
//        case 'A':
//            state->isArray=true;
//            return st_printf_percent;
//
//        case 'd':
//            if(state->isArray){
//                state->int_array_value = va_arg(args, int*);
//                *out_printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),10, digit,false);
//                state->isArray=false;
//                return st_printf_init;
//            }
//            else if (state->padding>0){
//                state->int_value = va_arg(args, int);
//                int actual_padding;
//                if(state->int_value>=0){
//                    actual_padding=state->padding-integerWidth(state->int_value);
//                }
//                else
//                    actual_padding=state->padding-integerWidth(-state->int_value)-1;
//                if(actual_padding>0)
//                    *out_printed_chars += print_int(state->int_value, 10, digit, actual_padding);
//                else
//                    *out_printed_chars += print_int(state->int_value, 10, digit, 0);
//                state->padding = addToWidthRequired(-1);
//                return st_printf_init;
//            }
//            else{
//                state->int_value = va_arg(args, int);
//                *out_printed_chars += print_int(state->int_value, 10, digit, 0);
//                return st_printf_init;
//            }
//        case 'b':
//            if(state->isArray){
//                state->int_array_value = va_arg(args, int*);
//                *out_printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),2, digit,true);
//                state->isArray=false;
//                return st_printf_init;
//            }
//            else{
//                state->int_value = va_arg(args, int);
//                *out_printed_chars += print_unsigned_int(state->int_value, 2, digit);
//                return st_printf_init;
//            }
//        case 'o':
//            if(state->isArray){
//                state->int_array_value = va_arg(args, int*);
//                *out_printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),8, digit,true);
//                state->isArray=false;
//                return st_printf_init;
//            }
//            else{
//                state->int_value = va_arg(args, int);
//                *out_printed_chars += print_unsigned_int(state->int_value, 8, digit);
//                return st_printf_init;
//            }
//        case 'x':
//            if(state->isArray){
//                state->int_array_value = va_arg(args, int*);
//                *out_printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),16, digit,true);
//                state->isArray=false;
//                return st_printf_init;
//            }
//            else{
//                state->int_value = va_arg(args, int);
//                *out_printed_chars += print_unsigned_int(state->int_value, 16, digit);
//                return st_printf_init;
//            }
//
//        case 'X':
//            if(state->isArray){
//                state->int_array_value = va_arg(args, int*);
//                *out_printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),16, DIGIT,true);
//                state->isArray=false;
//                return st_printf_init;
//            }
//            else{
//                state->int_value = va_arg(args, int);
//                *out_printed_chars += print_unsigned_int(state->int_value, 16, DIGIT);
//                return st_printf_init;
//            }
//        case 'u':
//            if(state->isArray){
//                state->int_array_value = va_arg(args, int*);
//                *out_printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),10, digit,true);
//                state->isArray=false;
//                return st_printf_init;
//            }
//            else{
//                state->int_value = va_arg(args, int);
//                *out_printed_chars+=print_unsigned_int(state->int_value,10,digit);
//                return st_printf_init;
//            }
//        case 's':
//            if(state->isArray){
//                state->strings_array_value=va_arg(args, char**);
//                *out_printed_chars+=print_string_array(state->strings_array_value, va_arg(args,int));
//                state->isArray=false;
//                return st_printf_init;
//            }
//            else if(state->padding>0){
//                state->string_value = va_arg(args, char *);
//                size_t actual_padding=state->padding-strlen(state->string_value);
//                if (actual_padding>0){
//                    if(!state->isNegative){
//                        *out_printed_chars += toy_printf(state->string_value);
//                        addPadding(actual_padding,' ');
//                        putchar('#');
//                    }
//                    else{
//                        addPadding(actual_padding,' ');
//                        *out_printed_chars += toy_printf(state->string_value);
//                        state->isNegative=false;
//                    }
//                }
//                else
//                {
//                    *out_printed_chars += toy_printf(state->string_value);
//                }
//                state->padding = addToWidthRequired(-1);
//                return st_printf_init;
//
//            }
//            else{
//                state->string_value = va_arg(args, char *);
//                *out_printed_chars += toy_printf(state->string_value);
//                return st_printf_init;
//            }
//
//        case 'c':
//            if(state->isArray){
//                char *char_array = va_arg(args, char*);
//                int size=va_arg(args,int);
//                putchar('{');
//                for(char *i=char_array;i<char_array+size;i++){
//                    putchar(*i);
//                    ++*out_printed_chars;
//                    if(i<char_array+size-1)
//                        putchar(',');
//                }
//                putchar('}');
//                state->isArray=false;
//                return st_printf_init;
//
//            }
//            else{
//                state->char_value = (char)va_arg(args, int);
//                putchar(state->char_value);
//                ++*out_printed_chars;
//                return st_printf_init;
//            }
//
//        default:
//            toy_printf("Unhandled format %%%c...\n", *state->fs);
//            exit(-1);
//    }
//}
//
//int toy_printf(char *fs, ...) {
//    struct state_args* stateArgs = malloc(sizeof(struct state_args));
//    stateArgs->isArray=false;
//    stateArgs->isNegative=false;
//    stateArgs->int_value=0;
//    stateArgs->padding=0;
//    int chars_printed = 0;
//    int printed_chars=0;
//    enum printf_state state;
//
//    state = st_printf_init;
//
//    va_list args;
//    va_start(args, fs);
//    for (; *fs != '\0'; ++fs) {
//        chars_printed = 0;
//        stateArgs->fs=fs;
//        switch (state) {
//            case st_printf_init:
//                state = initHandler(args, &chars_printed, stateArgs);
//                printed_chars+=chars_printed;
//                break;
//            case st_printf_percent:
//                state=percentHandler(args, &chars_printed, stateArgs);
//                printed_chars+=chars_printed;
//        }
//    }
//
//    va_end(args);
//    free(stateArgs);
//    return chars_printed;
//}
//
