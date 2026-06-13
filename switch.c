#include "const.h"      // 定数の定義を読み込む

// 悪い例: LEDを点灯して消灯する（点灯時間短すぎ）
int main()
{
  volatile int i, sw;
  // GPIOの機能設定
  *GPFSEL1 = GPFSEL_VEC1;  // #10〜#19の設定


  // ** ここに待ち時間が必要 **
  for(;;) {
	   // GPLEV0の値 (GPIO #0〜#31それぞれが読み取った値) をswに代入
  sw = *GPLEV0;
   if(sw & (1 << SW1_PORT)) {
       // LED点灯 (LED_PORTの出力を1にする)
  	*GPSET0 = 1 << LED_PORT;
   }
else {
  // LED消灯 (LED_PORTの出力を0にする)
  *GPCLR0 = 1 << LED_PORT; 
}
}
  return 0;  // init.sに戻る
}
