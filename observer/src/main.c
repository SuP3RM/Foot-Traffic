#include <zephyr/logging/log.h>
#include <zephyr/types.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gap.h>

LOG_MODULE_REGISTER(Observer, LOG_LEVEL_INF);

// Forward declaration of scan_cb function
static void scan_cb(struct net_buf_simple *buf);

static struct bt_le_scan_param scan_param = {
    .type       = BT_HCI_LE_SCAN_PASSIVE,
    .options    = BT_LE_SCAN_OPT_NONE,
    .interval   = BT_GAP_SCAN_FAST_INTERVAL,
    .window     = BT_GAP_SCAN_FAST_WINDOW,
};

static void scan_cb_wrapper(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
                            struct net_buf_simple *buf) {
    // You can ignore `addr`, `rssi`, and `adv_type` parameters if not needed.
    // Call your original scan_cb function.
    scan_cb(buf);
}

static void scan_cb(struct net_buf_simple *buf) {
        // Parse the advertising or scan response data.
        // See https://docs.zephyrproject.org/latest/reference/bluetooth/gap.html#_advertising_data_format
        // for more details.
        uint8_t len = buf->len;
        uint8_t type;
        uint8_t data;
        
        for (uint8_t i = 0; i < len; i++) {
                data = net_buf_simple_pull_u8(buf);
                type = (data & 0xF0) >> 4;
                data = (data & 0x0F);
        
                switch (type) {
                case BT_DATA_FLAGS:
                        LOG_INF("Flags: 0x%02x", data);
                        break;
                case BT_DATA_UUID16_SOME:
                case BT_DATA_UUID16_ALL:
                        LOG_INF("UUID16: 0x%02x", data);
                        break;
                case BT_DATA_UUID32_SOME:
                case BT_DATA_UUID32_ALL:
                        LOG_INF("UUID32: 0x%02x", data);
                        break;
                case BT_DATA_UUID128_SOME:
                case BT_DATA_UUID128_ALL:
                        LOG_INF("UUID128: 0x%02x", data);
                        break;
                case BT_DATA_NAME_SHORTENED:
                case BT_DATA_NAME_COMPLETE:
                        LOG_INF("Name: 0x%02x", data);
                        break;
                case BT_DATA_TX_POWER:
                        LOG_INF("TX Power: 0x%02x", data);
                        break;
                case BT_DATA_SOLICIT16:
                case BT_DATA_SOLICIT32:
                case BT_DATA_SOLICIT128:
                        LOG_INF("Solicit: 0x%02x", data);
                        break;
                case BT_DATA_SVC_DATA16:
                        LOG_INF("SvcData16: 0x%02x", data);
                        break;
                case BT_DATA_SVC_DATA32:
                        LOG_INF("SvcData32: 0x%02x", data);
                        break;
                case BT_DATA_SVC_DATA128:
                        LOG_INF("SvcData128: 0x%02x", data);
                        break;
                case BT_DATA_MANUFACTURER_DATA:
                        LOG_INF("Manufacturer: 0x%02x", data);
                        break;
                case BT_DATA_URI:
                        LOG_INF("URI: 0x%02x", data);
                        break;
                case BT_DATA_MESH_PROV:
                        LOG_INF("Mesh Provisioning: 0x%02x", data);
                        break;
                case BT_DATA_MESH_MESSAGE:    
                        LOG_INF("Mesh Message: 0x%02x", data);
                        break;
                case BT_DATA_MESH_BEACON:
                        LOG_INF("Mesh Beacon: 0x%02x", data);
                        break;
                }
        // If you want to stop scanning after receiving a packet,
        // uncomment the following line.
        bt_le_scan_stop();
        }
}

void main(void) {
    int err;

    LOG_INF("Starting Observer");

    err = bt_enable(NULL);
    if (err) {
        LOG_ERR("Bluetooth init failed (err %d)", err);
        return;
    }

    LOG_INF("Bluetooth initialized");

    err = bt_le_scan_start(&scan_param, scan_cb_wrapper);
    if (err) {
        LOG_ERR("Scanning failed to start (err %d)", err);
        return;
    }

    LOG_INF("Scanning started");

    while (1) {
        k_sleep(K_FOREVER);
    }
}