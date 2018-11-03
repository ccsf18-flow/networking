#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 8080


struct beat_msg{
    uint8_t note;
    uint8_t intensity;
};

struct pitch_peak_msg{
	uint8_t holder;
};

struct msg {
    uint8_t tag;
    uint64_t time_in_ms;
    
    union{
        beat_msg b_msg;
        pitch_peak_msg p_msg;
    } data_msg;
};

msg recv_data();
bool send_message(uint8_t tag, uint64_t ms, void* data_msg);