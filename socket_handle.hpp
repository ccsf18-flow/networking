#ifndef _SOCKET_HANDLE_HPP_
#define _SOCKET_HANDLE_HPP_

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 8080

struct beat_msg {
    uint8_t note;
    uint8_t intensity;
} __attribute__((packed));

struct pitch_peak_msg {
    uint8_t holder;
} __attribute__((packed));

#define MSG_TAG_NO_MSG 255 
#define MSG_TAG_NOTE_ON 1
#define MSG_TAG_NOTE_OFF 2

struct msg {
    uint8_t tag;
    uint64_t time_in_ms;
    
    union{
        struct beat_msg b_msg;
        struct pitch_peak_msg p_msg;
    } data_msg;
} __attribute__((packed));

struct server_socket {
    int sockfd;
};

#ifdef _cplusplus
server_socket begin_recv();
msg recv_data(server_socket * srv);
bool send_message(uint8_t tag, uint64_t ms, void* data_msg);
#endif

#endif
