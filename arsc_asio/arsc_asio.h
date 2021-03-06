#ifndef ARSC_ARSC_ASIO_ARSC_ASIO_H_
#define ARSC_ARSC_ASIO_ARSC_ASIO_H_

#include "../arscdev.h"

/*
Channels are handled differently in ASIO than MME, so made sense to
map the "segments" passed from the calling program in the format
Steve set up to an array of these structures.

32-bit integers is what all of the sound cards tested expect. 
*/
typedef struct {
	int32_t* data;				    // Pointer to segment
	int32_t size;				    // # of samples, e.g. 2048, in this block
	int32_t Index;					    // How many samples have already been played
	int32_t channel;				    // 0, 1, 2, . . . 
	int32_t segment;				    // Current segment for this channel
} ArAsioOutputAudio;

typedef struct {
	int32_t* data;
	int32_t size;
	int32_t Index;
	int32_t channel;
	int32_t segment;
	int32_t SkippedSamples; // Skipped samples before Latency kicks in
	int LatencyReached; // For each channel, but only segment 0
} ArAsioInputAudio;

extern int32_t (*ar_asio_devices)();
extern char* (*ar_asio_device_name)(int32_t);
extern void (*ar_asio_io_stop)(int32_t);
extern void (*ar_asio_close)(int32_t);
extern int32_t(*ar_asio_open)(int32_t);
extern int32_t(*ar_asio_io_prepare)(int32_t);
extern void (*ar_asio_io_start)(int32_t);
extern int32_t(*ar_asio_transfer_segment)(int32_t, int32_t);
extern int32_t(*ar_asio_check_segment)(int32_t, int32_t);
extern int32_t(*ar_asio_latency)(int32_t, int32_t);
extern int32_t(*ar_asio_list_rates)(int32_t);
extern int32_t (*pLockAndLoad)(int32_t aintDevice);
extern ArAsioOutputAudio* global_ar_asio_output_audio;
extern ArAsioInputAudio* global_ar_asio_input_audio;
extern ARDEV* global_ar_asio_current_device;

int32_t _ar_asio_open(int32_t);
int32_t _ar_asio_io_prepare(int32_t);
int32_t _ar_asio_chk_seg(int32_t di, int32_t b);
int32_t ar_asio_write_device_buffer(int32_t* buffer, int32_t size, ArAsioOutputAudio* audio);
int32_t ar_asio_read_device_buffer(int32_t* buffer, int32_t size, ArAsioInputAudio* audio);
void _ar_asio_io_start(int32_t di);

#endif
