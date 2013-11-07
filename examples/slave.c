#include <8051.h>
#include "../common.h"
#include "../stc89.h"
#include "../nrf.h"

byte ADDR_TEST_MASTER[] = {0x02, 0x02, 0x02, 0x02, 0x01};
byte ADDR_TEST_SLAVE[] = {0x02, 0x02, 0x02, 0x02, 0x02};

void main() {
    byte buffer[NRF_PAYLOAD_LEN];

    sleep(1000);

    if (!nrf_init()) {
        P0 = _nrf_get_reg(STATUS);
        return;
    }

    while (TRUE) {
        nrf_recv(ADDR_TEST_SLAVE, buffer, 0);
        if (nrf_send(ADDR_TEST_MASTER, buffer))
            P0 = buffer[0];
        else
            P0 = 0x55;
    }
}
