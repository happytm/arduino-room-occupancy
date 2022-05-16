#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// Byte order: little endian
// Packet structure:
// - header
// - data (optional, depends on packet type)
// - crc16 (Xmodem)
//


typedef struct
{
	uint16_t preamble;      // Must be equal PREAMBLE macro
	uint16_t size;          // Size including checksum
	uint8_t  type;          // Packet type: for answers MSB should be set
	uint8_t  error;         // Error code
	uint8_t  reserved[2];
}
PacketHeader;


#define PREAMBLE          (0x55AA)
#define ANS(type)         ((type) |  (1u<<7))
#define IS_ANS(type)      ((type) &  (1u<<7))
#define PACKET_TYPE(type) ((type) & ~(1u<<7))


typedef enum
{
	TYPE_ERROR = 0,
	TYPE_MODEL_INFO,
	TYPE_DATASET_INFO,
	TYPE_DATASET_SAMPLE,
	TYPE_PERF_REPORT,
}
PacketType;


typedef enum
{
	ERROR_SUCCESS = 0,
	ERROR_INVALID_SIZE,
	ERROR_NO_MEMORY,
	ERROR_SEND_AGAIN
}
ErrorCode;


typedef struct
{
	uint16_t rowsCount;         // Rows count in dataset sample (with BIAS)
	uint16_t reverseByteOrder;  // Reverse bytes order flag
}
DatasetInfo;


typedef struct
{
	uint16_t rowsCount;         // Rows count in result
	uint16_t taskType;          // Task type
}
ModelInfo;


typedef struct
{
	float usSampleMin;      // Minimum calculating time per sample
	float usSampleMax;      // Maximum calculating time per sample
	float usSampleAvg;      // Average calculating time per sample
	uint32_t ramUsageCur;   // Current RAM usage, bytes
	uint32_t ramUsage;      // Total RAM usage, bytes
	uint32_t bufferSize;    // IO buffer size, bytes
	uint32_t flashUsage;    // Model size in flash, bytes
	uint32_t freq;          // CPU frequency, Hz
}
PerformanceReport;

#ifdef __cplusplus
}
#endif

#endif // PROTOCOL_H
