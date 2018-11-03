#include "socket_handle.hpp"

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
 * Call this to recieve data. NOTE: it must be called before send_msg
 * and will halt execution until it completes.
 *
 * return -> msg struct with all information sent. No validation done
 */
msg recv_data(){
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addr_len = sizeof(address);
    char buffer[1024] = {0};
    msg rec_val;
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
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
    if (listen(server_fd, 3) < 0)
    {
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addr_len))<0)
    {
        perror("accept failure");
        exit(EXIT_FAILURE);
    }
    read( new_socket , &rec_val, sizeof(struct msg));
    printf("recieved message\n");
    return rec_val;
}