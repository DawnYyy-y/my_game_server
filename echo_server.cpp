#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define PORT 8888
#define BUFFER_SIZE 1024
#define endl '\n'
using namespace std;

int main(){
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd == -1){
         cout << "socket 创建失败" << endl;
        return -1;
    }
   struct sockaddr_in server_addr;
   memset(&server_addr,0,sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = INADDR_ANY;
   server_addr.sin_port = htons(PORT);
   if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1){
        cerr<<"bind失败"<<endl;
        close(server_fd);
        return -1;
   }

   if(listen(server_fd,5) == -1){
    cerr << "listen失败" << endl;
    close(server_fd);
   }

   cout << "服务器启动，监听端口" << PORT << "..." << endl;

   struct sockaddr_in client_addr;
   socklen_t client_len = sizeof(client_addr);
   int client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&client_len);
   if(client_fd == -1){
        cerr << "accept失败" << endl;
        close(server_fd);
        return -1;
   }
    
   char client_ip[INET_ADDRSTRLEN];
   inet_ntop(AF_INET,&client_addr.sin_addr,client_ip,INET_ADDRSTRLEN);
   cout << "客户端已连接，IP: " << client_ip;
   cout << ",端口："<< ntohs(client_addr.sin_port) << endl;

   char buffer[BUFFER_SIZE];
   int bytes_read = read(client_fd,buffer,BUFFER_SIZE-1);

   if(bytes_read > 0){
        buffer[bytes_read] = '\0';
        cout << "收到消息：" << buffer << endl;
   }
   close(client_fd);
   close(server_fd);
   cout << "连接已关闭" << endl;

    return 0;

}