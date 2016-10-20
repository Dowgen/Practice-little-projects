//TCP聊天室客户端
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h>
#include<pthread.h>

int sockfd;//客户端socket
char* IP="192.168.0.100";
short PORT=8888;//服务器服务端口
typedef struct sockaddr SA;
char name[30];//用于存放聊天任的昵称

//客户端也分为：启动客户端，连接服务器
void init(){
	printf("聊天室客户端开始启动\n");
	sockfd=socket(PF_INET,SOCK_STREAM,0);
	if(sockfd==-1){ perror("socket"); exit(-1);}

	struct sockaddr_in addr;
	addr.sin_family=PF_INET;
	addr.sin_port=htons(PORT);
	addr.sin_addr.s_addr=inet_addr(IP);
	
	if(connect(sockfd,(SA*)&addr,
			sizeof(addr))==-1){
		perror("无法连接到服务器");
		printf("客户端启动失败\n");
		exit(-1);
	}
	printf("连接成功\n");
}
//开始通信
void start(){
	//发送消息之前启动一个线程，用来接受服务器发过来的消息
	pthread_t pid;
	void* recv_thread(void*);//线程函数声明
	pthread_create(&pid,0,recv_thread,0);
	char buf[100]={0};
	while(1){//内部不断的发送数据
		bzero(buf,100);
		scanf("%s",buf);//读取客户端的输入信息
		send(sockfd,buf,strlen(buf),0);//发消息给服务器
	}
}

void* recv_thread(void* p){
	while(1){
		char buf[100]={};//存储读到的消息
		if(recv(sockfd,buf,sizeof(buf),0)<=0){//接受出错
		return ;//结束线程
		}
		printf("%s\n",buf);
	}
}
void sig_close(){
	//关闭客户端的socket
	close(sockfd);
	exit(0);
}
int main(){
	//f发信号，关闭socket
	signal(SIGINT,sig_close);//自定义信号处理函数
	printf("请输入您的名字:");
	scanf("%s",name);
	init();
	send(sockfd,name,strlen(name),0);//把名字给服务器
	start();//通信
}
