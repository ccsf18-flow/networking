#include "socket_handle.cpp"

int main(){
	printf("Starting socket connection... \n");
	msg rec_msg = recv_data();
	printf("Value recieved!\n");
	printf("Message tag: %d\n", rec_msg.tag);
	printf("Message time: %d\n", rec_msg.time_in_ms);
	printf("Message Data...\n");
	printf("\tNote: %d\n", rec_msg.data_msg.b_msg.note);
	return 0;
}