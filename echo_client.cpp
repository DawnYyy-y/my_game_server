#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    // 1. 创建socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        std::cerr << "socket创建失败" << std::endl;
        return -1;
    }
    
    // 2. 设置服务器地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    // 转换IP地址
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "IP地址无效" << std::endl;
        close(sock_fd);
        return -1;
    }
    
    // 3. 连接服务器
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "连接失败" << std::endl;
        close(sock_fd);
        return -1;
    }
    std::cout << "已连接到服务器" << std::endl;
    
    // 4. 发送消息
    const char* message = "Hello, 回声服务器！";
    write(sock_fd, message, strlen(message));
    std::cout << "发送: " << message << std::endl;
    
    // 5. 接收回复
    char buffer[BUFFER_SIZE];
    int bytes_read = read(sock_fd, buffer, BUFFER_SIZE - 1);
    
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        std::cout << "收到回复: " << buffer << std::endl;
    }
    
    // 6. 关闭连接
    close(sock_fd);
    std::cout << "连接已关闭" << std::endl;
    
    return 0;
}