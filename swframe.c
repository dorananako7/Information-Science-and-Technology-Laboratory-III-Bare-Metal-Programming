#include "const.h"

// フレームバッファ
char frame_buffer[8] = {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};

// ROW,COLに関する定数を配列に格納
int row_ports[8] = {ROW1_PORT, ROW2_PORT, ROW3_PORT, ROW4_PORT, ROW5_PORT, ROW6_PORT, ROW7_PORT, ROW8_PORT};
int col_ports[8] = {COL1_PORT, COL2_PORT, COL3_PORT, COL4_PORT, COL5_PORT, COL6_PORT, COL7_PORT, COL8_PORT};

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

    int i, k, r, c;

    //メモリの初期化
    for (r = 0; r < 8; r++) {
        frame_buffer[r] = 0;
    }
    // ボタンが押されている間に書き換える場所を示すカウンター
    int x = 0;
    while (1)
    {
        // SW1が押されているか調べる
        if ((*GPLEV0 & (1 << SW1_PORT)) != 0)
        {

            // 点x (0〜63) に対応する「行(r)」と「列(c)」を計算する
            r = x / 8;
            c = x % 8;

            // 排他的論理和を使って、その場所のビットを反転させる
            frame_buffer[r] = frame_buffer[r] ^ (1 << (7 - c));

            // 次のLEDへ進む
            x = x + 1;
            if (x == 64)
            {
                x = 0; // 最後まで行ったら最初に戻る
            }
        }
        // 10回ずつスイッチが押されているかチェックする
        for (k = 0; k < 10; k++)
        {
            // 1行ずつ描画処理
            for (r = 0; r < 8; r++)
            {
                *GPSET0 = (1 << row_ports[r]);
                *GPCLR0 = (1 << COL1_PORT) | (1 << COL2_PORT) | (1 << COL3_PORT) | (1 << COL4_PORT) |
                          (1 << COL5_PORT) | (1 << COL6_PORT) | (1 << COL7_PORT) | (1 << COL8_PORT);

                for (c = 0; c < 8; c++)
                {
                    // もし7-c列目が1であったら
                    if (frame_buffer[r] & (1 << (7 - c)))
                    {
                        // 描画
                        *GPSET0 = (1 << col_ports[c]);
                    }
                }

                // 点灯
                *GPCLR0 = (1 << row_ports[r]);
                // 待つ
                for (i = 0; i < 1000; i++)
                    ;
                // 消灯
                *GPSET0 = (1 << row_ports[r]);
            }
        }
    }
    return 0;
}