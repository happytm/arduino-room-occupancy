#ifndef CHECKSUM_H
#define CHECKSUM_H


#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t crc16_table(uint8_t* btData, uint32_t wLen, uint16_t prev);

#ifdef __cplusplus
}
#endif

#endif // CHECKSUM_H
