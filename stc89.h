#ifndef __STC89_H__
#define __STC89_H__

void _sleep_ms() {
    byte i;
    for (i = 0; i < 100; i++) ;
    for (i = 0; i < 100; i++) ;
    for (i = 0; i < 100; i++) ;
    for (i = 0; i < 100; i++) ;
    for (i = 0; i < 45; i++) ;
}

void sleep(int millis) {
    for (; millis--; )
        _sleep_ms();
}

#endif