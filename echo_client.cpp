#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
using namespace std;
#define BUFFER_SIZE 1024
// #define PORT 8888
// #define SERVER_IP "127.0.0.1"
int PORT;
string SERVER_IP;
int main(){
    cerr << "请输入端口号" << endl;
    cin >> PORT;
    cerr << "请输入ip地址" << endl;
    cin >> SERVER_IP;
    cin.ignore(); 
    cout << "端口号为: " << PORT << " IP地址为: " << SERVER_IP << endl;
    int sock_fd = socket(AF_INET,SOCK_STREAM,0);
        if(sock_fd == -1){
            cerr << "socket创建失败" << endl;
            return -1;
        }
        struct sockaddr_in server_addr;
        memset(&server_addr,0,sizeof server_addr);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);

        if(inet_pton(AF_INET,SERVER_IP.c_str(),&server_addr.sin_addr)<=0){

            cerr << "IP地址无效" << endl;
            close(sock_fd);
            return -1;
        }

        if(connect(sock_fd,(struct sockaddr*)&server_addr,sizeof server_addr)==-1)
        {
            cerr << "连接失败" << endl;
            close(sock_fd);
            return -1;
        }
        cerr << "已连接到服务器" << endl;
        string  message;
    while(1){
        getline(cin, message);
        if(message == "quit")break;
        
        write(sock_fd,message.c_str(),message.length());
        cout << "发送：" << message << endl;

        char buffer[BUFFER_SIZE];
        int bytes_read = read(sock_fd,buffer,BUFFER_SIZE - 1);

        if(bytes_read > 0){
            buffer[bytes_read] = '\0';
            cout << "收到回复：" << buffer << endl;
        }
    }
    close((int)sock_fd);
    cout << "连接已关闭" << endl;

    return 0;
}