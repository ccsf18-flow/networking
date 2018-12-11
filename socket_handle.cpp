#include "socket_handle.hpp"

#include <fcntl.h>
#include <cerrno>

/*
 * Call this to send message.
 * parameters:
 * @tag     -> tag to designate what message is sent
 * @ms      -> current system time in ms message was sent
 * @gen_msg -> message struct to be sent.
 * return   -> returns true on completion, -1 otherwise.
 */

template<typename T>
bool send_message(uint8_t tag, uint64_t ms, T gen_msg){
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    msg msg_out;
    switch(tag){
        case 1:
            msg_out = {tag, ms, gen_msg};
            break;
    }


    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , &msg_out , sizeof(msg_out) , 0 );
    printf("Struct Sent!\n");
    return 1;
}

/*
 * Bind to our listen address, returning a server_socket structure
 * connected to the DAW
 */
struct server_socket begin_recv() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addr_len = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    // {
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    //Force onto port 8080?
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("port binding failed");
        exit(EXIT_FAILURE);
    }

    // Set the socket to nonblocking mode
    int fd_flags = fcntl(server_fd, F_GETFL);
    if (fd_flags < 0) {
        perror("getting socket flags");
        exit(EXIT_FAILURE);
    }
    fcntl(server_fd, F_SETFL, fd_flags | O_NONBLOCK);

    struct server_socket tr;
    tr.sockfd = server_fd;

    return tr;
}

/*
 * Call this to recieve data. NOTE: it must be called before send_msg
 * and will halt execution until it completes.
 *
 * return -> msg struct with all information sent. No validation done
 */
msg recv_data(server_socket * srv){
    struct sockaddr_in si_other;
    size_t slen = sizeof(si_other);

    msg rec_val = {0};
    int amount_read = 0;
    while ((amount_read = recvfrom(srv->sockfd, &rec_val, sizeof(msg), 0, (struct sockaddr *)&si_other, &slen)) < 0) {
         if (amount_read == -1) {
             if (errno != EWOULDBLOCK && errno != EAGAIN) {
               perror("receiving data");
               exit(EXIT_FAILURE);
             } else {
               rec_val.tag = MSG_TAG_NO_MSG;
               return rec_val;
             }
         } 
    }
    // while ((amount_read = recvfrom( srv->sockfd , &rec_val, sizeof(uint32_t), 0)) != sizeof(uint32_t)) {
    //     if (amount_read != -1 && amount_read != sizeof(uint32_t)) {
    //         printf("Read %d bytes\n", amount_read);
    //     }
    //     if (amount_read == -1 && errno != EWOULDBLOCK && errno != EAGAIN) {
    //         perror("receiving data");
    //         exit(EXIT_FAILURE);
    //     }
    // }
    printf("recieved message\n");
    return rec_val;
}
