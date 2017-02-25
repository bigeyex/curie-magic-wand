#include "Bluetooth.h"
#include "sequencer.h"

const unsigned char beepcmd[] = {0xff, 0x55, 0x07, 0x00, 0x02, 0x22, 0x06, 0x01, 0xfa, 0x00};

void setup() {
    Serial.begin(115200); 
    Serial.println("Start");
    ble_setup();
    ble_on_connect(on_connected);
    ble_on_disconnect(on_disconnet);
    ble_connect_nearest();
}

void on_connected() {
    Serial.println("Connected");
    ble_write(beepcmd, 10);
    delay(1000);
    // use seq_run_single to run a single command;
    seq_add(MOVE_FORWARD);
    seq_add(MOVE_BACKWARD);
    seq_add(MOVE_LEFT);
    seq_add(MOVE_RIGHT);
    seq_add(MOVE_STOP);
    seq_play();
}

void on_disconnet() {
    Serial.println("Disconnected");
    ble_connect_nearest();
}

void loop() {
    ble_loop();
    seq_loop();
}
