#define BUF_SIZE 256 
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void DieWithError(char *errorMessage){
	perror(errorMessage); //標準エラー出力にエラーメッセージを出す。
	exit(1); //終了ステータス。1以上を返すという意味。
}
void commun(int sock){
	char buf[BUF_SIZE];
	int len_r; //受信文字数
	if((len_r = recv(sock, buf, BUF_SIZE, 0)) <= 0)
		DieWithError("recv() failed");
	buf[len_r] = '\0';
	printf("%s\n", buf);
	if(send(sock,buf,strlen(buf),0) != strlen(buf))
		DieWithError("send() sent a message of unexpected bytes");
}

int main(int argc, char **argv){
	int cliSock;
	int servSock = socket(PF_INET,SOCK_STREAM,0);
	struct sockaddr_in servAddress;
	struct sockaddr_in clientAddress;
	int szClientAddr;
    
	servAddress.sin_family = AF_INET;
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY:インターネットのアドレスだったらなんでもいい
	servAddress.sin_port = htons(80); //10001番のポートに接続してきたら対応する。
	bind(servSock,(struct sockaddr*)&servAddress,sizeof(servAddress)); //servSock:待ち受け専用
	listen(servSock,5); //5人まで順番待ちできる。
	while(1){
    	szClientAddr = sizeof(clientAddress);
    	cliSock = accept(servSock,(struct sockaddr*)&clientAddress,&szClientAddr);
    	commun(cliSock);
	}
    
	close(servSock);
    
	return 0;
}