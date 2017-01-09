#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"  //使用套接字
#include "arpa/inet.h"
#include "sys/syslog.h"
#include "sys/time.h"
#include "errno.h"  //错误号定义 errno
#include "linux/msg.h"  //消息队列
#include "semaphore.h"
#include "unistd.h"  //sleep()
#include "fcntl.h"
#include "server-init.h"
#include "client.h"
#include "string.h"  //strerror()

int server_sockfd;

int main(){
    int client_sockfd;
    int sin_size;
    int err_ret;
    struct sockaddr_in client_addr;


    log_init();   //初始化syslog，以便查看调试信息
    syslog(LOG_DEBUG,"start muma-server");
    client_manage_init();  //初始化客户端管理器
    server_socket_create(&server_sockfd,1080);
    listen(server_sockfd,10);  //开始监听,最大链接客户端是10
    syslog(LOG_DEBUG,"server-socket is listening...");


    while(1){
        //接受客户端的连接请求
        if((client_sockfd = accept(server_sockfd,(struct sockaddr*)&client_addr,(socklen_t *)&sin_size)) < 0){
            syslog(LOG_DEBUG,"accept error:%s",strerror(errno));
        }
        //创建客户端
        if((err_ret = client_create(client_sockfd,client_addr.sin_addr) < 0)){
            syslog(LOG_DEBUG,"failed to create client:%d",err_ret);//如果失败，输出失败代码
        }
    }
    return 1;
}