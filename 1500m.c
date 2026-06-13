#include "const.h"

int main()
{
    unsigned int time, target; // 現在のカウンタ値
    int led_state = 0;
    // LEDに接続されたGPIOポートを出力用に設定
    *GPFSEL1 = GPFSEL_VEC1;
    target = *TMCLO + 1500000;
    while (1)
    {

        time = *TMCLO; // 現在時刻を読み出す ([>
        // timeのビット19が1
        if (led_state == 0)
        {
            *GPSET0 = 1 << LED_PORT; // 点灯
	    led_state = 1;
        }
        else
        {
            *GPCLR0 = 1 << LED_PORT; // 消灯
            led_state = 0;        
}
		target += 1500000;
    }

    return 0; // この行には到達しないが安全のた>
}

