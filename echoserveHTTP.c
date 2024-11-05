#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

// max number of requests to queue
#define LISTEN_BACKLOG 5

// handle argument "-p" to specify port number
// "guts" of program: handle a connection, read from client, write to client
// usage: ./echoserve

void handleHTTPConnection(int someClient)
{
    char buffer[1024];
    int bytesRead;

    // read the HTTP request from the client
    bytesRead = read(someClient, buffer, sizeof(buffer) - 1);

    if (bytesRead > 0)
    {
        // clear the buffer
        buffer[bytesRead] = '\0';
        printf("Received request:\n%s\n", buffer);

        // some basic http response
        const char *http_response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 51\r\n"
            "\r\n"
            "<html><body><h1>Hello from the server!</h1></body></html>";

        // send the HTTP response to the client
        write(someClient, http_response, strlen(http_response));

        printf("HTTP response sent to client.\n");
    }
    // close the connection
    close(someClient);
}

int main(int argc, char *argv[])
{
    // default (can't use global PORT since we need to modify)
    int port_number = 8000;

    // specify the port number
    if (argc == 3)
    {
        if (strcmp(argv[1], "-p") == 0)
        {
            port_number = atoi(argv[2]);
        }
        printf("Listening to port number: %d\n", port_number);
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in socket_address;
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;          // internet address family
    socket_address.sin_port = htons(INADDR_ANY);  // any address on computer (listen on all, IDC!)
    socket_address.sin_port = htons(port_number); // only listen on this port

    int return_value;
    return_value = bind(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));

    // now listen in on the socket
    return_value = listen(socket_fd, LISTEN_BACKLOG);

    // accept a connection
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);

    bool flag = true;

    while (flag)
    {
        int client_socket = accept(socket_fd, (struct sockaddr *)&client_address, &client_address_length);

        int pid = fork();

        // pid being 0 means it is the child process
        if (pid == 0)
        {
            handleHTTPConnection(client_socket);
            close(client_socket);
            flag = false;
        }
        else
        {
            close(client_socket);
        }
    }
    return 0;
}