#ifndef __NRF_H__
#define __NRF_H__
#include <8051.h>

#ifndef __BOOLEAN__
#define __BOOLEAN__
typedef char bool;
#define TRUE 1
#define FALSE 0
#endif

// Memory Map
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD       0x1C

// configuration register
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0

// enable auto acknowledgment
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0

// enable rx addresses
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0

// setup of address width
#define AW          0 // 2 bits

// setup of auto re-transmission
#define ARD         4 // 4 bits
#define ARC         0 // 4 bits

// RF setup register
#define PLL_LOCK    4
#define RF_DR       3
#define RF_PWR      1 // 2 bits   

// general status register
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1 // 3 bits
#define TX_FULL     0

// transmit observe register
#define PLOS_CNT    4 // 4 bits
#define ARC_CNT     0 // 4 bits

// fifo status
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0

// dynamic length
#define DPL_P0      0
#define DPL_P1      1
#define DPL_P2      2
#define DPL_P3      3
#define DPL_P4      4
#define DPL_P5      5

// Instruction Mnemonics
#define R_REGISTER    0x00 // last 4 bits will indicate reg. address
#define W_REGISTER    0x20 // last 4 bits will indicate reg. address
#define REGISTER_MASK 0x1F
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60

#define NRF_CONFIG ((1<<EN_CRC)|(0<<CRCO))
#define NRF_PAYLOAD_LEN 16

bool nrf_init();
bool nrf_send(char* addr, char* buffer);
bool nrf_recv(char* addr, char* buffer, int timeout);

bool _nrf_check_init();
void _nrf_config();
char _nrf_get_reg(char reg);
void _nrf_get_reg_mb(char reg, char* buffer, char len);
void _nrf_init_pins();
void _nrf_read_rx_payload(char* buffer);
char _nrf_rw(char value);
void _nrf_set_reg(char reg, char value);
void _nrf_set_reg_mb(char reg, char* buffer, char len);
void _nrf_sleep(int millis);
bool _nrf_wait_for_recv(int timeout);
bool _nrf_wait_for_send();
void _nrf_write_tx_payload(char* buffer);
bool _nrf_try_recv();

bool nrf_init() {
    _nrf_init_pins();
    _nrf_config();
    return _nrf_check_init();
}

bool nrf_recv(char* addr, char* buffer, int timeout) {
    bool success;
    _nrf_set_reg_mb(RX_ADDR_P0, addr, 5);
    _nrf_set_reg(CONFIG, NRF_CONFIG|((1<<PWR_UP)|(1<<PRIM_RX)));
    CSN = 0; {
        _nrf_rw(FLUSH_RX);
    } CSN = 1;
    CE = 1; {
        success = _nrf_wait_for_recv(timeout);
    } CE = 0;
    if (success)
        _nrf_read_rx_payload(buffer);
    _nrf_set_reg(STATUS, (1<<RX_DR));

    // sleep 100ms to avoid sending a response when the other peer is still retyring request
    _nrf_sleep(100);
    
    return success;
}

bool nrf_send(char* addr, char* buffer) {
    bool success;
    _nrf_set_reg_mb(TX_ADDR, addr, 5);
    _nrf_set_reg_mb(RX_ADDR_P0, addr, 5);
    _nrf_set_reg(CONFIG, NRF_CONFIG|((1<<PWR_UP)|(0<<PRIM_RX)));
    CSN = 0; {
        _nrf_rw(FLUSH_TX);
    } CSN = 1;
    CE = 1; {
        _nrf_write_tx_payload(buffer);
        success = _nrf_wait_for_send();
    } CE = 0;
    _nrf_set_reg(STATUS, (1<<TX_DS)|(1<<MAX_RT));
    return success;
}

bool _nrf_check_init() {
    char status;
    status = _nrf_get_reg(STATUS);
    if (status != 0x0e)
        return FALSE;
    status = _nrf_get_reg(FIFO_STATUS);
    if (status != 0x11)
        return FALSE;
    return TRUE;
}

void _nrf_config() {
    _nrf_set_reg(SETUP_RETR,(0x0f<<ARD)|(0x0f<<ARC)); // 15 retries with 4000 us delay
    _nrf_set_reg(RF_SETUP, (0<<RF_DR)|(3<<RF_PWR)); // 1 Mbps, 0 dBm
    _nrf_set_reg(EN_RXADDR, (1<<ERX_P0));
    _nrf_set_reg(RX_PW_P0, NRF_PAYLOAD_LEN);
    _nrf_set_reg(EN_AA, 1<<ENAA_P0);
}

char _nrf_get_reg(char reg) {
    char value;
    CSN = 0; {
        _nrf_rw(R_REGISTER | (REGISTER_MASK & reg));
        value = _nrf_rw(0);
    } CSN = 1;
    return value;
}

void _nrf_get_reg_mb(char reg, char* buffer, char len) {
    char i;
    CSN = 0; {
        _nrf_rw(R_REGISTER | (REGISTER_MASK & reg));
        for (i = 0; i < len; i++)
            buffer[i] = _nrf_rw(0);
    } CSN = 1;
}

void _nrf_init_pins() {
    CE = 0;
    CSN = 1;
}

void _nrf_read_rx_payload(char* buffer) {
    char i;
    CSN = 0; {
        _nrf_rw(R_RX_PAYLOAD);
        for (i = 0; i < NRF_PAYLOAD_LEN; i++)
            buffer[i] = _nrf_rw(0);
    } CSN = 1;
}

char _nrf_rw(char value) {
    char i;
    for (i = 0; i < 8; i++) {
        MOSI = (value & 0x80);
        value <<= 1;
        SCK = 1;
        value |= MISO;
        SCK = 0;
    }
    return value;
}

void _nrf_set_reg(char reg, char value) {
    CSN = 0; {
        _nrf_rw(W_REGISTER | (REGISTER_MASK & reg));
        _nrf_rw(value);
    } CSN = 1;
}

void _nrf_set_reg_mb(char reg, char* buffer, char len) {
    char i;
    CSN = 0; {
        _nrf_rw(W_REGISTER | (REGISTER_MASK & reg));
        for (i = 0; i < len; i++)
            _nrf_rw(buffer[i]);
    } CSN = 1;
}

bool _nrf_wait_for_recv(int timeout) {
    if (timeout < 0) {
        while (!_nrf_try_recv())
            _nrf_sleep(100);
        return TRUE;
    } else {
        while (timeout > 0) {
            if (_nrf_try_recv())
                return TRUE;
            timeout -= 100;
            _nrf_sleep(100);
        }
        return FALSE;
    }
}

bool _nrf_wait_for_send() {
    char status;
    while (TRUE) {
        status = _nrf_get_reg(STATUS);
        if (status & (1<<TX_DS))
            return TRUE;
        else if (status & (1<<MAX_RT))
            return FALSE;
    }
}

void _nrf_write_tx_payload(char* buffer) {
    char i;
    CSN = 0; {
        _nrf_rw(W_TX_PAYLOAD);
        for (i = 0; i < NRF_PAYLOAD_LEN; i++)
            _nrf_rw(buffer[i]);
    } CSN = 1;
}

bool _nrf_try_recv() {
    char status = _nrf_get_reg(STATUS);
    return status & (1<<RX_DR);
}

void _nrf_sleep(int millis) {
    /*
    We use the form "for (i = 0; i < 100; i++)" instead of "for (i = 100; i--;)".
    Because the 1st form is actually twice fast as the 2nd form after SDCC optimization.
    Note that this is compiler dependent.
    
    Also note that the 1st milli takes less loops because we have to take account of
    the overhead of the function call.
    This function takes 6 NOPs for entering and another 6 NOPs for return (the times
    may be different for other functions).
    */
    char i;
    for (i = 0; i < 100; i++) ;
    for (i = 0; i < 100; i++) ;
    for (i = 0; i < 100; i++) ;
    for (i = 0; i < 100; i++) ;
    for (i = 0; i < 85; i++) ;
    millis--;
    while (millis--) {
        for (i = 0; i < 100; i++) ;
        for (i = 0; i < 100; i++) ;
        for (i = 0; i < 100; i++) ;
        for (i = 0; i < 100; i++) ;
        for (i = 0; i < 91; i++) ;
    }
}

#endif
