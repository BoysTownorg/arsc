#include "arsc_asio_tests.h"
#define ASIO
#include <arsc_asio.h>

static int32_t(*ar_asio_devices_restore)();

static void setup(void) {
	ar_asio_devices_restore = ar_asio_devices;
}

static void teardown(void) {
	ar_asio_devices = ar_asio_devices_restore;
}

static int32_t devices;

static int32_t ar_asio_devices_stub() {
	return devices;
}

#define ASSERT_EQUAL_INT(a, b) ck_assert_int_eq(a, b)

START_TEST(ar_asio_bind_returns_number_of_devices) {
	ar_asio_devices = ar_asio_devices_stub;
	devices = 3;
	ASSERT_EQUAL_INT(3, _ar_asio_bind(0, 0));
}
END_TEST

Suite* arsc_asio_test_suite() {
	Suite* suite = suite_create("arsc_asio");
	TCase* test_case = tcase_create("idk");
	tcase_add_checked_fixture(test_case, setup, teardown);
	tcase_add_test(test_case, ar_asio_bind_returns_number_of_devices);
	suite_add_tcase(suite, test_case);
	return suite;
}