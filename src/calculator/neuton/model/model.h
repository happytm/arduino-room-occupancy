#ifndef NEUTON_MODEL_MODEL_H
#define NEUTON_MODEL_MODEL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Model info */
#define NEUTON_MODEL_HEADER_VERSION 3
#define NEUTON_MODEL_QLEVEL 8
#define NEUTON_MODEL_FLOAT_SUPPORT 0
#define NEUTON_MODEL_TASK_TYPE 1  // binary classification
#define NEUTON_MODEL_NEURONS_COUNT 9
#define NEUTON_MODEL_WEIGHTS_COUNT 51
#define NEUTON_MODEL_INPUTS_COUNT 5
#define NEUTON_MODEL_INPUT_LIMITS_COUNT 5
#define NEUTON_MODEL_OUTPUTS_COUNT 2
#define NEUTON_MODEL_LOG_SCALE_OUTPUTS 0
#define NEUTON_MODEL_HAS_CLASSES_RATIO 0
#define NEUTON_MODEL_HAS_NEGPOS_RATIO 0

/* Preprocessing */
#define NEUTON_PREPROCESSING_ENABLED 0
#define NEUTON_DROP_ORIGINAL_FEATURES 0
#define NEUTON_BITMASK_ENABLED 1
#define NEUTON_INPUTS_IS_INTEGER 0
#define NEUTON_MODEL_SA_PRECISION 24

/* Types */
typedef float input_t;
typedef float extracted_feature_t;
typedef uint8_t coeff_t;
typedef int8_t weight_t;
typedef int32_t acc_signed_t;
typedef uint32_t acc_unsigned_t;
typedef uint8_t sources_size_t;
typedef uint8_t weights_size_t;
typedef uint8_t neurons_size_t;

/* Limits */
static const input_t modelInputMin[] = {
	19.1, 16.745001, 0, 412.75, 0.0028686963 };
static const input_t modelInputMax[] = {
	23.18, 39.1175, 732.75, 2028.5, 0.0064760135 };

static const uint8_t modelUsedInputsMask[] = { 0x1f };

/* Structure */
static const weight_t modelWeights[] = {
	-7, -103, -126, -81, 109, 71, 91, -37, 38, -46, 58, -31, -1, -125, 74,
	98, -128, 13, -128, -116, -128, 52, -35, 8, -128, 121, -12, -112, 116,
	-78, 20, -128, 8, -11, 10, -18, 103, -23, 113, -25, -36, -8, -128, 108,
	39, -79, -84, -67, 126, -122, 44 };

static const sources_size_t modelLinks[] = {
	0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 1, 3, 4, 5, 0, 1, 2, 0, 1, 5,
	0, 1, 2, 3, 4, 5, 3, 4, 0, 3, 4, 5, 0, 2, 5, 5, 0, 1, 4, 1, 3, 5, 1, 3,
	4, 7, 5 };

static const weights_size_t modelIntLinksBoundaries[] = {
	0, 7, 14, 21, 25, 32, 39, 43, 50 };
static const weights_size_t modelExtLinksBoundaries[] = {
	6, 12, 18, 24, 30, 36, 40, 46, 51 };

static const coeff_t modelFuncCoeffs[] = {
	160, 69, 145, 158, 157, 126, 78, 157, 90 };
static const uint8_t modelFuncTypes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static const neurons_size_t modelOutputNeurons[] = { 6, 8 };

#ifdef __cplusplus
}
#endif

#endif // NEUTON_MODEL_MODEL_H

