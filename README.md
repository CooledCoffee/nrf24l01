Introduction
============

nRF24L01+ library for C51. This library is optimized not for speed but for ease of use.

APIs
====

* bool nrf_init()

	Initializes the nRF24l01+ module. Returns TRUE on success, otherwise FALSE.

* bool nrf_send(byte* addr, byte* buffer)

	Sends data to a specified address. Returns TRUE on success, otherwise FALSE.

* bool nrf_recv(byte* addr, byte* buffer, int timeout)

	Listen on a specified address and returns received data. Pass -1 for timeout if you want to wait indefinitely. Returns TRUE if data received, otherwise FALSE.

Usage
=====

In your main.c file, include the nrf.h in this way:

	#define CE P1_4
	#define CSN P1_3
	#define SCK P1_2
	#define MOSI P1_1
	#define MISO P1_0
	#include "nrf.h"

Change the defines according to your mcu pins.

Please check out the examples for detail usage.

Compatibility
=============

This library is tested on STC89C52RC2 and should be compatible with AT89S52 and similar mcus. It assumes 11.0952 MHz crystal and 12 clocks per instruction. If you mcu is different from this configuration, you will need to replace the \_nrf\_sleep function with your own implementation.

Author
======

Mengchen LEE: <a href="https://plus.google.com/117704742936410336204" target="_blank">Google Plus</a>, <a href="https://cn.linkedin.com/pub/mengchen-lee/30/8/23a" target="_blank">LinkedIn</a>

