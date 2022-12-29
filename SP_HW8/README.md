HW8-1 from the exercise 44-6

The server in Listing 44-7 (fifo_seqnum_server.c) assumes that the client process is
well behaved. If a misbehaving client created a client FIFO and sent a request to the
server, but did not open its FIFO, then the server’s attempt to open the client FIFO
would block, and other client’s requests would be indefinitely delayed. (If done
maliciously, this would constitute a denial-of-service attack.) Devise a scheme to deal
with this problem. Extend the server (and possibly the client in Listing 44-8)
accordingly

![image](https://user-images.githubusercontent.com/72913466/209906722-cb33ee4a-b73d-46a5-83b3-82626a6d28dd.png)

![image](https://user-images.githubusercontent.com/72913466/209906784-d740fa94-249b-45f3-8196-dd9310b3c41d.png)

HW8-2 from the exercise 44-7

Write programs to verify the operation of nonblocking opens and nonblocking I/O
on FIFOs (see Section 44.9).

![image](https://user-images.githubusercontent.com/72913466/209906921-72101f82-fc7c-43da-a210-0b9bede2c75d.png)

HW8-3 from the exercise 46-2

Recode the sequence-number client-server application of Section 44.8 to use System V
message queues. Use a single message queue to transmit messages from both client
to server and server to client. Employ the conventions for message types described
in Section 46.8.

![image](https://user-images.githubusercontent.com/72913466/209907088-d2b029f9-8134-43bc-b985-99f64fc61d7a.png)

