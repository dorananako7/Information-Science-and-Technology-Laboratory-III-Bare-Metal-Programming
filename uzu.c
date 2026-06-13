#include "const.h"

int main()
{
    // GPIOをすべて出力用に設定 (GPIO #4, #7, #8, #10, #12, #14〜#18, #20〜#25, #27)
    *GPFSEL0 = GPFSEL_VEC0;
    *GPFSEL1 = GPFSEL_VEC1; // ※(LED_PORTの分も含む)
    *GPFSEL2 = GPFSEL_VEC2;

    // 一旦、すべての列を 0 にし、すべての行を 1 にする（全消灯状態を作る）
    *GPCLR0 = (1 << COL1_PORT) | (1 << COL2_PORT) | (1 << COL3_PORT) | (1 << COL4_PORT) |
              (1 << COL5_PORT) | (1 << COL6_PORT) | (1 << COL7_PORT) | (1 << COL8_PORT);

    *GPSET0 = (1 << ROW1_PORT) | (1 << ROW2_PORT) | (1 << ROW3_PORT) | (1 << ROW4_PORT) |
              (1 << ROW5_PORT) | (1 << ROW6_PORT) | (1 << ROW7_PORT) | (1 << ROW8_PORT);

    int i;
    while (1)
    {
        //1行目
        //ROW1に1を出力(消灯)
        *GPSET0 = (1<<ROW1_PORT);
        //2行目に光らせたいLEDを指定(列(COL)を指定)
        *GPCLR0 = (1<<COL1_PORT) | (1<<COL2_PORT) | (1<<COL3_PORT) | (1<<COL8_PORT);
        *GPSET0 = (1<<COL4_PORT) | (1<<COL5_PORT) | (1<<COL6_PORT) | (1<<COL7_PORT);
        //ROWに0を出力(点灯)
        *GPCLR0 = (1<<ROW1_PORT);
        for(i = 0; i < 1000; i++);
        //ROW1に1を出力(消灯)
        *GPSET0 = (1<<ROW1_PORT);

        //2行目
        //ROW2に1を出力(消灯)
        *GPSET0 = (1<<ROW2_PORT);
        //2行目に光らせたいLEDを指定(列(COL)を指定)
        *GPCLR0 = (1<<COL1_PORT) | (1<<COL2_PORT) | (1<<COL4_PORT) | (1<<COL5_PORT) | (1<<COL6_PORT) | (1<<COL7_PORT);
        *GPSET0 = (1<<COL3_PORT) | (1<<COL8_PORT);
        //ROW2に0を出力(点灯)
        *GPCLR0 = (1<<ROW2_PORT);
        for(i = 0; i < 1000; i++);
        //ROW2に1を出力(消灯)
        *GPSET0 = (1<<ROW2_PORT);

        //3行目
        //ROW3に1を出力(消灯)
        *GPSET0 = (1<<ROW3_PORT);
        //3行目に光らせたいLEDを指定(列(COL)を指定)
        *GPCLR0 = (1<<COL1_PORT) | (1<<COL3_PORT) | (1<<COL4_PORT) | (1<<COL7_PORT) | (1<<COL8_PORT);
        *GPSET0 = (1<<COL2_PORT) | (1<<COL5_PORT) | (1<<COL6_PORT);
        //ROW3に0を出力(点灯)
        *GPCLR0 = (1<<ROW3_PORT);
        for(i = 0; i < 1000; i++);
        //ROW3に1を出力(消灯)
        *GPSET0 = (1<<ROW3_PORT);        

        //4行目
        //ROW4に1を出力(消灯)
        *GPSET0 = (1<<ROW4_PORT);
        //4行目に光らせたいLEDを指定(列(COL)を指定)
        *GPCLR0 = (1<<COL2_PORT) | (1<<COL3_PORT) | (1<<COL5_PORT) | (1<<COL6_PORT) | (1<<COL8_PORT);
        *GPSET0 = (1<<COL1_PORT) | (1<<COL4_PORT) | (1<<COL7_PORT);
        //ROW4に0を出力(点灯)
        *GPCLR0 = (1<<ROW4_PORT);
        for(i = 0; i < 1000; i++);
        //ROW4に1を出力(消灯)
        *GPSET0 = (1<<ROW4_PORT);        

        //5行目
        //ROW5に1を出力(消灯)
        *GPSET0 = (1<<ROW5_PORT);
        //5行目に光らせたいLEDを指定(列(COL)を指定)
        *GPCLR0 = (1<<COL1_PORT) | (1<<COL3_PORT) | (1<<COL4_PORT) | (1<<COL6_PORT) | (1<<COL7_PORT);
        *GPSET0 = (1<<COL2_PORT) | (1<<COL5_PORT) | (1<<COL8_PORT);
        //ROW5に0を出力(点灯)
        *GPCLR0 = (1<<ROW5_PORT);
        for(i = 0; i < 1000; i++);
        //ROW5に1を出力(消灯)
        *GPSET0 = (1<<ROW5_PORT);        

        //6行目
        //ROW6に1を出力(消灯)
        *GPSET0 = (1<<ROW6_PORT);
        //6行目に光らせたいLEDを指定(列(COL)を指定)
        *GPCLR0 = (1<<COL1_PORT) | (1<<COL2_PORT) | (1<<COL4_PORT) | (1<<COL5_PORT) | (1<<COL6_PORT) | (1<<COL8_PORT);
        *GPSET0 = (1<<COL3_PORT) | (1<<COL7_PORT);
        //ROW6に0を出力(点灯)
        *GPCLR0 = (1<<ROW6_PORT);
        for(i = 0; i < 1000; i++);
        //ROW6に1を出力(消灯)
        *GPSET0 = (1<<ROW6_PORT);

        //7行目
        //ROW7に1を出力(消灯)
        *GPSET0 = (1<<ROW7_PORT);
        //7行目に光らせたいLEDを指定(列(COL)を指定)
        *GPCLR0 = (1<<COL1_PORT) | (1<<COL2_PORT) | (1<<COL3_PORT) | (1<<COL5_PORT) |  (1<<COL7_PORT) | (1<<COL8_PORT);
        *GPSET0 = (1<<COL4_PORT) | (1<<COL6_PORT);
        //ROW7に0を出力(点灯)
        *GPCLR0 = (1<<ROW7_PORT);
        for(i = 0; i < 1000; i++);
        //ROW7に1を出力(消灯)
        *GPSET0 = (1<<ROW7_PORT);

        //8行目
        //ROW8に1を出力(消灯)
        *GPSET0 = (1<<ROW8_PORT);
        //8行目に光らせたいLEDを指定(列(COL)を指定)
        *GPCLR0 = (1<<COL1_PORT) | (1<<COL2_PORT) | (1<<COL3_PORT) | (1<<COL4_PORT) | (1<<COL6_PORT) | (1<<COL7_PORT) | (1<<COL8_PORT);
        *GPSET0 = (1<<COL5_PORT);
        //ROW8に0を出力(点灯)
        *GPCLR0 = (1<<ROW8_PORT);
        for(i = 0; i < 1000; i++);
        //ROW8に1を出力(消灯)
        *GPSET0 = (1<<ROW8_PORT);
    }

    return 0;
}