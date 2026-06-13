#include "const.h"

// const.hで定義した各音の周期（クロック数）を配列にする
int scale_keys[8] = {
    KEY_A4, KEY_B4, KEY_C5, KEY_D5, KEY_E5, KEY_F5, KEY_G5, KEY_A5
};

// 指定したマイクロ秒だけ待つ関数
void delay_us(unsigned int us) {
    unsigned int start = *TMCLO;
    while ((*TMCLO - start) < us) {
        // 指定時間が経過するまでループして待つ
    }
}

int main()
{
    // 1. GPIOピンの機能選択 (GPIO #19 を Alt5: PWM に設定)
    *GPFSEL1 = (*GPFSEL1 & ~(7 << 27)) | (2 << 27);

    // 2. クロックソースの設定
    *CM_PWMCTL = 0x5a000021; 
    while (*CM_PWMCTL & 0x80)
        ;                      
    *CM_PWMDIV = 0x5a000000 | (2 << 12); 
    *CM_PWMCTL = 0x5a000211;           

    // 3. PWMの動作モード設定
    *PWM_CTL = (1 << 15) | (1 << 8);

    // 4. 定義された音の周期を順番に読み出して鳴らす
    for (int i = 0; i < 8; i++) {
        int rng = scale_keys[i]; // 配列から現在の音の周期を取り出す
        
        *PWM_RNG2 = rng;          // 周期（音の高さ）
        *PWM_DAT2 = rng / 2;      // デューティ比50%

        // 0.5秒間鳴らす
        delay_us(500000);
    }

    // 5. すべての音を鳴らし終わったら音を止める
    *PWM_DAT2 = 0; 

    // 無限ループで待機
    while (1) {
        // 何もしない
    }

    return 0;
}
