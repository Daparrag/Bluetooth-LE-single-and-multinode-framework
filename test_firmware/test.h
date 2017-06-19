/*firware_test
*this file include a simple test for the network an the aplication moudes
*
*/

#include <app_ble.h>
#include <network.h>

typedef enum TEST{
	TEST_SUCCESS=0x0,
	TEST_FAIL=0x1
}Rtest_t;


Rtest_t app_module_test(void);
Rtest_t chandler_module_test_central(void);
Rtest_t chandler_module_test_pherispheral(void);

Rtest_t global_test_central(void);
Rtest_t global_test_pherispheral(void);
Rtest_t global_test_broadcaster(void);
Rtest_t global_test_observer(void);


