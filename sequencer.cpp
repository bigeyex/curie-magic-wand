#include<Arduino.h>
#include "sequencer.h"
#include "mbotCommand.h"

#define SEQ_LIST_LENGTH 30

mbot_command_t seq_list[SEQ_LIST_LENGTH];
mbot_command_t seq_single_last_command;
int seq_index = 0;
int seq_play_index = 0;
unsigned long seq_last_timestamp = 0;

enum seq_run_status_t {
    SEQ_STATUS_IDLE, SEQ_STATUS_PLAY, SEQ_STATUS_SINGLE, SEQ_STATUS_SINGLE_RUNNING
};

seq_run_status_t seq_run_status = SEQ_STATUS_IDLE;

void seq_send_command(mbot_command_t command);

void seq_clear() {
    seq_index = 0;
}

void seq_play() {
    seq_last_timestamp = 0;
    seq_play_index = 0;
    seq_run_status = SEQ_STATUS_PLAY;
}

void seq_stop() {
    seq_send_command(MOVE_STOP);
    seq_run_status = SEQ_STATUS_IDLE;
}

void seq_add(mbot_command_t command) {
    if(seq_index < SEQ_LIST_LENGTH) {
        seq_list[seq_index] = command;
        seq_index++;
    }
}

void seq_run_single(mbot_command_t command) {
    seq_last_timestamp = 0;
    seq_run_status = SEQ_STATUS_SINGLE;
    seq_single_last_command = command;
}

void seq_loop() {
    if(millis() - seq_last_timestamp > MBOT_MOVE_TIME*1000) {
        seq_last_timestamp = millis();
        if (seq_run_status == SEQ_STATUS_PLAY) {
            if(seq_play_index < seq_index) {
                seq_send_command(seq_list[seq_play_index]);
                seq_play_index++;
            }
            else {  // finished the whole playing sequence
                seq_send_command(MOVE_STOP);
                seq_run_status = SEQ_STATUS_IDLE;
            }
        }
        else if (seq_run_status == SEQ_STATUS_SINGLE) {
            seq_send_command(seq_single_last_command);
            seq_run_status = SEQ_STATUS_SINGLE_RUNNING;
        }
        else if (seq_run_status == SEQ_STATUS_SINGLE_RUNNING) {
            seq_send_command(MOVE_STOP);
            seq_run_status = SEQ_STATUS_IDLE;
        }
    }
}

void seq_send_command(mbot_command_t command) {
    switch(command) {
        case MOVE_FORWARD: 
            mbot_forward(MBOT_SPEED);
            break;
        case MOVE_BACKWARD:
            mbot_back(MBOT_SPEED);
            break;
        case MOVE_LEFT:
            mbot_left(MBOT_ROTATE_SPEED);
            break;
        case MOVE_RIGHT:
            mbot_right(MBOT_ROTATE_SPEED);
            break;    
        case MOVE_STOP:
            mbot_stop();
            break;  
    }
}