#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <string.h>

// PSK
#define BROADCASTER_ISK "CS439WIRELESSNETWORKING"

#define NAME_LEN 30

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad)
{
    char addr_str[BT_ADDR_LE_STR_LEN];
    bool psk_matched = false;

    bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));

    while (ad->len > 1) {
        uint8_t len = net_buf_simple_pull_u8(ad);
        uint8_t type = net_buf_simple_pull_u8(ad);

        /* Check for Complete Local Name */
        if (type == BT_DATA_NAME_COMPLETE) {
            if (len-1 == strlen(BROADCASTER_ISK) && !strncmp((char *)ad->data, BROADCASTER_ISK, len-1)) {
                psk_matched = true;
                break;  // Exit the loop once the PSK is matched
            }
        }

        net_buf_simple_pull(ad, len - 1);
    }

    // If the PSK did not match, simply return and do nothing.
    if (!psk_matched) {
        return;
    }

    // If we reach here, it means the PSK matched. Continue processing as required.
    printk("Broadcaster with matching ISK found: %s (RSSI %d)\n", addr_str, rssi);
}



int observer_start(void)
{
	struct bt_le_scan_param scan_param = {
		.type       = BT_LE_SCAN_TYPE_PASSIVE,
		.options    = BT_LE_SCAN_OPT_FILTER_DUPLICATE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};
	int err;

	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		printk("Start scanning failed (err %d)\n", err);
		return err;
	}
	printk("Started scanning...\n");

	return 0;
}