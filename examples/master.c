#define CE P1_4
#define CSN P1_3
#define SCK P1_2
#define MOSI P1_1
#define MISO P1_0
#include "../nrf.h"

char ADDR_TEST_MASTER[] = {0x02, 0x02, 0x02, 0x02, 0x01};
char ADDR_TEST_SLAVE[] = {0x02, 0x02, 0x02, 0x02, 0x02};

void main() {
    int i;
    char failures;
    char buffer[NRF_PAYLOAD_LEN];
    
    _nrf_sleep(1000);
    if (!nrf_init()) {
        P0 = _nrf_get_reg(STATUS);
        return;
    }

    for (i = 0; i < 1000; i++) {
        if (!nrf_send(ADDR_TEST_SLAVE, buffer)) {
            failures++;
            P0 = failures;
            continue;
        }
        if (!nrf_recv(ADDR_TEST_MASTER, buffer, -1)) {
            failures++;
            P0 = failures;
            continue;
        }
    }
}
