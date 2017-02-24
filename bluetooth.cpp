#include "bluetooth.h"

#define MAX_IMU_WRITE_COMMAND_LENGTH 64
#define MAX_IMU_READ_COMMAND_LENGTH 64
#define BLE_AUTO_CONNECT_THRESHOLD -50
#define MAX_IMU_RECORD 1

typedef struct {
    int index;
    unsigned int slot[3];
} ble_imu_frame_t;
ble_read_handler_t ble_read_handler = NULL;
ble_connected_handler_t ble_connected_handler = NULL;
ble_connected_handler_t ble_disconnected_handler = NULL;
BLECharacteristic ble_write_characteristic, ble_read_characteristic;
bool _ble_is_connected = false;
void ble_central_discover_handler(BLEDevice peripheral);
void ble_central_disconnect_handler(BLEDevice peripheral);

void ble_setup() {
    BLE.begin();
    BLE.setEventHandler(BLEDiscovered, ble_central_discover_handler);
    BLE.setEventHandler(BLEDisconnected, ble_central_disconnect_handler);
}

void ble_connect_nearest() {
    BLE.scanForName("Makeblock_LE");
}

void ble_loop() {
    BLE.poll();
}

void ble_write(const unsigned char value[], int length){
    if(_ble_is_connected){
        ble_write_characteristic.write(value, length);
    }
}

void ble_on_read(ble_read_handler_t handler){
    ble_read_handler = handler;
}

void ble_on_connect(ble_connected_handler_t handler){
    ble_connected_handler = handler;
}

void ble_on_disconnect(ble_connected_handler_t handler){
    ble_disconnected_handler = handler;
}

// privates

void ble_central_disconnect_handler(BLEDevice peripheral) {
    _ble_is_connected = false;
    if(ble_disconnected_handler) {
        ble_disconnected_handler();
    }
}

void ble_central_discover_handler(BLEDevice peripheral) {
    if(peripheral.rssi() > BLE_AUTO_CONNECT_THRESHOLD) {
        BLE.stopScan();
        if(peripheral.connect()) {
            peripheral.discoverAttributes();
            ble_read_characteristic = peripheral.characteristic("FFE2");
            ble_write_characteristic = peripheral.characteristic("FFE3");
            _ble_is_connected = true;
            if (ble_connected_handler) {
                ble_connected_handler();
            } 

            if(!ble_write_characteristic) {
                Serial.println("No Write Char");
            }
        }
        else {
            BLE.scanForName("Makeblock_LE");
            
            return;
        }

    }
}
