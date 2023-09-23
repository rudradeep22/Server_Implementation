#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


void error(const char *err){

	perror(err);
	exit(1);
}

int main(int argc, char* argv[]){
	
	if(argc < 2){
		fprintf(stderr, "Port no. not given. Terminated\n");
		exit(1);
	}
	int sockfd, newsockfd, port, n;
	char buffer[255];
	struct sockaddr_in server_addr, cli_addr;	//defined in netinet/in.h
	socklen_t clilen;		// 32bit datatype in sys/socket.h

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){		//socket resulted in failure
		error("Socket not generated\n");
	}

	memset(&server_addr, 0, sizeof(server_addr));
	port = atoi(argv[1]);		// argv[0] is filename, argv[1] has port no

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if( bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) <0)
		error("Binding failed!\n");

	listen(sockfd, 5); // 5 is number of clients to be supported
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if(newsockfd < 0){
		error("Socket failed to connect!\n");
	}

	while(1){
		memset(buffer, 0, 255);
		n = read(newsockfd, buffer, 255);
		if(n < 0)
			error("Failed to read!\n");

		printf("Client: %s\n", buffer);
		memset(buffer, 0, 255);
		fgets(buffer, 255, stdin);		//reply from server

		n = write(newsockfd, buffer, strlen(buffer));
		if(n<0)
			error("Failed to write!\n");
		int i = strncmp("Bye", buffer, 3);	//first 3 letters are bye
		if(!i)
				break;
	}
	close(newsockfd);
	close(sockfd);		//close both file descriptors

	return 0;
}