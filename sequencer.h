#ifndef FILE_SEQUENCER
#define FILE_SEQUENCER

// the moving speed of mbot
#define MBOT_SPEED 200
#define MBOT_ROTATE_SPEED 200

// how long (seconds) does the mbot move
#define MBOT_MOVE_TIME 1

enum mbot_command_t {
    MOVE_FORWARD, MOVE_BACKWARD, MOVE_LEFT, MOVE_RIGHT, MOVE_STOP
};

void seq_clear();
void seq_play();
void seq_stop();
void seq_add(mbot_command_t command);
void seq_run_single(mbot_command_t command);

void seq_loop();


#endif