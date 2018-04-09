#include "toy_stdio.h"



int main(int argc, char *argv[]) {
    toy_printf("Hex unsigned: %x\n", -1);//Hex unsigned: ffffffff
    toy_printf("Octal unsigned: %o\n", -1);//Octal unsigned: 37777777777
    toy_printf("Unsigned value: %u\n", 15);//Unsigned value: 15
    toy_printf("Unsigned value: %u\n", -15);//Unsigned value: 4294967281
    int integers_array[] = {1,2,3,4,5};
    char * strings_array[] = {"This", "is", "array", "of", "strings"};
    int array_size = 5;
    toy_printf("Print array of integers: %Ad\n", integers_array, array_size);//Print array of integers: {1, 2, 3, 4, 5}
    toy_printf("Print array of strings: %As\n", strings_array, array_size);//Print array of strings:  {This,  is,  array,  of,  strings}
    toy_printf("Non-padded string: %s\n", "str");//Non-padded string: str
    toy_printf("Right-padded string: %6s\n", "str");//Right-padded string: str   #
    toy_printf("Left-padded string: %-6s\n", "str");//Left-padded string:    str
    toy_printf("With numeric placeholders: %05d\n", -1);//With numeric placeholders: -0001







}