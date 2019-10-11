#include "arsc_asio_tests_common.h"

ARDEV** devices_(int32_t device) {
	return &_ardev[device];
}

ARDEV* devices(int32_t device) {
	return *devices_(device);
}

void free_device(int device) {
	free(devices(device));
}

void allocate_device(int device) {
	*devices_(device) = calloc(1, sizeof(ARDEV));
}

void assign_integer_array(int32_t* a, int i, int32_t what) {
	*integer_array_at(a, i) = what;
}

int32_t read_integer_array_at(int32_t* a, int i) {
	return *integer_array_at(a, i);
}

int32_t* integer_array_at(int32_t* a, int i) {
	return a + i;
}
