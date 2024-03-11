How to Run:
  1. Compile the server file: gcc server.c -o server
  2. Get the server running(can use any port number, here using 9999): ./server 9999
  3. Complie client file: gcc client.c -o client
  4. Connect the client to server(use 127.0.0.1 if on the same device or use the server's ip address if using in two different devices. Port no. should be same as the server): ./client 127.0.0.1 9999
  5. DONE!
