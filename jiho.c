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

    // 音を止めるタスク用の目標時刻
    unsigned int target_sound_off = 0;
    // フレームバッファに表示する数字を書き込むタスク
    unsigned int target4 = *TMCLO + TIMER_HZ;
    // 点灯させる行を切り替えるタスク
    unsigned int target5 = *TMCLO + 1000;

    int i;
    // メモリの初期化
    for (i = 0; i < 8; i++)
    {
        frame_buffer[i] = 0;
    }

    int num = 0;      // 表示する数字のカウンター
    int num_max = 10; // 表示する数字の最大値
    int num_min = 0;  // 表示する数字の最小値
    int r = 0;        // 行を示す

    // 1. GPIOピンの機能選択
    // 111を27から29ビットのところに置き、それを、反転(~ not)させる。
    // それを、and演算でくっつけることで、他のピンの設定はそのまま保護した状態で書き換えれる
    // 最後に、010を27ビットから29ビット目にor演算で足してピンの設定を行う
    *GPFSEL1 = (*GPFSEL1 & ~(7 << 27)) | (2 << 27);

    // 2. クロックソースの設定(講義資料より)
    // 2. クロックソースの設定
    *CM_PWMCTL = 0x5a000021; // src=osc, enable=false
    while (*CM_PWMCTL & 0x80)
        ;                              // busyビット(7ビット目)がクリアされるまで確実に待つ
    *CM_PWMDIV = 0x5a000000 | 2 << 12; // div=2.0
    *CM_PWMCTL = 0x5a000211;           // src=osc, enable=true

    // 3. PWMの動作モード設定
    // 8ビット目を1にして、動作開始に
    // 15ビット目を1にしてM/S転送モードに
    *PWM_CTL = (1 << 15) | (1 << 8);
    *PWM_DAT2 = 0; // 最初は無音にしておく

    while (1)
    {
        // 現在時刻を読み出す(ポーリング)
        unsigned int time = *TMCLO;
        // フレームバッファに表示する数字を書き込むタスクの処理
        if (time >= target4)
        {
            // 数xの画像をframe_bufferに書き込む
            int ten = num / 10; // 2桁目
            int one = num % 10; // 1桁目

            // 10の位を左に4ビットシフトさせ、1桁目と足し合わせる。
            for (i = 0; i < 8; i++)
            {
                frame_buffer[i] = (font[ten][i] << 4) | font[one][i];
            }
            if (num == 7 || num == 8 || num == 9)
            {
                *PWM_RNG2 = KEY_A4;                        // 周波数(音の高さ)
                *PWM_DAT2 = KEY_A4 / 2;                    // onとoffの割合が半々のきれいな並を作る
                target_sound_off = time + (TIMER_HZ / 10); // 0.1秒後に音を止める
            }
            else if (num == 0)
            {
                *PWM_RNG2 = KEY_A4 / 2;                       // 周期を半分にして 880Hz (1オクターブ上)
                *PWM_DAT2 = (KEY_A4 / 2) / 2;                 // 発音開始
                target_sound_off = time + (TIMER_HZ * 3 / 4); // 0.75秒後に止めるよう予約
            }
            if (num < 9)
            {
                num++;
            }
            else
            {
                num = num_min; // 99を超えたら0に戻る
            }
            target4 = target4 + TIMER_HZ; // 1秒追加
        }

        // 点灯させる行を切り替えるタスクの処理
        if (time >= target5)
        {
            // 消灯
            *GPSET0 = (1 << row_ports[r]);
            r = (r + 1) % 8; // 8になったら0に戻る
            // すべての行を1にする
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
            target5 = target5 + 1000; // 0.001秒追加
        }

        // 音を止めるタスクの処理
        if (target_sound_off != 0 && time >= target_sound_off)
        {
            *PWM_DAT2 = 0;        // 音を止める
            target_sound_off = 0; // タスクの状態をリセット
        }
    }
    return 0;
}
