//TCP聊天室服务器
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
struct client{
	char name[20];//客户端连接上来时 输入的姓名
	int fds;//客户端的socket
};
struct client c[100]={0};//最多记录100个sockfd
int size=0;//记录客户端的个数，数组的索引
int sockfd;//服务器的sockfd（结束时用信号关闭）
char filename[4096]={0};//用来传文件
char* IP="192.168.0.100";//记录本机IP
short PORT=8888;
typedef struct sockaddr SA;//通信地址类型转换的时候使用

//初始化服务器的网络,创建socket..
void init(){
	printf("聊天室服务器开始启动..\n");
	//创建socket
	sockfd=socket(PF_INET,SOCK_STREAM,0);
	if(sockfd==-1){ 
		perror("创建socket失败"); 
		printf("服务器启动失败\n");
		exit(-1);}
	//准备通信地址
	struct sockaddr_in addr;
	addr.sin_family=PF_INET;
	addr.sin_port=htons(PORT);
	addr.sin_addr.s_addr=inet_addr(IP);
	//绑定socket和通信地址
	if(bind(sockfd,(SA*)&addr,sizeof(addr))==-1){
		perror("绑定失败");
		printf("服务器启动失败\n");
		exit(-1);
	}
	printf("成功绑定\n");
	//设置监听
	if(listen(sockfd,100)==-1){ 
		perror("设置监听失败");
		printf("服务器启动失败\n");
		exit(-1);
	}
	printf("设置监听成功\n");
	printf("初始化服务器成功\n");
	//等待客户端连接和通信部分写到另一个函数中
}
//用来分发消息的函数
void sendMsgToAll(char* msg){
	int i=0;
	for(;i<size;i++){
		if(c[i].fds!=0){
			printf("sendto:%d\n",c[i].fds);
			send(c[i].fds,msg,strlen(msg),0);
		}
	}
}
//线程函数，用来接受客户端的消息并分发给所有客户
void* service_thread(void* p){
	int m_id=0;
	char name[20]={};
	if(recv(c[size].fds,name,sizeof(name),0)>0){//接受到名字
		strcpy(c[size].name,name);
	}
	m_id=size;
	size++;
	//进入线程之后，先群发一条提示，提示谁连上来了
	char tishi[100]={};
	sprintf(tishi,"热烈欢迎%s登录本聊天室..",
			c[m_id].name);
	sendMsgToAll(tishi);
	int fd=*(int*)p;
	printf("pthread=%d\n",fd);//线程对应客户端的描述符
	//通信，接受消息，分发消息
	while(1){
		char buf[100]={};
		if(recv(fd,buf,sizeof(buf),0)<=0){//接收消息
			printf("fd=%dquit\n",fd);//recv函数返回值小于0，则表示有客户端断开，打印quit
	//然后将退出客户端的socket重新置0
			int i;
			for(i=0;i<size;i++){
				if(c[i].fds==fd){
					c[i].fds=0;
					break;
				}
			}
			char msg[100]={};
			sprintf(msg,"欢送%s离开聊天室",c[i].name);
		//将退出消息发给所有聊天的人
			sendMsgToAll(msg);
			return ;//某客户端退出后，结束线程
		}//正确读到数据之后
		else{
			char msg[100]={0};
			sprintf(msg,"%s说:%s",c[m_id].name,buf);
			sendMsgToAll(msg);
		}
		bzero(buf,100);
	}
}

//等待客户端连接，启动服务器的服务
void service(){
	printf("服务器开始服务\n");
	while(1){
		struct sockaddr_in fromaddr;//存储客户端的通信地址
		socklen_t len=sizeof(fromaddr);
		int fd=accept(sockfd,(SA*)&fromaddr,&len);
		if(fd==-1){
			printf("客户端连接出错..\n");
			continue;//继续下一次循环，等待客户端连接
		}
		//如果客户端连接成功，那就记录它的socket描述符，并开启一个线程，为此客户端服务
		//记录客户端的socket
		c[size].fds=fd;
		printf("fd=%d\n",fd);
		//开启线程
		pthread_t pid;
		pthread_create(&pid,0,service_thread,&c[size].fds);
	}
}
void sig_close(){
	//关闭服务器的socket
	close(sockfd);
	printf("服务器已经关闭..\n");
	exit(0);
}
int main(){
	//发信号，ctrl+c 然后关闭socket
	signal(SIGINT,sig_close);//自定义信号处理函数，做好善后工作，关闭socket
	//初始化网络
	init();
	//启动服务
	service();
	return 0;
}


