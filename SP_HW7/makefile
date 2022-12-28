all:
	gcc error_functions.c sig_speed_sigsuspend.c get_num.c -o sigsuspend
	gcc error_functions.c sig_speed_sigwaitinfo.c get_num.c -o sigwaitinfo
	gcc systemv_api.c -o systemv_api
	
clean:
	rm -f sigsuspend
	rm -f sigwaitinfo
	rm -f systemv_api
