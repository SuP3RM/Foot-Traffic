#include <dk_buttons_and_leds.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* STEP 2.1 - Declare the Company identifier (Company ID) */
#define COMPANY_ID_CODE 439 /* Wiresless Networking Class */

/* STEP 2.2 - Declare the structure for your custom data  */
typedef struct adv_mfg_data {
  uint16_t company_code; /* Company Identifier Code. */
  uint16_t number_press; /* Number of times Button 1 is pressed */
} adv_mfg_data_type;

#define USER_BUTTON DK_BTN1_MSK

/* STEP 1 - Create an LE Broadcasting Parameters variable */
static struct bt_le_adv_param *adv_param =
    BT_LE_ADV_PARAM(BT_LE_ADV_OPT_NONE, /* No options specified */
                    800, /* Min Broadcasting Interval 500ms (800*0.625ms) */
                    801, /* Max Broadcasting Interval 500.625ms (801*0.625ms) */
                    NULL); /* Set to NULL for undirected Broadcasting */

/* STEP 2.3 - Define and initialize a variable of type adv_mfg_data_type */
static adv_mfg_data_type adv_mfg_data = {
    COMPANY_ID_CODE,
    0,
};

static unsigned char scan_repsonse[] = {
    0x03, /* Length */
    0x03, /* Param: Complete List of 16-bit Service Class UUIDs */
    0xAA, /* Eddystone UUID */
    0xFE,
    0x17, /* Length */
    0x16, /* Param: Service Data */
    0xAA, /* Eddystone UUID */
    0xFE,
    0x10, /* Eddystone-URL frame type */
    0x00, /* Tx power */
    0x00, /* URL Scheme Prefix */
    'n',  'o', 'r', 'd', 'i', 'c', 's', '.', 'c', 'o', 'm'
};

LOG_MODULE_REGISTER(main);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

#define RUN_STATUS_LED DK_LED1
#define RUN_LED_BLINK_INTERVAL 1000

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
    /* STEP 3 - Include the Manufacturer Specific Data in the Broadcasting
       packet. */
    BT_DATA(BT_DATA_MANUFACTURER_DATA, (unsigned char *)&adv_mfg_data,
            sizeof(adv_mfg_data)),
};

static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_URI, scan_repsonse, sizeof(scan_repsonse)),
};
/* STEP 5 - Add the definition of callback function and update the Broadcasting
 * data dynamically */
static void button_changed(uint32_t button_state, uint32_t has_changed) {
  if (has_changed & button_state & USER_BUTTON) {
    adv_mfg_data.number_press += 1;
    bt_le_adv_update_data(ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
  }
}
/* STEP 4.1 - Define the initialization function of the buttons and setup
 * interrupt.  */
static int init_button(void) {
  int err;

  err = dk_buttons_init(button_changed);
  if (err) {
    printk("Cannot init buttons (err: %d)\n", err);
  }

  return err;
}

void main(void) {
  int blink_status = 0;
  int err;

  LOG_INF("Starting Broadcaster \n");

  err = dk_leds_init();
  if (err) {
    LOG_ERR("LEDs init failed (err %d)\n", err);
    return;
  }
  /* STEP 4.2 - Setup buttons on your board  */
  err = init_button();
  if (err) {
    printk("Button init failed (err %d)\n", err);
    return;
  }

  err = bt_enable(NULL);
  if (err) {
    LOG_ERR("Bluetooth init failed (err %d)\n", err);
    return;
  }

  LOG_INF("Bluetooth initialized\n");

  err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
  if (err) {
    LOG_ERR("Broadcasting failed to start (err %d)\n", err);
    return;
  }

  LOG_INF("Broadcasting successfully started\n");

  for (;;) {
    dk_set_led(RUN_STATUS_LED, (++blink_status) % 2);
    k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
  }
}
