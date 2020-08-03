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
volatile uint8_t prevKey = 0;

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

    LEDA = 1; LEDB = 0;
    delay(500);
    LEDA = 0; LEDB = 1;
    usbReleaseAll();
    usbPushKeydata();
    requestHIDData();

    while (1) {
        if (IKY == 0) {
            while (IKY == 0);
            usbReleaseAll();
            usbSetKeycode(0, 1);                        // Report ID 1
            usbSetKeycode(9, 41);                       // KEY_ESC
            usbPushKeydata();
            delay(100);
            usbReleaseAll();
            usbSetKeycode(0, 1);                        // Report ID 1
            usbSetKeycode(9, 0);                        // KEY_ESC
            usbPushKeydata();
        }
        
        usbReleaseAll();
        usbSetKeycode(0, 1);                            // Report ID 1
        usbSetKeycode(1, 0);                            // NO CONTROL
        uint8_t val = 0;
        val |= (EKY != 0 ? 0 : 0x01);
        usbSetKeycode(2, EKY != 0 ? 0 : 44);            // KEY_SPACE
        if (val != prevKey) {
            prevKey = val;
            usbPushKeydata();
        }
    }
    
}
