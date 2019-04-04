#include <iostream>
#include <cstring>
#include "include/sockLib.h"
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

int main(int argc, char const *argv[])
{
	if ( argc < 2 ) {
		cout << "<port>" << endl;
		return 0;
	}

	int s, sockfd;
	struct sockaddr_in server;

	s = Socket();

	initSockaddr( &server, atoi( argv[1] ) );

	Bind( s, &server );

	Listen( s, 2 );

	sockfd = Accept( s );

	char msg[128];
	int n;

	for (;;) {
		n = read( sockfd, msg, sizeof(msg) );
		if ( n < 0 ) {
			perror( "[-] read call failed" );
			close( s );
			exit( 1 );
		}
		else if ( n == 0 ) {
			cout << "EOF" << endl;
			break;
		}
		msg[n] = '\0';

		cout << "Client: " << msg << endl;

		n = write( sockfd, msg, strlen( msg ) );
		if ( n < 0) {
			perror( "[-] write call failed" );
			close( s );
			exit( 1 );
		}

		if ( strcmp( msg, "exit()" ) == 0 )
			break;
	}

	close( sockfd );
	cout << "[-] disconnected." << endl;

	close( s );
	cout << "[-] server socket is closed." << endl;
 
	return 0;
}