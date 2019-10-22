#include "arsc_asio_tests_common.h"
#include "arsc_asio_open_device_tests.h"
#include <arsc_asio_wrappers.h>
#include <stdlib.h>

static int32_t(*list_rates_restore)(int32_t);
static int32_t(*pLockAndLoadRestore)(int32_t);
static bool(*SDKAsioSetSampleRateRestore)(ASIOSampleRate);
static bool (*SDKAsioGetBufferSizeRestore)(
	long* alngMinBufferSize,
	long* alngMaxBufferSize,
	long* aslngPreferredBufferSize,
	long* alngGranularity
);
static bool (*SDKAsioCreateBuffersRestore)(
	ASIOBufferInfo* bufferInfos,
	long numChannels,
	long bufferSize,
	ASIOCallbacks* callbacks
);
static bool (*SDKAsioGetLatenciesRestore)(long* inputLatency, long* outputLatency);

static int32_t rates;
static int32_t list_rates_device;

static int32_t list_rates_stub(int32_t n) {
	list_rates_device = n;
	return rates;
}

static bool SDKAsioSetSampleRateStub(ASIOSampleRate r) {
	r;
	return 1;
}

static bool SDKAsioGetBufferSizeStub(
	long* alngMinBufferSize,
	long* alngMaxBufferSize,
	long* aslngPreferredBufferSize,
	long* alngGranularity
) {
	alngMinBufferSize;
	alngMaxBufferSize;
	aslngPreferredBufferSize;
	alngGranularity;
	return 1;
}

static int32_t pLockAndLoadStub(int32_t device) {
	device;
	return 1;
}

static bool SDKAsioCreateBuffersStub(
	ASIOBufferInfo* bufferInfo,
	long numChannels,
	long bufferSize,
	ASIOCallbacks* callbacks
) {
	bufferInfo;
	numChannels;
	bufferSize;
	callbacks;
	return 1;
}

static bool SDKAsioGetLatenciesStub(long* inputLatency, long* outputLatency) {
	inputLatency;
	outputLatency;
	return 1;
}

static void setup(void) {
	allocate_device(0);
	allocate_device(1);
	assign_device_input_channels(0, -2);
	assign_device_input_channels(1, -2);
	assign_device_output_channels(0, -2);
	assign_device_output_channels(1, -2);
	list_rates_restore = ar_asio_list_rates;
	pLockAndLoadRestore = pLockAndLoad;
	SDKAsioSetSampleRateRestore = SDKAsioSetSampleRate;
	SDKAsioGetBufferSizeRestore = SDKAsioGetBufferSize;
	SDKAsioCreateBuffersRestore = SDKAsioCreateBuffers;
	SDKAsioGetLatenciesRestore = SDKAsioGetLatencies;
	ar_asio_list_rates = list_rates_stub;
	pLockAndLoad = pLockAndLoadStub;
	SDKAsioSetSampleRate = SDKAsioSetSampleRateStub;
	SDKAsioGetBufferSize = SDKAsioGetBufferSizeStub;
	SDKAsioCreateBuffers = SDKAsioCreateBuffersStub;
	SDKAsioGetLatencies = SDKAsioGetLatenciesStub;
}

static void teardown(void) {
	ar_asio_list_rates = list_rates_restore;
	pLockAndLoad = pLockAndLoadRestore;
	SDKAsioSetSampleRate = SDKAsioSetSampleRateRestore;
	SDKAsioGetBufferSize = SDKAsioGetBufferSizeRestore;
	SDKAsioCreateBuffers = SDKAsioCreateBuffersRestore;
	SDKAsioGetLatencies = SDKAsioGetLatenciesRestore;
	free_device(0);
	free_device(1);
}

static int32_t open_device(int32_t n) {
	return _ar_asio_open(n);
}

static int32_t open() {
	return open_device(0);
}

static ASIOBufferInfo* bufferInfo_(int i) {
	return bufferInfos + i;
}

static ASIOBool bufferInfoIsInput(int i) {
	return bufferInfo_(i)->isInput;
}

static long bufferInfoChannelNumber(int i) {
	return bufferInfo_(i)->channelNum;
}

#define ASSERT_EQUAL_INT(a, b) ck_assert_int_eq(a, b)

#define ASSERT_BUFFER_INFO_IS_INPUT_EQUALS(a, b) ASSERT_EQUAL_ANY(a, bufferInfoIsInput(b))

#define ASSERT_BUFFER_INFO_IS_INPUT_FOR_DEVICE_RANGE(a, b)\
for (int i = a; i < b; ++i)\
	ASSERT_BUFFER_INFO_IS_INPUT_EQUALS(ASIOFalse, i)

#define ASSERT_BUFFER_INFO_IS_OUTPUT_FOR_DEVICE_RANGE(a, b)\
for (int i = a; i < b; ++i)\
	ASSERT_BUFFER_INFO_IS_INPUT_EQUALS(ASIOTrue, i)

#define ASSERT_BUFFER_INFO_CHANNEL_NUMBER(a, b) ASSERT_EQUAL_ANY(a, bufferInfoChannelNumber(b))

START_TEST(open_returns_zero_on_success) {
	ASSERT_EQUAL_ANY(0, open());
}

START_TEST(open_assigns_good_sample_rates) {
	rates = 1;
	open();
	ASSERT_EQUAL_ANY(1, devices(0)->gdsr);
}

START_TEST(open_passes_device_to_list_rates) {
	open_device(1);
	ASSERT_EQUAL_ANY(1, list_rates_device);
}

START_TEST(open_initializes_buffer_infos) {
	set_device_desired_output_channels(1, 2);
	set_device_desired_input_channels(1, 3);
	open_device(1);
	ASSERT_BUFFER_INFO_IS_INPUT_FOR_DEVICE_RANGE(0, 2);
	ASSERT_BUFFER_INFO_IS_OUTPUT_FOR_DEVICE_RANGE(2, 2 + 3);
	ASSERT_BUFFER_INFO_CHANNEL_NUMBER(0, 0);
	ASSERT_BUFFER_INFO_CHANNEL_NUMBER(1, 1);
	ASSERT_BUFFER_INFO_CHANNEL_NUMBER(0, 2);
	ASSERT_BUFFER_INFO_CHANNEL_NUMBER(1, 3);
	ASSERT_BUFFER_INFO_CHANNEL_NUMBER(2, 4);
}

Suite* arsc_asio_open_device_suite() {
	Suite* suite = suite_create("arsc_asio_open_device");
	TCase* test_case = tcase_create("open_device");
	tcase_add_checked_fixture(test_case, setup, teardown);
	add_test(test_case, open_returns_zero_on_success);
	add_test(test_case, open_assigns_good_sample_rates);
	add_test(test_case, open_passes_device_to_list_rates);
	add_test(test_case, open_initializes_buffer_infos);
	suite_add_tcase(suite, test_case);
	return suite;
}