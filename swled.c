#include "const.h"

int main() {
    //GPIO 10を出力用に
    *GPFSEL1 = 1;
    
    int i;
    while(1) {
        //スイッチ1を押したとき
        if((*GPLEV0 & (1 << SW1_PORT)) != 0) {
            //点灯(0.5秒)
            *GPSET0 = (1 << LED_PORT);
            for(i = 0; i < 1380000; i++);
            //消灯
            *GPCLR0 = (1 << LED_PORT);
            for(i = 0; i < 345000; i++);
        }
        //スイッチ2を押したとき
        else if((*GPLEV0 & (1 << SW2_PORT)) != 0) {
            //点灯(3秒)
            *GPSET0 = (1 << LED_PORT);
            for(i = 0; i < 2070000; i++);
            //消灯
            *GPCLR0 = (1 << LED_PORT);
              for(i = 0; i < 345000; i++);
        }
        //スイッチ3を押したとき
        else if((*GPLEV0 & (1 << SW3_PORT)) != 0) {
            //点灯(6秒)
            *GPSET0 = (1 << LED_PORT);
            for(i = 0; i < 4140000; i++);
            //消灯
            *GPCLR0 = (1 << LED_PORT);
              for(i = 0; i < 345000; i++);
        }
        //スイッチ4を押したとき
        else if((*GPLEV0 & (1 << SW4_PORT)) != 0) {
            //点灯(9秒)
            *GPSET0 = (1 << LED_PORT);
            for(i = 0; i < 6210000; i++);
            //消灯
            *GPCLR0 = (1 << LED_PORT);
             for(i = 0; i < 345000; i++);
        }
    }
    
}