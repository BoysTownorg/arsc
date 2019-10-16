#include "arsc_asio_bind_and_open_tests.h"
#include "arsc_asio_read_device_buffer_tests.h"
#include "arsc_asio_write_device_buffer_tests.h"
#include "arsc_asio_io_prepare_tests.h"

int main() {
	SRunner* runner = srunner_create(arsc_asio_test_suite());
	srunner_add_suite(runner, arsc_asio_read_device_buffer_suite());
	srunner_add_suite(runner, arsc_asio_write_device_buffer_suite());
	srunner_add_suite(runner, arsc_asio_io_prepare_suite());
	srunner_run_all(runner, CK_NORMAL);
	srunner_free(runner);
}
