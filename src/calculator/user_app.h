#ifndef SRC_CALCULATOR_USER_APP_H_
#define SRC_CALCULATOR_USER_APP_H_

#include <stdint.h>

#include "protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	float    usSampleMin;
	float    usSampleMax;
	float    usSampleAvg;
}
CalculatorStats;


uint8_t app_init();
void app_reset();
uint32_t app_inputs_size();
uint32_t app_model_size();
ModelInfo app_model_info();
int app_dataset_info(DatasetInfo *info);
void app_reset_stats(CalculatorStats *stats);
CalculatorStats app_get_stats();
float* app_run_inference(float* sample, uint32_t size_in, uint32_t* size_out);

#ifdef __cplusplus
}
#endif

#endif /* SRC_CALCULATOR_USER_APP_H_ */
