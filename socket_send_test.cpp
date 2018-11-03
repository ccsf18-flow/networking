#include "socket_handle.cpp"

int main(){
	beat_msg msg = {100, 200};
	send_message(1, 2000, msg);
	return 0;
}