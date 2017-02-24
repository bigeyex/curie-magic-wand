#include "bluetooth.h"

#define MBOT_PREFIX_A 0xff
#define MBOT_PREFIX_B 0x55
#define MBOT_CMD_LENGTH 0x07
#define MBOT_CMD_INDEX 0x00
#define MBOT_CMD_WRITE 0x02
#define MBOT_CMD_TYPE 0x05
#define MBOT_LEFT_MOTOR_INDEX 0X06
#define MBOT_RIGHT_MOTOR_INDEX 0X08

unsigned char mbot_command_template[10] = {
    MBOT_PREFIX_A, MBOT_PREFIX_B, MBOT_CMD_LENGTH, MBOT_CMD_INDEX, 
    MBOT_CMD_WRITE, MBOT_CMD_TYPE, 0x00, 0x00, 0x00, 0x00
};

void mbot_motors_command(int leftSpeed, int rightSpeed) {
    mbot_command_template[MBOT_LEFT_MOTOR_INDEX] = (unsigned char)(leftSpeed & 0xff);
    mbot_command_template[MBOT_LEFT_MOTOR_INDEX+1] = (unsigned char)(leftSpeed >> 8);
    mbot_command_template[MBOT_RIGHT_MOTOR_INDEX] = (unsigned char)(rightSpeed & 0xff);
    mbot_command_template[MBOT_RIGHT_MOTOR_INDEX+1] = (unsigned char)(rightSpeed >> 8);

    ble_write(mbot_command_template, sizeof(mbot_command_template));
}

void mbot_forward(int speed) {
    mbot_motors_command(-speed, speed);
}

void mbot_back(int speed) {
    mbot_motors_command(speed, -speed);
}

void mbot_left(int speed) {
    mbot_motors_command(speed, speed);
}

void mbot_right(int speed) {
    mbot_motors_command(-speed, -speed);
}

void mbot_stop() {
    mbot_motors_command(0, 0);
}