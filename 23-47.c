#include "const.h"      // 定数の定義を読み込む

// 点(2,3)と(4,7)を点灯するプログラム
int main()
{
  // GPIOの機能設定
  *GPFSEL0 = GPFSEL_VEC0;  // #0〜#9の設定
  *GPFSEL1 = GPFSEL_VEC1;  // #10〜#19の設定
  *GPFSEL2 = GPFSEL_VEC2;  // #20〜#29の設定

  // 無限ループ (forのセミコロン修正)
  for(;;) {
    
    // --- ① (2,3) を点灯 ---
    // COL3のみ1, それ以外の列は0にする
    *GPCLR0 = 1 << COL1_PORT
            | 1 << COL2_PORT
            | 1 << COL4_PORT
            | 1 << COL5_PORT
            | 1 << COL6_PORT
            | 1 << COL7_PORT
            | 1 << COL8_PORT;
    *GPSET0 = 1 << COL3_PORT;

    // ROW2のみ0, それ以外の行は1にする
    *GPSET0 = 1 << ROW1_PORT
            | 1 << ROW3_PORT
            | 1 << ROW4_PORT
            | 1 << ROW5_PORT
            | 1 << ROW6_PORT
            | 1 << ROW7_PORT
            | 1 << ROW8_PORT;
    *GPCLR0 = 1 << ROW2_PORT;


    // --- ② (4,7) を点灯 ---
    // COL7のみ1, それ以外の列は0にする
    *GPCLR0 = 1 << COL1_PORT
            | 1 << COL2_PORT
            | 1 << COL3_PORT
            | 1 << COL5_PORT
            | 1 << COL6_PORT
            | 1 << COL4_PORT
            | 1 << COL8_PORT;
    *GPSET0 = 1 << COL7_PORT;

    // ROW4のみ0, それ以外の行は1にする
    *GPSET0 = 1 << ROW1_PORT
            | 1 << ROW3_PORT
            | 1 << ROW2_PORT
            | 1 << ROW5_PORT
            | 1 << ROW6_PORT
            | 1 << ROW7_PORT
            | 1 << ROW8_PORT;
    *GPCLR0 = 1 << ROW4_PORT;

  }

  return 0;  // init.sに戻る
}
