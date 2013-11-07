#ifndef __COMMON_H__
#define __COMMON_H__

#define NULL 0
#define TRUE 1
#define FALSE 0
#define nop() \
    __asm     \
        nop   \
    __endasm
#define nop2() \
    __asm     \
        nop   \
        nop   \
    __endasm
#define nop4() \
    __asm     \
        nop   \
        nop   \
        nop   \
        nop   \
    __endasm
#define nop8() \
    __asm     \
        nop   \
        nop   \
        nop   \
        nop   \
        nop   \
        nop   \
        nop   \
        nop   \
    __endasm

typedef char byte;
typedef unsigned char ubyte;
typedef char bool;

int abs(int value) {
    return value >= 0 ? value : -value;
}

void loop(byte times) {
    while (times--)
        ;
}

int round(float value) {
    int integral = (int)value;
    float fraction = value - integral;
    return fraction >= 0.5 ? integral + 1 : integral;
}

#endif