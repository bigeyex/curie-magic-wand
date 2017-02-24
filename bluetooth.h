#ifndef FILE_BLUETOOTH
#define FILE_BLUETOOTH
#include <CurieBLE.h>


typedef void (*ble_read_handler_t)(const unsigned char value[]);
typedef void (*ble_connected_handler_t)();

// connect to the nearest mbot
void ble_setup();
void ble_connect_nearest();
void ble_write(const unsigned char value[], int length);
void ble_on_read(ble_read_handler_t handler);
void ble_on_connect(ble_connected_handler_t);
void ble_on_disconnect(ble_connected_handler_t);

// call this in loop
void ble_loop();

#endif