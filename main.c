#include "ch552.h"
#include "sys.h"
#include "usb.h"

#include <string.h>

void __usbDeviceInterrupt() __interrupt (INT_NO_USB) __using (1);
extern uint8_t FLAG, Ready;

__sbit __at (0xB3) LEDA;
__sbit __at (0xB4) LEDB;

__sbit __at (0x91) IKY;
__sbit __at (0xB0) EKY;

volatile __bit control = 1;
volatile uint8_x __at (0x0000) xRAM[0x400];

void main() {
    P1_MOD_OC &= ~(0x02); P1_DIR_PU &= ~(0x02);
    P3_MOD_OC &= ~(0x01); P3_DIR_PU &= ~(0x01);
    LEDA = 0; LEDB = 0;
    sysClockConfig();
    delay(5);

    usbDevInit();
    EA = 1;
    UEP1_T_LEN = 0;
    UEP2_T_LEN = 0;
    UEP3_T_LEN = 0;

    FLAG = 1;

    LEDA = 0; LEDB = 1;
    memset(xRAM, 0x00, sizeof(xRAM));
    delay(1000);
    LEDA = 1; LEDB = 0;
    usbReleaseAll();
    usbPushKeydata();

    while (1) {
        if (IKY == 0) {
            while (IKY == 0);
            control = !control;
            usbReleaseAll();
            usbPushKeydata();
        }
        
        if (control) {
            usbSetKeycode(2, EKY != 0 ? 0 : 44); // KEY_SPACE
            usbPushKeydata();
            delay(1);
        }

        LEDB = control;
    }
    
}
