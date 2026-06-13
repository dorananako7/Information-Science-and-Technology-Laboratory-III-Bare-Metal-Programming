#include "const.h"      // 定数の定義を読み込む

// LEDを点灯するだけのプログラム
int main()
{
  // GPIOの機能設定
  *GPFSEL1 = GPFSEL_VEC1;  // #10〜#19の設定

  // LED点灯
  *GPSET0 = 1 << LED_PORT;

  return 0;  // init.sに戻る
}
