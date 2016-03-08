/* From 
  ECHOSERV.C
  ==========
  (c) Paul Griffiths, 1999
  Email: mail@paulgriffiths.net
  
  Simple TCP/IP echo server.
  * Adatted by TB
  * Da provare con 
  * telnet 127.0.0.1 2003
*/

#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/*  Global constants  */

#define ECHO_PORT	(2003)
#define MAX_LINE	(1000)
#define LISTENQ		(1024)   /*  Backlog for listen()   */

void upcase(char*,int);
int Binding();

int main(int argc, char *argv[]) 
{

	int       conn_s;                /*  connection socket         */
	int i=0;
	int list_s;
	char      buffer[MAX_LINE];      /*  character buffer          */
	char BYEE[]="BYEE";
	int n,X;

	list_s=Binding();
	
	/*Listen on port for connections*/
	if ( listen(list_s, LISTENQ) < 0 ) 
	{
		fprintf(stderr, "ECHOSERV: Error calling listen()\n");
		exit(EXIT_FAILURE);
	}

	/*  Enter an infinite loop to respond to client requests and echo input  */
	X=1;	
	while ( X==1 ) 
	{
		/*  Wait for a connection, then accept() it  */
		if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) 
		{
		     fprintf(stderr, "ECHOSERV: Error calling accept()\n");
		     printf("ECHOSERV: Error calling accept()\n");
		     exit(EXIT_FAILURE);
		}
		else printf("Connection accepted: \n \t");
		/*  Retrieve an input line from the connected socket then simply write it back to the same socket.     */		    
		/*Gestione della connessione*/
		printf("cnns=%d\n",conn_s);
		do
		{
			memset(buffer,0,MAX_LINE);
			recv(conn_s,buffer, MAX_LINE,0);
			
			n=strlen(buffer);
			if ( buffer[strlen(buffer)-1]=='\n') buffer[strlen(buffer)-1]='\0';
			printf("\t got \t\"%s\"\t!\n",buffer);
			buffer[strlen(buffer)]='\n';
			upcase(buffer,n);
			
			send(conn_s,buffer, strlen(buffer),0);
		}
		while (strncmp(buffer,BYEE,4)!=0);
		/*  Close the connected socket  */
		i=close(conn_s);
		if ( i < 0 ) 
		{
		    fprintf(stderr, "ECHOSERV: Error calling close()\n");
		    printf( "ECHOSERV: Error calling close() i=%d\n",i);
		    exit(EXIT_FAILURE);
		}
		else  printf("Connection closed: i=%d\n ",i);
		if (buffer[4]=='0') break;
	}	/* while */
	return 0;
    
}	/* main() */

int Binding()
{
	int i=0;
	int list_s;			/*  listening socket          */
	struct sockaddr_in  servaddr;	/*  socket address structure  */
	short int port = ECHO_PORT;	/*  port number               */
	do
	{    
	    printf("Echoserver-simple starting. Try to Listen on %i port.\n",port);
	    /*  Create the listening socket  */
	    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
	    {
		fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
		/* exit(EXIT_FAILURE); */
	    }

	    /*  Set all bytes in socket address structure to zero, and fill in the relevant data members   */

	    memset(&servaddr, 0, sizeof(servaddr));
	    servaddr.sin_family      = AF_INET;
	    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	    servaddr.sin_port        = htons(port);

	    /*  Bind our socket addresss to the listening socket, and call listen()  */
	    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) 
	    {
		fprintf(stderr, "ECHOSERV: Error calling bind()\n");
		/*exit(EXIT_FAILURE);*/
	    }
	    else i=1;
	    if (i!=1)	port++;
	}
	while(i==0);
	printf("Server: listening on %i port. \n",port);
	return list_s;
}

void upcase(char* S,int n)
{
	for (;n>=0;--n)
		if (S[n]<='z' && S[n]>='a') S[n]=S[n]-'a'+'A';
}


