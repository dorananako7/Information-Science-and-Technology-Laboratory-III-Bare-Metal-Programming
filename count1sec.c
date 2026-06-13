#include "const.h"

// フレームバッファ
char frame_buffer[8];

// ROW,COLに関する定数を配列に格納
const int row_ports[8] = {ROW1_PORT, ROW2_PORT, ROW3_PORT, ROW4_PORT, ROW5_PORT, ROW6_PORT, ROW7_PORT, ROW8_PORT};
const int col_ports[8] = {COL1_PORT, COL2_PORT, COL3_PORT, COL4_PORT, COL5_PORT, COL6_PORT, COL7_PORT, COL8_PORT};

// フォントデータ
const char font[10][8] = {
    {0b0000, 0b1110, 0b1010, 0b1010, 0b1010, 0b1110, 0b0000, 0b0000}, // 0
    {0b0000, 0b100, 0b1100, 0b0100, 0b0100, 0b1110, 0b0000, 0b0000},  // 1
    {0b0000, 0b1110, 0b0010, 0b1110, 0b1000, 0b1110, 0b0000, 0b0000}, // 2
    {0b0000, 0b1110, 0b0010, 0b1110, 0b0010, 0b1110, 0b0000, 0b0000}, // 3
    {0b0000, 0b1010, 0b1010, 0b1110, 0b0010, 0b0010, 0b0000, 0b0000}, // 4
    {0b0000, 0b1110, 0b1000, 0b1110, 0b0010, 0b1110, 0b0000, 0b0000}, // 5
    {0b0000, 0b1110, 0b1000, 0b1110, 0b1010, 0b1110, 0b0000, 0b0000}, // 6
    {0b0000, 0b1110, 0b1010, 0b0010, 0b0010, 0b0010, 0b0000, 0b0000}, // 7
    {0b0000, 0b1110, 0b1010, 0b1110, 0b1010, 0b1110, 0b0000, 0b0000}, // 8
    {0b0000, 0b1110, 0b1010, 0b1110, 0b0010, 0b1110, 0b0000, 0b0000}  // 9
};

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
    // メモリの初期化
    for (i = 0; i < 8; i++)
    {
        frame_buffer[i] = 0;
    }

    // タスク1は1秒後に実行
    unsigned int target1 = *TMCLO + TIMER_HZ;
    // タスク2は0.001秒後に実行
    unsigned int target2 = *TMCLO + 1000;

    // 表示する数字のカウンター
    int num = 0;
    int r = 0; // 行を示す
    while (1)
    {
        // 現在時刻を読み出す(ポーリング)
        unsigned int time = *TMCLO;
        // フレームバッファに表示する数字を書き込む(タスク1)
        if (time >= target1)
        {
            // 数xの画像をframe_bufferに書き込む
            int ten = num / 10; // 2桁目
            int one = num % 10; // 1桁目

            // 10の位を左に4ビットシフトさせ、1桁目と足し合わせる。
            for (i = 0; i < 8; i++)
            {
                frame_buffer[i] = (font[ten][i] << 4) | font[one][i];
            }
            // 次に表示する数字
            num = (num + 7) % 100;
            target1 = target1 + TIMER_HZ; // 1秒追加
        }
        // 描画する(タスク2)
        if (time >= target2)
        {
            // 消灯
            *GPSET0 = (1 << row_ports[r]);
            r = (r + 1) % 8; // 8になったら0に戻る
            //すべての行を1にする
            *GPCLR0 = (1 << COL1_PORT) | (1 << COL2_PORT) | (1 << COL3_PORT) | (1 << COL4_PORT) |
                      (1 << COL5_PORT) | (1 << COL6_PORT) | (1 << COL7_PORT) | (1 << COL8_PORT);

            int c;
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
            target2 = target2 + 1000;
        }
    }
    return 0;
}