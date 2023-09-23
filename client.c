#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

void error(const char *err){

	perror(err);
	exit(1);
}

//COMMAND LINE INPUT:  filename server_IP portno

int main(int argc, char* argv[]){
	
	if(argc < 3){
		fprintf(stderr, "3 arguments not given. Terminated\n");
		exit(1);
	}

	int sockfd, port, n;
	char buffer[255];
	struct sockaddr_in server_addr;	//defined in netinet/in.h
	struct hostent *server;	//in netdb.h

	port = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){		//socket resulted in failure
		error("Socket not generated\n");
	}

	server = gethostbyname(argv[1]);
	if(server == NULL)
			error("Error in finding host!");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	memcpy(server->h_addr, &server_addr.sin_addr.s_addr, server->h_length);

	server_addr.sin_port = htons(port);
	if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
		error("Failed to connect!\n");

	while(1){
		memset(buffer, 0, 255);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, strlen(buffer));
		if(n<0)
			error("Failed to Write! \n");

		memset(buffer, 0, 255);
		n = read(sockfd, buffer, 255);
		if(n<0)
			error("Failed to Read \n");

		printf("Server: %s\n", buffer);

		int i = strncmp("Bye", buffer, 3);
		if(!i)
			break;
	}
	close(sockfd);

	return 0;
}


