#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "toy_stdio.h"

/* SUPPORTED:
 *   %b, %d, %o, %x, %X --
 *     integers in binary, decimal, octal, hex, and HEX
 *   %s -- strings
 */
const char *digit = "0123456789abcdef";

const char *DIGIT = "0123456789ABCDEF";
struct state_result* (*handlers[2])(va_list args, struct state_args *state) = {initHandler,percentHandler};
void (*PercentHandlers[128])(struct state_result* result,va_list args, struct state_args *state);
void (*InitHandlers[128])(struct state_result* result,va_list args, struct state_args *state);
void Percent_char_Perchent_Handler(struct state_result *result, va_list args, struct state_args *state) {
    putchar('%');
    result->next_state = st_printf_init;
    result->printed_chars = 1;
}
void Percent_char_Minus_Handler(struct state_result *result, va_list args, struct state_args *state) {
    state->isNegative=true;
    result->next_state = st_printf_percent;
    result->printed_chars = 0;
}
void Percent_char_Number_Handler(struct state_result *result, va_list args, struct state_args *state) {
    state->padding=addToWidthRequired(*state->fs-'0');
    result->next_state = st_printf_percent;
    result->printed_chars = 0;
}
void Percent_char_A_Handler(struct state_result *result, va_list args, struct state_args *state) {
    state->isArray=true;
    result->next_state = st_printf_percent;
    result->printed_chars = 0;
}
void Percent_char_d_Handler(struct state_result *result, va_list args, struct state_args *state) {
    if(state->isArray){
        state->int_array_value = va_arg(args, int*);
        result->printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),10, digit,false);
        state->isArray=false;
        result->next_state = st_printf_init;
    }
    else if (state->padding>0){
        state->int_value = va_arg(args, int);
        int actual_padding;
        if(state->int_value>=0){
            actual_padding=state->padding-integerWidth(state->int_value);
        }
        else
            actual_padding=state->padding-integerWidth(-state->int_value)-1;
        if(actual_padding>0)
            result->printed_chars += print_int(state->int_value, 10, digit, actual_padding);
        else
            result->printed_chars += print_int(state->int_value, 10, digit, 0);
        state->padding = addToWidthRequired(-1);
        result->next_state = st_printf_init;
    }
    else{
        state->int_value = va_arg(args, int);
        result->printed_chars += print_int(state->int_value, 10, digit, 0);
        result->next_state = st_printf_init;
    }
}
void Percent_char_b_Handler(struct state_result *result, va_list args, struct state_args *state) {
    if(state->isArray){
        state->int_array_value = va_arg(args, int*);
        result->printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),2, digit,true);
        state->isArray=false;
        result->next_state = st_printf_init;
    }
    else{
        state->int_value = va_arg(args, int);
        result->printed_chars += print_unsigned_int(state->int_value, 2, digit);
        result->next_state = st_printf_init;
    }
}
void Percent_char_o_Handler(struct state_result *result, va_list args, struct state_args *state) {
    if(state->isArray){
        state->int_array_value = va_arg(args, int*);
        result->printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),8, digit,true);
        state->isArray=false;
        result->next_state = st_printf_init;
    }
    else{
        state->int_value = va_arg(args, int);
        result->printed_chars += print_unsigned_int(state->int_value, 8, digit);
        result->next_state = st_printf_init;
    }
}
void Percent_char_x_Handler(struct state_result *result, va_list args, struct state_args *state) {
    if(state->isArray){
        state->int_array_value = va_arg(args, int*);
        result->printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),16, digit,true);
        state->isArray=false;
        result->next_state=st_printf_init;
    }
    else{
        state->int_value = va_arg(args, int);
        result->printed_chars += print_unsigned_int(state->int_value, 16, digit);
        result->next_state=st_printf_init;
    }
}
void Percent_char_X_Handler(struct state_result *result, va_list args, struct state_args *state) {
    if(state->isArray){
        state->int_array_value = va_arg(args, int*);
        result->printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),16, DIGIT,true);
        state->isArray=false;
        result->next_state=st_printf_init;
    }
    else{
        state->int_value = va_arg(args, int);
        result->printed_chars += print_unsigned_int(state->int_value, 16, DIGIT);
        result->next_state=st_printf_init;
    }
}
void Percent_char_u_Handler(struct state_result *result, va_list args, struct state_args *state) {
    if(state->isArray){
        state->int_array_value = va_arg(args, int*);
        result->printed_chars += print_int_Array(state->int_array_value,va_arg(args, int),10, digit,true);
        state->isArray=false;
        result->next_state=st_printf_init;
    }
    else{
        state->int_value = va_arg(args, int);
        result->printed_chars+=print_unsigned_int(state->int_value,10,digit);
        result->next_state=st_printf_init;
    }
}
void Percent_char_s_Handler(struct state_result *result, va_list args, struct state_args *state) {
    if(state->isArray){
        state->strings_array_value=va_arg(args, char**);
        result->printed_chars+=print_string_array(state->strings_array_value, va_arg(args,int));
        state->isArray=false;
        result->next_state=st_printf_init;
    }
    else if(state->padding>0){
        state->string_value = va_arg(args, char *);
        size_t actual_padding=state->padding-strlen(state->string_value);
        if (actual_padding>0){
            if(!state->isNegative){
                result->printed_chars += toy_printf(state->string_value);
                addPadding(actual_padding,' ');
                putchar('#');
            }
            else{
                addPadding(actual_padding,' ');
                result->printed_chars += toy_printf(state->string_value);
                state->isNegative=false;
            }
        }
        else
        {
            result->printed_chars += toy_printf(state->string_value);
        }
        state->padding = addToWidthRequired(-1);
        result->next_state=st_printf_init;

    }
    else{
        state->string_value = va_arg(args, char *);
        result->printed_chars += toy_printf(state->string_value);
        result->next_state=st_printf_init;
    }
}
void Percent_char_c_Handler(struct state_result *result, va_list args, struct state_args *state) {
    if(state->isArray){
        char *char_array = va_arg(args, char*);
        int size=va_arg(args,int);
        putchar('{');
        for(char *i=char_array;i<char_array+size;i++){
            putchar(*i);
            ++result->printed_chars;
            if(i<char_array+size-1)
                putchar(',');
        }
        putchar('}');
        state->isArray=false;
        result->next_state=st_printf_init;

    }
    else{
        state->char_value = (char)va_arg(args, int);
        putchar(state->char_value);
        ++result->printed_chars;
        result->next_state=st_printf_init;
    }
}
void Percent_char_default_Handler(struct state_result *result, va_list args, struct state_args *state) {
    toy_printf("Unhandled format %%%c...\n", *state->fs);
    exit(-1);
}
void initiateArray(){
    for (int i=0;i<128;i++){
        if ((char)i=='%')
            InitHandlers[i]=Init_char_Percent_Handler;
        else
            InitHandlers[i]= Init_char_default_Handler;
    }
    for(int i=0;i<128;i++){
        switch ((char)i){
            case '%':
                PercentHandlers[i]=Percent_char_Perchent_Handler;
                break;
            case '-':
                PercentHandlers[i]=Percent_char_Minus_Handler;
                break;
            case 48 ... 57:
                PercentHandlers[i]=Percent_char_Number_Handler;
                break;
            case 'A':
                PercentHandlers[i]=Percent_char_A_Handler;
                break;
            case 'd':
                PercentHandlers[i]=Percent_char_d_Handler;
                break;
            case 'b':
                PercentHandlers[i]=Percent_char_b_Handler;
                break;
            case 'o':
                PercentHandlers[i]=Percent_char_o_Handler;
                break;
            case 'x':
                PercentHandlers[i]=Percent_char_x_Handler;
                break;
            case 'X':
                PercentHandlers[i]=Percent_char_X_Handler;
                break;
            case 'u':
                PercentHandlers[i]=Percent_char_u_Handler;
                break;
            case 's':
                PercentHandlers[i]=Percent_char_s_Handler;
                break;
            case 'c':
                PercentHandlers[i]=Percent_char_c_Handler;
                break;
            default:
                PercentHandlers[i]=Percent_char_default_Handler;
        }
    }
}
void Init_char_Percent_Handler(struct state_result* result, va_list args, struct state_args *state){
    result->next_state = st_printf_percent;
    result->printed_chars = 0;
}
void Init_char_default_Handler(struct state_result* result, va_list args, struct state_args *state){
    putchar(*state->fs);
    result->next_state = st_printf_init;
    result->printed_chars = 1;
}

int print_int_helper(unsigned int n, int radix, const char *digit) {
    int result;

    if (n < radix) {
        putchar(digit[n]);
        return 1;
    }
    else {
        result = print_int_helper(n / radix, radix, digit);
        putchar(digit[n % radix]);
        return 1 + result;
    }
}

int print_unsigned_int(unsigned int n, int radix, const char * digit) {
    if (radix < 2 || radix > 16) {
        toy_printf("Radix must be in [2..16]: Not %d\n", radix);
        exit(-1);
    }

    if (n > 0) {
        return print_int_helper(n, radix, digit);
    }
    if (n == 0) {
        putchar('0');
    }
    return 1;

}

int print_int(int n, int radix, const char * digit,int padding) {
    if (radix < 2 || radix > 16) {
        toy_printf("Radix must be in [2..16]: Not %d\n", radix);
        exit(-1);
    }

    if (n > 0) {
        addPadding(padding,'0');
        return print_int_helper(n, radix, digit);
    }
    if (n == 0) {
        addPadding(padding,'0');
        putchar('0');
        return 1;
    }
    else {
        putchar('-');
        addPadding(padding,'0');
        return 1 + print_int_helper(-n, radix, digit);
    }
}

int print_string_array(char** array, int size){
    int ans = 0;
    putchar('{');
    for(char**i=array;i<array+size;i++){
        ans += toy_printf(*i);
        if (i<array+size-1)
            putchar(',');
    }
    putchar('}');
    return ans;
}

int print_int_Array(int* array, int size,int radix, const char * digit, bool isunSigned){
    int ans=0;
    putchar('{');
    for(int *i=array;i<array+size;i++){
        if(isunSigned)
            ans +=print_unsigned_int(*i, radix,digit);
        else
            ans +=print_int(*i, radix,digit,0);
        if(i<array+size-1)
            putchar(',');
    }
    putchar('}');
    return ans;
}

int addToWidthRequired(int curr){//sums up the width
    static int acc=0;
    if(curr<0)
        acc=0;
    else
        acc=(acc*10)+curr;
    return acc;
}

void addPadding(int padding,char padd){//assuming padding>0
    for(int i=0;i<padding;i++){
        putchar(padd);
    }
}

int integerWidth(unsigned int number){
    int ans=1;
    while(number>10){
        number=number/10;
        ans++;
    }
    return ans;
}

struct state_result* initHandler(va_list args, struct state_args *state) {
    struct state_result*result = malloc(sizeof(struct state_result));
    InitHandlers[*(state->fs)](result, args, state);
    return result;
}

struct state_result* percentHandler(va_list args, struct state_args *state) {
    struct state_result*result = malloc(sizeof(struct state_result));
    PercentHandlers[*(state->fs)](result,args,state);
    return result;
}

int toy_printf(char *fs, ...) {
    initiateArray();
    struct state_args* stateArgs = malloc(sizeof(struct state_args));
    struct state_result* result;
    stateArgs->isArray=false;
    stateArgs->isNegative=false;
    stateArgs->int_value=0;
    stateArgs->padding=0;
    int chars_printed = 0;
    int printed_chars=0;
    enum printf_state state;

    state = st_printf_init;

    va_list args;
    va_start(args, fs);
    for (; *fs != '\0'; ++fs) {
        chars_printed = 0;
        stateArgs->fs=fs;
        result = handlers[state](args, stateArgs);
        printed_chars+=result->printed_chars;
        state = result->next_state;
        free(result);
    }

    va_end(args);
    free(stateArgs);
    return chars_printed;
}

