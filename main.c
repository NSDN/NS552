#include "ch552.h"
#include "sys.h"
#include "usb.h"

void __usbDeviceInterrupt() __interrupt (INT_NO_USB) __using (1);
extern uint8_t FLAG, Ready;

__sbit __at (0xB3) LEDA;
__sbit __at (0xB4) LEDB;

__sbit __at (0x91) IKY;
__sbit __at (0xB0) EKY;

volatile __bit control = 1;

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

    FLAG = 0;
    Ready = 0;

    LEDA = 1; LEDB = 1;
    uint8_t count = 0;
    while (!Ready) {
        delay(100);
        count += 1;
        if (count > 10)
            break;
    }
    LEDA = 1; LEDB = 0;
    usbReleaseAll();
    usbPushKeydata();
    delay(100);
    LEDA = 0; LEDB = 1;
    delay(100);
    LEDA = 1; LEDB = 0;

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
