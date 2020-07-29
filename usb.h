#ifndef __USB_H
#define __USB_H

#include <stdint.h>

void usbDevInit();
void usbSetKeycode(uint8_t i, uint8_t key);
void usbReleaseAll();
void usbPushKeydata();

void usbHIDTestSend(uint8_t i);

#endif
