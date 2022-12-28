all:
	gcc cp.c -o 49_1_cp
	gcc posix_msg_file_server.c error_functions.c get_num.c -o 52_3_server
	gcc posix_msg_file_client.c error_functions.c get_num.c -o 52_3_client
	gcc psshm_xfr_reader.c error_functions.c get_num.c binary_sems.c -o 54_1_reader
	gcc psshm_xfr_writer.c error_functions.c get_num.c binary_sems.c -o 54_1_writer


clean:
	rm -f 49_1_cp
	rm -f 52_3_server
	rm -f 52_3_client
	rm -f 54_1_reader
	rm -f 54_1_writer
	

