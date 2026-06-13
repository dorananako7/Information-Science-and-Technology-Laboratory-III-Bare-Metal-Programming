#include "const.h"

// 約0.5秒点灯，約0.5秒消灯 を繰り返すプログラム
int main()
{
    unsigned int time; // 現在のカウンタ値

    // LEDに接続されたGPIOポートを出力用に設定
    *GPFSEL1 = GPFSEL_VEC1;

    while (1)
    {                  

        time = *TMCLO; // 現在時刻を読み出す (ポーリング)
        // timeのビット19が1
        if ((time & (1 << 19)) != 0)
        {
            *GPSET0 = 1 << LED_PORT; // 点灯
        }
        // timeの19ビットが0
        else
        {
            *GPCLR0 = 1 << LED_PORT; // 消灯
        }
    }

    return 0; // この行には到達しないが安全のため記述
}