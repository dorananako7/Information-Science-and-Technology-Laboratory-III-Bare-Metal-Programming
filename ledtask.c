#include "const.h"

void main(void) {
    unsigned int time;
    unsigned int target1;
    unsigned int target2;

    *GPFSEL1 = GPFSEL_VEC1;

    target1 = *TMCLO + 1000;
    target2 = target1 + (3 * TIMER_HZ / 4);

    while (1) {
        time = *TMCLO;

        if (time >= target1) {
            *GPSET0 = (1 << LED_PORT);
            target1 += 1 * TIMER_HZ;
        }

        if (time >= target2) {
            *GPCLR0 = (1 << LED_PORT);
            target2 += 1 * TIMER_HZ;
        }
    }
}

