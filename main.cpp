/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include "mbed.h"
#include "mbed_stats.h"

#include "drivers/USBHID.h"

// Declare a USBHID device
USBHID HID(8, 8, 0x1234, 0x0006, 0x0001, true);

HID_REPORT output_report = {
    .length = 8,
    .data = {0}
};
HID_REPORT input_report = {
    .length = 0,
    .data = {0}
};

DigitalOut led_out(LED1);

int main(void)
{
    while (1) {

        // Fill the report
        for (int i = 0; i < output_report.length; i++) {
            output_report.data[i] = rand() & UINT8_MAX;
        }

        // Send the report
        HID.send(&output_report);

        // Try to read a msg
        if (HID.read_nb(&input_report)) {
            led_out = !led_out;
            for (int i = 0; i < input_report.length; i++) {
                printf("%d ", input_report.data[i]);
            }
            printf("\r\n");
        }

        ThisThread::sleep_for(1000);

        mbed_stats_cpu_t stats;
        mbed_stats_cpu_get(&stats);
        printf("Uptime: %llu ", stats.uptime / 1000);
        printf("Sleep time: %llu ", stats.sleep_time / 1000);
        printf("Deep Sleep: %llu\n\r", stats.deep_sleep_time / 1000);
    }
}
