#define GPIO_BASE       (volatile int *)0x3f200000
#define GPFSEL0         (GPIO_BASE + 0)  // GPIO #0〜9の機能選択
#define GPFSEL1         (GPIO_BASE + 1)  // GPIO #10〜19の機能選択
#define GPFSEL2         (GPIO_BASE + 2)  // GPIO #20〜29の機能選択
#define GPSET0          (GPIO_BASE + 7)  // 出力値を1にする
#define GPCLR0          (GPIO_BASE + 10) // 出力値を0にする
#define GPLEV0          (GPIO_BASE + 13) // 入力値を読み出す

// GPFSELxに設定する値
#define GPFSEL_VEC0     0x01201000  // GPFSEL0用 (#4, #7, #8 が出力)
#define GPFSEL_VEC1     0x01249041  // GPFSEL1用 (#10, #12, #14〜18 が出力)
#define GPFSEL_VEC2     0x00209249  // GPFSEL2用 (#20〜25, #27 が出力)

// 各装置が接続されているGPIO番号
#define LED_PORT        10  // LED
#define SW1_PORT        13  // SW1 (※実験ボードの仕様に合わせる)
#define SW2_PORT 26
#define SW3_PORT 5
#define SW4_PORT 6
//タイマーの周波数
#define TIMER_HZ     (1000 * 1000)
// 8x8ドットマトリックスLEDの各ポート番号
//列
#define COL1_PORT 27
#define COL2_PORT 8
#define COL3_PORT 25
#define COL4_PORT 23
#define COL5_PORT 24
#define COL6_PORT 22
#define COL7_PORT 17
#define COL8_PORT 4
//行
#define ROW1_PORT 14
#define ROW2_PORT 15
#define ROW3_PORT 21
#define ROW4_PORT 18
#define ROW5_PORT 12
#define ROW6_PORT 20
#define ROW7_PORT 7
#define ROW8_PORT 16

//システムタイマの番地
#define TIME_BASE  (volatile int *)0x3f003000
#define TMCLO  (TIME_BASE + 1) // +1 は int型(4バイト)分進むという意味で、ベースアドレス + 4番地 を表します


/* クロックマネージャ関連 */
#define CM_BASE ((volatile unsigned int *)0x3f101000)
#define CM_PWMCTL (CM_BASE + 40)
#define CM_PWMDIV (CM_BASE + 41)

/* PWM関連 */
#define PWM_BASE ((volatile unsigned int *)0x3f20c000)
#define PWM_CTL (PWM_BASE + 0)
#define PWM_RNG2 (PWM_BASE + 8)                                    /* チャンネル2用 (0x20 / 4 = 8) */
#define PWM_DAT2 (PWM_BASE + 9) /* チャンネル2用 (0x24 / 4 = 9) */ /* DAT2レジスタ (変位 0x24 -> 36/4=9) [cite: 130] */

#define PWM_HZ (9600 * 1000)  // PWM装置のクロック周波数
#define KEY_A4 (PWM_HZ / 440) // 440Hzの1周期分のクロック数
#define KEY_B4 (PWM_HZ / 494) // シ (494Hz)
#define KEY_C5 (PWM_HZ / 523) // ド (523Hz)
#define KEY_D5 (PWM_HZ / 587) // レ (587Hz)
#define KEY_E5 (PWM_HZ / 659) // ミ (659Hz)
#define KEY_F5 (PWM_HZ / 698) // ファ (698Hz)
#define KEY_G5 (PWM_HZ / 784) // ソ (784Hz)
#define KEY_A5 (PWM_HZ / 880) // ラ (880Hz)
