#include <8051.h>
#include "../common.h"
#include "../stc89.h"
#include "../nrf.h"

byte ADDR_TEST_MASTER[] = {0x02, 0x02, 0x02, 0x02, 0x01};
byte ADDR_TEST_SLAVE[] = {0x02, 0x02, 0x02, 0x02, 0x02};

void main() {
    byte counter;
    byte buffer[NRF_PAYLOAD_LEN];
    
    sleep(1000);
    if (!nrf_init()) {
        P0 = _nrf_get_reg(STATUS);
        return;
    }

    counter = 0;
    while (TRUE) {
        sleep(500);
        counter = (counter + 1) % 8;
        buffer[0] = ~(1 << counter);
        if (!nrf_send(ADDR_TEST_SLAVE, buffer)) {
            P0 = 0x55;
            continue;
        }
        if (!nrf_recv(ADDR_TEST_MASTER, buffer, 0)) {
            P0 = 0xaa;
            continue;
        }
        P0 = buffer[0];
    }
}
