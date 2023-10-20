#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>

int observer_start(void);

int main(void)
{
	int err;

	printk("Starting Observer Demo\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}

	(void)observer_start();

	printk("Exiting %s thread.\n", __func__);
	return 0;
}