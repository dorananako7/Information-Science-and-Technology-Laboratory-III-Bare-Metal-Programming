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

    // LEDをonにするタスク
    unsigned int target1 = *TMCLO + 1000;
    // LEDをoffにするタスク
    unsigned int target2 = *TMCLO + TIMER_HZ / 2;
    // スイッチが次に反応できる時間
    unsigned int target3 = *TMCLO;
    // フレームバッファに表示する数字を書き込むタスク
    unsigned int target4 = *TMCLO + TIMER_HZ / 10;
    // 点灯させる行を切り替えるタスク
    unsigned int target5 = *TMCLO + 1000;

    int i;
    // メモリの初期化
    for (i = 0; i < 8; i++)
    {
        frame_buffer[i] = 0;
    }

    int num = 0;         // 表示する数字のカウンター
    int num_max = 99;    // 表示する数字の最大値
    int num_min = 0;     // 表示する数字の最小値
    int r = 0;           // 行を示す
    int timer_on = 0;    // タイマーが動いているか示す
    int preview_sw1 = 0; // 前回のsw1の物理的な状態を記憶
    int preview_sw2 = 0;
    int preview_sw3 = 0;
    int preview_sw4 = 0;
    int sw4_push_start_time = 0; //いつ押し始めたのか記憶
    int sw4_long_pressed = 0; //リセット処理を行ったのかを判定

    while (1)
    {
        // 現在時刻を読み出す(ポーリング)
        unsigned int time = *TMCLO;
        // SW1が押されているか調べる(チャタリング防止対策)
        int current_sw1 = (*GPLEV0 & (1 << SW1_PORT)) != 0;
        // イマ押されている かつ 前回は押されていなかった かつ 前回反応してから十分時間がたった
        if (current_sw1 == 1 && preview_sw1 == 0 && time >= target3)
        {
            if (timer_on == 0)
            {
                timer_on = 1;
            }
            else
            {
                timer_on = 0;
            }

            // 0.1秒間は次の入力を受け付けないようにする
            target3 = time + TIMER_HZ / 2;
        }
        // 今回のスイッチの状態を前回の状態として記憶
        preview_sw1 = current_sw1;

        // sw2が押されているか確認(チャタリング防止対策)
        int current_sw2 = (*GPLEV0 & (1 << SW2_PORT)) != 0;
        // イマ押されている かつ 前回は押されていなかった かつ 前回反応してから十分時間がたった
        if (current_sw2 == 1 && preview_sw2 == 0 && time >= target3)
        {
            if(timer_on == 0) {
                //0を下回らないための処理
                if(num > 0) {
                    num--;
                } else {
                    num = num_max;
                }
            }
            //タイマーを止める
            if(timer_on == 1) {
                timer_on = 0;
            }
            // 0.1秒間は次の入力を受け付けないようにする
            target3 = time + TIMER_HZ / 2;
        }
        // 今回のスイッチの状態を前回の状態として記憶
        preview_sw2 = current_sw2;

        // sw4が押されているか確認(チャタリング防止対策)
        int current_sw4 = (*GPLEV0 & (1 << SW4_PORT)) != 0;
        // イマ押されている かつ 前回は押されていなかった かつ 前回反応してから十分時間がたった
        if (current_sw4 == 1 && preview_sw4 == 0 && time >= target3)
        {
            if(timer_on == 0) {
                //99を超えないための処理
                if(num < 99) {
                    num++;
                } else {
                    num = num_min;
                }
                //押し始めた時刻を記録し、長押し完了フラグをリセットする
                 sw4_push_start_time = time;
                 sw4_long_pressed = 0; 
            }
            // 0.1秒間は次の入力を受け付けないようにする
            target3 = time + TIMER_HZ / 2;
        }
        //イマ押されている　かつ　タイマーが止まっている
        if(current_sw4 == 1 && timer_on == 0) {
            //リセットしてなく、押し始めてから1秒以上経過したか
            if(sw4_long_pressed == 0 && (time - sw4_push_start_time) >= TIMER_HZ) {
                num = 0;
                sw4_long_pressed = 1; //リセット完了のフラグを立てる
            }
        }
        // 今回のスイッチの状態を前回の状態として記憶
        preview_sw4 = current_sw4;

        // sw3が押されているか確認(チャタリング防止対策)
        int current_sw3 = (*GPLEV0 & (1 << SW3_PORT)) != 0;
        // イマ押されている かつ 前回は押されていなかった かつ 前回反応してから十分時間がたった
        if (current_sw3 == 1 && preview_sw3 == 0 && time >= target3)
        {
            //タイマーが停止しているかの確認
            if (timer_on == 0)
            {
                //99を溢れないようにする
                num = (num + 10) % 100;
            }
            // 0.1秒間は次の入力を受け付けないようにする
            target3 = time + TIMER_HZ / 2;
        }
        // 今回のスイッチの状態を前回の状態として記憶
        preview_sw3 = current_sw3;

        // LED点滅タスクの処理
        if (time >= target1)
        {
            // LEDを点灯
            *GPSET0 = 1 << LED_PORT;
            // 1秒足す
            target1 = target1 + TIMER_HZ;
        }
        if (time >= target2)
        {
            // LEDを消灯
            *GPCLR0 = 1 << LED_PORT;
            target2 = target2 + TIMER_HZ;
        }
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
            // スイッチが押されていたら数字を増やす
            if (timer_on == 1)
            {
                num++;
                if (num > num_max)
                {
                    num = num_min; // 99を超えたら0に戻る
                }
            }
            target4 = target4 + TIMER_HZ / 10; // 0.1秒追加
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
    }
    return 0;
}
