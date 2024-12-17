![[blocking-vs-nonblocking.png]]

> Sockets can be in either blocking or non-blocking mode.
> 在阻塞模式下，`recv`, `send`, `connect` and `accept` (TCP only),  socket API 调用将无限期阻塞，直到执行了请求的操作。
> 在非阻塞模式下，这些函数立即返回。如果使用 `select` ，`select` 将阻塞，直到套接字准备好。`select` 采用超时参数，该参数控制等待操作完成或错误的时间量。

阻塞套接字。
遵循一个简单的范例：I/O操作暂停程序的执行，直到完成。
当 read from 或 write to 阻塞套接字时，程序将阻塞，直到数据可供读取或写入操作完成。

优势：简单

问题：使用阻塞套接字意味着在任何一个线程中，任何时候只有一个套接字可以处于活动状态。
即当使用阻塞套接字同时与多个客户端通信时，如果一个客户端的操作需要非常长的时间才能完成，则可能阻塞整个程序，从而可能导致服务其他客户端的延迟。

通常的解决方案是，使用多个套接字，为每个套接字创建一个线程，但线程有开销。

---
```c
void set_nonblocking(int sockfd) { 
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}
// 调用该函数后，文件描述符被认为是非阻塞的。之后，会阻塞的 I/O 系统调用（如 read 和 write 将返回 -1 ，并将 errno 设置为 EWOULDBLOCK
```
---
非阻塞套接字。当在非阻塞套接字上发起 I/O 操作时，无论操作是否成功，程序都会立即继续执行。这种异步行为允许程序在等待 I/O 操作完成的同时执行其他任务，从而提高整体效率和响应能力。

特征：
非阻塞行为：I/O 操作立即返回，即使它们无法立即完成。
异步操作：操作是异步执行的，使程序能够继续执行而无需等待每个操作完成。
复杂性增加：非阻塞套接字给程序逻辑带来了额外的复杂性，因为它需要处理操作可能无法立即完成的情况。

非阻塞套接字提供了提升的响应能力和更好的资源利用率，但它们需要仔细处理异步事件。开发者必须实现有效管理非阻塞套接字的异步特性的机制，如采用事件循环或使用 `select()` 或 `poll()` 等多路复用技术。

下述代码使用 `epoll` 监听多个 socket 。
可以使用 `nc` 或 `telnet` 连接到该服务器
``` bash
nc localhost 8080
nc localhost 8081

// telnet
telnet localhost 8080
telnet localhost 8081
```
当多个客户端连接到 Blocking Server 时，仅第一个可与服务器通信，只有断开第一个客户端与服务器连接后，第二个客户端方能与服务器通信。

而当多个客户端与 Non-Blocking Server 连接时，可以异步建立连接。服务器可以异步接收来自多个客户端的数据（通过 `accept()` 接收来自客户端的 `connect()` 连接时，创建的sockets）。

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_EVENTS 10

void handle_client_blocking(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // 服务器将在下面循环中阻塞（等待从客户端 read 数据），直到客户端主动断开连接。
    while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        
        printf("Blocking server received: %.*s", (int)bytes_read, buffer);
        write(client_socket, buffer, bytes_read);
    }

    if (bytes_read == 0) {
        printf("Blocking server: Client disconnected\n");
    } else {
        perror("Blocking server: read error");
    }

    close(client_socket);
}

void set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

int create_server_socket(int port) {
    int server_fd;
    struct sockaddr_in address;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void run_blocking_server() {
    int server_fd = create_server_socket(PORT);
    int client_socket;
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);

    printf("Blocking server running on port %d\n", PORT);

    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&client_address, &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Blocking server: New connection\n");
        handle_client_blocking(client_socket);
    }
}

void run_nonblocking_server() {
    int server_fd = create_server_socket(PORT + 1);
    set_nonblocking(server_fd);

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN | EPOLLET; // Edge-triggered mode
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }

    printf("Non-blocking server with epoll running on port %d\n", PORT + 1);

    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == server_fd) {
                // New connection
                struct sockaddr_in client_address;
                socklen_t addrlen = sizeof(client_address);
                int client_socket = accept(server_fd, (struct sockaddr *)&client_address, &addrlen);
                // 此处使用新的 client_socket 与 client 通信
                if (client_socket == -1) {
                    perror("accept");
                    continue;
                } else {
                    printf("Use socket %d to communicate with client", client_socket);
                }
                
                set_nonblocking(client_socket);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &ev) == -1) {
                    perror("epoll_ctl: client_socket");
                    exit(EXIT_FAILURE);
                }
                printf("Non-blocking server: New connection\n");
            } else {
                // Data from existing connection
                int client_socket = events[n].data.fd;
                char buffer[BUFFER_SIZE];
                ssize_t bytes_read;

                while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
                    printf("Non-blocking server received: %.*s from socket %d", (int)bytes_read, buffer, client_socket);
                    write(client_socket, buffer, bytes_read);
                }

                if (bytes_read == -1 && errno != EAGAIN) {
                    perror("read");
                    close(client_socket);
                } else if (bytes_read == 0) {
                    printf("Non-blocking server: Client disconnected\n");
                    close(client_socket);
                }
            }
        }
    }
}

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        run_blocking_server();
    } else if (pid > 0) {
        // Parent process
        run_nonblocking_server();
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}
```