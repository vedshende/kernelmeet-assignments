/*
	the file contains some problem related to socket
	you can only retrieve only 1358 bytes data.
	i am unable to figure it out
	rest everything going fine
	tested on a small website(www.example.com/index.html)
	static program url isn't taken from user due to above problem
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>//for hostent structure
#include <string.h>
int main() {
	
	char request[] = 
			"GET /index.html HTTP/1.0\r\n"
			"\n";
		//a blank line is left as the header format has a standard to leave a line between header and body
	char buf[1024*1024];
	int num_of_bytes_recv;
	
	// create the socket
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock<0){
		perror("problem with socket creation");	
		close(sock);
		exit(1);
	}
	//setup an address
	else{
		printf("socket created");	

		struct sockaddr_in server_address;
		struct hostent *hentity;
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(80);
	
		/*
		this part is of immense importance else the system hangs as no address is resolved,also the structure is important
		can there be any alternative for the structure?
		*/
		

		//optionally you could have used getaddrinfo()
		hentity=gethostbyname("www.example.com");
		memcpy((char *)&server_address.sin_addr.s_addr, (char *)hentity->h_addr,hentity->h_length);
		
		//inet_aton("www.google.com", &server_address.sin_addr.s_addr);


		int confd=connect(sock, (struct sockaddr *) &server_address, sizeof(server_address));
		if(confd<0){
			perror("connection problem:");
			close(sock);
			exit(-1);
		}
		else{
			printf("connection established ");
			send(sock, request,strlen(request), 0);//why strlen?=as the request message's size should be sent
			printf("sent a http request to server");
			num_of_bytes_recv=recv(sock, &buf, 1024*1024, 0);
			printf("data bytes recv %d",num_of_bytes_recv);

			char *found=strstr(buf,"\r\n\r\n");//followed the RFC,where body of HHTP starts
			printf("\n i got the file ready for u.just check it!:)%s",found);
		
			//creating a file now

			FILE *fp;
			fp=fopen("somethingcool.html","w+");
			int k=0;
			while(*found!='\0'){
				fputc(*found,fp);		
			found++;
			}
			fclose(fp);
			
			
		}
	
	

	}
	close(sock);

	return 0;
}

