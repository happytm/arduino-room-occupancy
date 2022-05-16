#include <string.h>

#include "src/calculator/user_app.h"
#include "src/calculator/protocol.h"
#include "src/calculator/parser.h"
#include "src/calculator/checksum.h"
#include "src/calculator/neuton/neuton.h"

#define TURN_OFF_NEUTON

#define BELL_PIN 11

static bool initialised = 0;

static int channel_make_packet(PacketHeader* hdr, void* payload, size_t size, PacketType type, ErrorCode err)
{
  if (!hdr || ((size + sizeof(PacketHeader) + sizeof(uint16_t)) > (parser_buffer_size())))
    return 1;

  if (size && !payload)
    return 2;

  hdr->preamble = PREAMBLE;
  hdr->type = ANS(type);
  hdr->error = err;
  hdr->size = sizeof(PacketHeader) + size + sizeof(uint16_t);

  if (size)
    memcpy(hdr + 1, payload, size);

  uint16_t crc = crc16_table((uint8_t*) hdr, hdr->size - sizeof(uint16_t), 0);
  memcpy((uint8_t*) hdr + hdr->size - sizeof(uint16_t), &crc, sizeof(uint16_t));

  return 0;
}

static inline void channel_send_packet(void* data, size_t size)
{
  Serial.write((const char*) data, size);
}


static void channel_on_valid_packet(void* data, uint32_t)
{
  PacketHeader* hdr = (PacketHeader*) data;

  if (IS_ANS(hdr->type))
    return;

  hdr->size -= sizeof(PacketHeader) + sizeof(uint16_t);

  switch (PACKET_TYPE(hdr->type))
  {
  case TYPE_MODEL_INFO:
  {
    ModelInfo info = app_model_info();

    if (0 == channel_make_packet(hdr, &info, sizeof(info), TYPE_MODEL_INFO, ERROR_SUCCESS))
      channel_send_packet(data, hdr->size);

    app_reset();

    break;
  }

  case TYPE_DATASET_INFO:
  {
    if (hdr->size < sizeof(DatasetInfo))
      break;

    DatasetInfo* info = (DatasetInfo*) (hdr + 1);

    if (0 == app_dataset_info(info))
    {
      if (0 == channel_make_packet(hdr, NULL, 0, TYPE_DATASET_INFO, ERROR_SUCCESS))
        channel_send_packet(data, hdr->size);
    }
    else
    {
      if (0 == channel_make_packet(hdr, NULL, 0, TYPE_ERROR, ERROR_INVALID_SIZE))
        channel_send_packet(data, hdr->size);
    }

    app_reset();

    break;
  }

  case TYPE_DATASET_SAMPLE:
  {
    float* sample = (float*) (hdr + 1);

    uint32_t sz = 0;
    float* result = app_run_inference(sample, hdr->size, &sz);
    if (result && sz)
    {
      if (0 == channel_make_packet(hdr, result, sz, TYPE_DATASET_SAMPLE, ERROR_SUCCESS))
        channel_send_packet(data, hdr->size);
    }
    else
    {
      if (0 == channel_make_packet(hdr, NULL, 0, TYPE_ERROR, result ? ERROR_INVALID_SIZE : ERROR_NO_MEMORY))
        channel_send_packet(data, hdr->size);
    }

    break;
  }

  case TYPE_PERF_REPORT:
  {
    PerformanceReport report;

    report.ramUsage = neuton_model_ram_usage();
    report.ramUsageCur = neuton_model_ram_usage();
    report.bufferSize = parser_buffer_size();
    report.flashUsage = app_model_size();
    report.freq = F_CPU;

    CalculatorStats stats = app_get_stats();

    report.usSampleAvg = stats.usSampleAvg;
    report.usSampleMin = stats.usSampleMin;
    report.usSampleMax = stats.usSampleMax;

    if (0 == channel_make_packet(hdr, &report, sizeof(report), TYPE_PERF_REPORT, ERROR_SUCCESS))
      channel_send_packet(data, hdr->size);

    break;
  }

  case TYPE_ERROR:
  default:
    break;
  }
}


#ifndef TURN_OFF_NEUTON
void channel_init(uint32_t size)
{
  initialised = (0 == parser_init(channel_on_valid_packet, size));
}
#endif

extern "C" void bell_ON(uint32_t Freq)
{
  tone(BELL_PIN, Freq);
}

extern "C" void bell_OFF(void)
{
  noTone(BELL_PIN);
}

void setup()
{
  bell_OFF();

#ifndef TURN_OFF_NEUTON  
  initialised = (0 == app_init());
  initialised &= (0 == parser_init(channel_on_valid_packet, app_inputs_size()));
#endif

  Serial.begin(230400);
}

void loop()
{
  if (!initialised)
  {
    while(1)
    {
      bell_ON(500);
      delay(100);
      bell_OFF();
      delay(100);
    }
  }

#ifndef TURN_OFF_NEUTON  
  while (Serial.available() > 0)
    parser_parse(Serial.read());
#endif
}
