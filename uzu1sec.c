#include "const.h"

char frame_buffer[8] = {0b00011110, 0b00100001, 0b01001100, 0b10010010, 0b01001001, 0b00100010, 0b00010100, 0b00001000};

// ROW,COLに関する定数を配列に格納
int row_ports[8] = {ROW1_PORT, ROW2_PORT, ROW3_PORT, ROW4_PORT, ROW5_PORT, ROW6_PORT, ROW7_PORT, ROW8_PORT};
int col_ports[8] = {COL1_PORT, COL2_PORT, COL3_PORT, COL4_PORT, COL5_PORT, COL6_PORT, COL7_PORT, COL8_PORT};

int main()
{
    unsigned int target;
    int row = 0; //現在点灯している行の番号(0から7)
    int i,c;

    // GPIOをすべて出力用に設定 (GPIO #4, #7, #8, #10, #12, #14〜#18, #20〜#25, #27)
    *GPFSEL0 = GPFSEL_VEC0;
    *GPFSEL1 = GPFSEL_VEC1; // ※(LED_PORTの分も含む)
    *GPFSEL2 = GPFSEL_VEC2;

    // 一旦、すべての列を 0 にし、すべての行を 1 にする（全消灯状態を作る）
    *GPCLR0 = (1 << COL1_PORT) | (1 << COL2_PORT) | (1 << COL3_PORT) | (1 << COL4_PORT) |
              (1 << COL5_PORT) | (1 << COL6_PORT) | (1 << COL7_PORT) | (1 << COL8_PORT);

    *GPSET0 = (1 << ROW1_PORT) | (1 << ROW2_PORT) | (1 << ROW3_PORT) | (1 << ROW4_PORT) |
              (1 << ROW5_PORT) | (1 << ROW6_PORT) | (1 << ROW7_PORT) | (1 << ROW8_PORT);

    //最初の目標を設定
    target = *TMCLO + 125000;
    while (1)
    {
       int time = *TMCLO;

        if(time >= target){
            //row行をすべて1(消灯)にする
            *GPSET0 = (1 << row_ports[row]);
            //次の行に進む
            row = (row + 1) % 8;
            //すべての列の出力を0にする
            for(i=0; i < 8; i++){
                *GPCLR0 = (1 << col_ports[i]);
            }
            //フレームバッファのデータに合わせて、点灯させたい列だけを出力する
            for(c = 0; c < 8; c++) {
                //1のところを点灯させる
                if(frame_buffer[row] & (1 << (7 - c))) {
                    *GPSET0 = (1 << col_ports[c]);
                }
            }
            //新しい行を0(点灯)する
            *GPCLR0 = (1 << row_ports[row]);
            //次の目標時刻を設定する
            target = target + 125000;
        }
    }
    return 0;
}