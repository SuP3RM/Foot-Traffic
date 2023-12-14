#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/types.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>

// PSK
#define BROADCASTER_IRK "CS439WIRELESSNETWORKING"

static uint8_t mfg_data[] = {0xff, 0xff, 0x00};

static const struct bt_data ad[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 3),
    BT_DATA(BT_DATA_NAME_COMPLETE, BROADCASTER_IRK,
            sizeof(BROADCASTER_IRK) - 1) // -1 to exclude null terminator
};

int main(void) {
  int err;

  printk("Starting Broadcaster\n");

  /* Initialize the Bluetooth Subsystem */
  err = bt_enable(NULL);
  if (err) {
    printk("Bluetooth init failed (err %d)\n", err);
    return 0;
  }

  printk("Bluetooth initialized\n");

  struct bt_le_oob oob;
  err = bt_le_oob_get_local(BT_ID_DEFAULT, &oob);
  if (err) {
    printk("Error fetching local OOB (err %d)\n", err);
  } else {
    char addr_str[BT_ADDR_STR_LEN];
    bt_addr_to_str(&oob.addr.a, addr_str, sizeof(addr_str));
    printk("Local Address: %s\n", addr_str);
  }

  do {
    k_msleep(10); // Sleep for 100 milliseconds 

    printk("Sending advertising data: 0x%02X\n", mfg_data[2]);

    
    struct bt_le_adv_param adv_param = BT_LE_ADV_PARAM(BT_LE_ADV_OPT_USE_IDENTITY, 
      BT_GAP_ADV_FAST_INT_MIN_2, 
      BT_GAP_ADV_FAST_INT_MAX_2, 
      NULL);

    // Set channel map to prefer the desired channel (37, 38, or 39)
    adv_param.channel_map = BIT(38 - 37);
    /* Start advertising */
    err =
        bt_le_adv_start(&adv_param, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
      printk("Broadcaster advertising failed to start (err %d)\n", err);
      return 0;
    }

    k_msleep(10); // Sleep for 100 milliseconds 

    err = bt_le_adv_stop();
    if (err) {
      printk("Broadcaster advertising failed to stop (err %d)\n", err);
      return 0;
    }

    mfg_data[2]++;

  } while (1);

  return 0;
}
