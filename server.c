#define BUF_SIZE 256 
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>

void DieWithError(char *errorMessage){
	perror(errorMessage); //�W���G���[�o�͂ɃG���[���b�Z�[�W���o���B
	exit(1); //�I���X�e�[�^�X�B1�ȏ��Ԃ��Ƃ����Ӗ��B
}
void commun(int sock){
	char buf[BUF_SIZE];
	int len_r; //��M������
	char *message = "";
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
	unsigned int szClientAddr;
    
	servAddress.sin_family = AF_INET;
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY:�C���^�[�l�b�g�̃A�h���X��������Ȃ�ł�����
	servAddress.sin_port = htons(10001); //10001�Ԃ̃|�[�g�ɐڑ����Ă�����Ή�����B
	bind(servSock,(struct sockaddr*)&servAddress,sizeof(servAddress)); //servSock:�҂��󂯐�p
	listen(servSock,5); //5�l�܂ŏ��ԑ҂��ł���B
	while(1){
    	szClientAddr = sizeof(clientAddress);
    	cliSock = accept(servSock,(struct sockaddr*)&clientAddress,&szClientAddr);
    	commun(cliSock);
	}
    
	close(servSock);
    
	return 0;
}