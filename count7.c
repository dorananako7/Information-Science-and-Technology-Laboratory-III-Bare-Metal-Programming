#include "const.h"     

const int col_ports[8] = {COL1_PORT, COL2_PORT, COL3_PORT, COL4_PORT, COL5_PORT, COL6_PORT, COL7_PORT, COL8_PORT};
const int row_ports[8] = {ROW1_PORT, ROW2_PORT, ROW3_PORT, ROW4_PORT, ROW5_PORT, ROW6_PORT, ROW7_PORT, ROW8_PORT};

const unsigned char font_numeric[10][8] = {
    { 0b01100000, 0b10010000, 0b10010000, 0b10010000, 0b10010000, 0b10010000, 0b01100000, 0b00000000 },
    { 0b01000000, 0b11000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b11100000, 0b00000000 },
    { 0b01100000, 0b10010000, 0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b11110000, 0b00000000 },
    { 0b11110000, 0b00010000, 0b00100000, 0b00010000, 0b00010000, 0b10010000, 0b01100000, 0b00000000 },
    { 0b10010000, 0b10010000, 0b10010000, 0b11110000, 0b00010000, 0b00010000, 0b00010000, 0b00000000 },
    { 0b11110000, 0b10000000, 0b11100000, 0b00010000, 0b00010000, 0b10010000, 0b01100000, 0b00000000 },
    { 0b01100000, 0b10000000, 0b10000000, 0b11100000, 0b10010000, 0b10010000, 0b01100000, 0b00000000 },
    { 0b11110000, 0b10010000, 0b00010000, 0b00100000, 0b01000000, 0b01000000, 0b01000000, 0b00000000 },
    { 0b01100000, 0b10010000, 0b10010000, 0b01100000, 0b10010000, 0b10010000, 0b01100000, 0b00000000 },
    { 0b01100000, 0b10010000, 0b10010000, 0b01110000, 0b00010000, 0b00010000, 0b01100000, 0b00000000 }
};

char frame_buffer[8];

void delay(volatile int count) {
    while (count > 0) {
        count--;
    }
}

void write_to_frame_buffer(int x) {
    int tens = x / 10;
    int ones = x % 10;
    int row;

    for (row = 0; row < 8; row++) {
        frame_buffer[row] = font_numeric[tens][row] | (font_numeric[ones][row] >> 4);
    }
}

int main(void) {
    int row, col, i;
    int x = 0;

    *GPFSEL0 = GPFSEL_VEC0;
    *GPFSEL1 = GPFSEL_VEC1;
    *GPFSEL2 = GPFSEL_VEC2;

    while (1) {
        write_to_frame_buffer(x);
        x = (x + 7) % 100;

        for (i = 0; i < 60; i++) {
            for (row = 0; row < 8; row++) {
                for (col = 0; col < 8; col++) {
                    int bit_state = (frame_buffer[row] >> (7 - col)) & 0x01;
                    if (bit_state == 1) {
                        *GPSET0 = (1 << col_ports[col]);
                    } else {
                        *GPCLR0 = (1 << col_ports[col]);
                    }
                }

                *GPCLR0 = (1 << row_ports[row]);
                delay(2000);
                *GPSET0 = (1 << row_ports[row]);
            }
        }
    }

    return 0;
}
