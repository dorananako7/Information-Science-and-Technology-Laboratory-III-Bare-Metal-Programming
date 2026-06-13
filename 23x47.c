#include "const.h"

int main()
{
    // GPIOをすべて出力用に設定 (GPIO #4, #7, #8, #10, #12, #14〜#18, #20〜#25, #27)
    *GPFSEL0 = 0x01201000;
    *GPFSEL1 = 0x01fc1400; // ※(LED_PORTの分も含む)
    *GPFSEL2 = GPFSEL_VEC2;

    // 一旦、すべての列を 0 にし、すべての行を 1 にする（全消灯状態を作る）
    *GPCLR0 = (1<<COL1_PORT) | (1<<COL2_PORT) | (1<<COL3_PORT) | (1<<COL4_PORT) | 
              (1<<COL5_PORT) | (1<<COL6_PORT) | (1<<COL7_PORT) | (1<<COL8_PORT);
              
    *GPSET0 = (1<<ROW1_PORT) | (1<<ROW2_PORT) | (1<<ROW3_PORT) | (1<<ROW4_PORT) | 
              (1<<ROW5_PORT) | (1<<ROW6_PORT) | (1<<ROW7_PORT) | (1<<ROW8_PORT);

    // 目的の (2,3),(4,7)を点灯させる(だめなやり方)
    // COL3,COL7 に 1 を出力
    *GPSET0 = (1<<COL3_PORT) | (1<<COL7_PORT);
    // ROW2,ROW4 に 0 を出力
    *GPCLR0 = (1<<ROW2_PORT) | (1<<ROW4_PORT);


    // 点灯させたまま無限ループで維持
    for(;;) { }

    return 0;
}