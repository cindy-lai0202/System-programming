part1

Set up a client and server while talk over TCP AF_UNIX sockets. The server
performs the dictionary look up.
1. Edit the lookup7.c file to communicate with a server by using TCP
AF_UNIX sockets.
2. Edit usock_server.c to listen on an AF_UNIX socket for any number of
TCP AF_UNIX clients and reply down the same socket.
3. After the file have been edited, type make, or make usock_serverand
usock_cilent.
4. When you get the prompt, run the usock_server and usock_client as
shown in the output below

![image](https://user-images.githubusercontent.com/72913466/209911243-fcac1b46-81ab-42e7-825b-1ba3b70ab39f.png)

![image](https://user-images.githubusercontent.com/72913466/209910870-f93444c7-3ed2-4e3c-9f60-d20be19245e8.png)

part2

Set up a client and server while talk over TCP AF_INET sockets. The server 
performs the dictionary look up.
1. Edit the lookup8.c file to communicate with a server by using TCP 
AF_INET sockets.
2. Edit isock_server.c to listen on an TCP AF_INET socket for any number of 
internet clients and reply down the same socket.
3. After the file have been edited, type make, or make isock_server and 
isock_cilent.
4. When you get the prompt, run the isock_server and isock_client.

![image](https://user-images.githubusercontent.com/72913466/209911199-9f43400c-7262-4609-a448-d059cdc006e6.png)

![image](https://user-images.githubusercontent.com/72913466/209911023-79de82e4-358b-403e-87f6-240948f561df.png)

part3

Set up a client and server while talk over UDP AF_INET sockets. server 
performs the dictionary look up.
1. Edit the lookup9.c file to communicate with a server by using UDP 
AF_INET sockets.
2. Edit udp_server.c to listen on an UDP AF_INET socket for any number of 
internet clients and reply down the same socket.
3. After the file have been edited, type make, or make udp_server and 
udp_cilent.
4. When you get the prompt, run the udp_server and udp_client as shown in 
the output below.

![image](https://user-images.githubusercontent.com/72913466/209911178-e6e4f524-266d-4933-b5cb-1c17d998dd77.png)

![image](https://user-images.githubusercontent.com/72913466/209911156-4aa79853-7399-45c1-a2c2-7e20615eedc1.png)
