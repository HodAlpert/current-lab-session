//
// Created by hod on 02/04/18.
//
#ifndef TOY_PRINTF_TOY_STDIO_H
#define TOY_PRINTF_TOY_STDIO_H

#include <stdbool.h>
#include <stdarg.h>

enum printf_state {
    st_printf_init,
    st_printf_percent,
};

extern int toy_printf(char *fs, ...);

int addToWidthRequired(int curr);
void addPadding(int padding,char padd);
int integerWidth(unsigned int number);

int print_unsigned_int(unsigned int n, int radix, const char * digit);
int print_int_helper(unsigned int n, int radix, const char *digit);
int print_int(int n, int radix, const char * digit,int padding);
int print_string_array(char** array, int size);
int print_int_Array(int* array, int size,int radix, const char * digit, bool isunSigned);





typedef struct state_args {
    char* fs;
    int* int_array_value;
    char ** strings_array_value;
    int int_value;
    char *string_value;
    char char_value;
    bool isArray;
    bool isNegative;
    int padding;
} state_args;

struct state_result {
    int printed_chars;
    enum printf_state next_state;
}state_result;
struct state_result* percentHandler(va_list args, struct state_args* state);
struct state_result* initHandler(va_list args, struct state_args* state);
void Percent_char_Perchent_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_Minus_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_Number_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_A_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_d_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_b_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_o_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_x_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_X_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_u_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_s_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_c_Handler(struct state_result* result, va_list args, struct state_args* state);
void Percent_char_default_Handler(struct state_result *result, va_list args, struct state_args* state);

void Init_char_Percent_Handler(struct state_result* result, va_list args, struct state_args* state);
void Init_char_default_Handler(struct state_result* result, va_list args, struct state_args* state);





#endif //TOY_PRINTF_TOY_STDIO_H
