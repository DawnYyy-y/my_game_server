#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <algorithm> 
#include <thread>
#define PORT 8888
#define BUFFER_SIZE 1024
#define endl '\n'
using namespace std;
void handle_client(int client_fd){
     for(;;){
          char buffer[BUFFER_SIZE];

          int bytes_read = read(client_fd,buffer,BUFFER_SIZE-1);
          if(bytes_read > 0){
               buffer[bytes_read] = '\0';
               cout << "线程" << this_thread::get_id() << "收到："<< buffer << endl;
               transform(buffer, buffer + bytes_read, buffer, ::toupper);
               write(client_fd,buffer,bytes_read);
               cout << "线程" << this_thread::get_id() << "已返回" << endl;
          }
          else if(bytes_read == 0){
               cout << "线程" << this_thread::get_id() << "检测到客户端关闭连接" << endl;
               break;  
          }
          else {
               cerr << "线程" << this_thread::get_id() << "读取错误" << endl;
               break; 
          }
     }
     close(client_fd);
    cout << "线程 " << this_thread::get_id() << " 结束" << endl;
}
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
    return -1;
   }

   cout << "服务器启动，监听端口" << PORT << "..." << endl;

   while(1){
          struct sockaddr_in client_addr;
          socklen_t client_len = sizeof(client_addr);
          int client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&client_len);
          if(client_fd == -1){
               cerr << "accept失败" << endl;
               continue;
          }
          
          char client_ip[INET_ADDRSTRLEN];
          inet_ntop(AF_INET,&client_addr.sin_addr,client_ip,INET_ADDRSTRLEN);
          cout << "新客户端连接,IP: " << client_ip << ",创建新线程" << endl;
          thread t(handle_client,client_fd);
          t.detach();
     }
   close(server_fd);
   cout << "连接已关闭" << endl;

    return 0;

}