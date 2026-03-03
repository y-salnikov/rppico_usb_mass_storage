/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "bsp/board.h"
#include "hardware/uart.h"
#include "tusb.h"
#include "pico_hal.h"
#include "memfind.h"

#define DISK_BLOCK_SIZE  512
#define MOUNT_WAIT 1.0

extern uint8_t msc_disk[][DISK_BLOCK_SIZE];
extern uint8_t mounted;
extern int img_file;
void msc_flush_reopen(void);


//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

int USB_WAIT=0;
uint32_t flash_size;



void mounted_task(void)
{
	static uint8_t old_mounted;
	static uint8_t status;

	if(mounted!=old_mounted)
	{
		old_mounted=mounted;
		hal_start();
		status=1;
	}

	if(status==1)
	{
		if(hal_elapsed()>=MOUNT_WAIT)
		{
			gpio_put(PICO_DEFAULT_LED_PIN, mounted);
			status=0;
			if (mounted==0)
			{
				msc_flush_reopen();
			}
		}
}
}


/*------------- MAIN -------------*/
int main(void)
{
	int s=0;
  board_init();
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  flash_size=storage_get_flash_capacity();


  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);



  while (1)
  {
    tud_task(); // tinyusb device task
	mounted_task();

    if (s==500000)
    {
	#ifdef DBG
		printf("\033[H\033[J");
		printf("/-------------------------------------/\n");
		printf("tud_ready:%b\nflash size:%d\n",tud_ready(),flash_size);
	#endif
		USB_WAIT=1;

	}
	if (s<500001) s++;

  }

  return 0;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
	//printf("mount cb\n");   не работает
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
//	printf("umount cb\n"); не работает
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{

}

