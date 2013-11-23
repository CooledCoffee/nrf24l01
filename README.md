nRF24L01+
=========

nRF24L01+ library for C51. This library is optimized not for speed but for ease of use. It provides only three functions:

* bool nrf_init()

	Initializes the nRF24l01+ module. Returns TRUE on success, otherwise FALSE.

* bool nrf_send(byte* addr, byte* buffer)

	Sends data to a specified address. Returns TRUE on success, otherwise FALSE.

* bool nrf_recv(byte* addr, byte* buffer, int timeout)

	Binds to a specified address and receives data. Specify -1 for timeout if you want to wait indefinitely. Returns TRUE if data received, otherwise FALSE.

Please check out the examples for detail usage.

This library is tested on STC89C52RC2 and should be compatible with AT89S52 and similar mcus. It assumes 11.0952 MHz crystal and 12 clocks per instruction. If you mcu is different from this configuration, you will need to provide your own sleep function in the stc89.h header file. You will also need to change these defines in nrf.h according to your own situation:

	#define CE P1_4
	#define CSN P1_3
	#define SCK P1_2
	#define MOSI P1_1
	#define MISO P1_0
