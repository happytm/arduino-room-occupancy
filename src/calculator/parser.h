#ifndef PARSER_H
#define PARSER_H


#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*valid_packet_cb)(void* data, uint32_t size);


uint8_t parser_init(valid_packet_cb callback, uint32_t size);
uint32_t parser_buffer_size();
void parser_parse(uint8_t data);
void parser_reset();

#ifdef __cplusplus
}
#endif

#endif // PARSER_H
