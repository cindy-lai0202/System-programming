all:
	gcc fifo_seqnum_server.c error_functions.c get_num.c -o 44_6_server
	gcc fifo_seqnum_client.c error_functions.c get_num.c -o 44_6_client
	gcc fifo_seqnum_client_bad.c error_functions.c get_num.c -o 44_6_client_bad
	
	gcc fifo_server_nonblock.c error_functions.c get_num.c -o 44_7_server
	gcc fifo_client_nonblock.c error_functions.c get_num.c -o 44_7_client
	gcc svmsg_server.c error_functions.c get_num.c -o 46_2_server
	gcc svmsg_client.c error_functions.c get_num.c -o 46_2_client


clean:
	rm -f 44_6_server
	rm -f 44_6_client
	rm -f 44_6_client_bad
	rm -f 44_7_server
	rm -f 44_7_client
	rm -f 46_2_server
	rm -f 46_2_client

