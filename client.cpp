#include <iostream>
#include <cstring>
#include "include/sockLib.h"
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

int main(int argc, char const *argv[])
{
	if ( argc < 3 ) {
		cout << "<host> <port>" << endl;
		return 0;
	}

	int s;
	struct sockaddr_in server;

	s = Socket();

	initSockaddr( &server, argv[1], atoi( argv[2] ) );

	Connect( s, &server );

	char msg[128];
	int n;

	for (;;) {
		n = read( 0, msg, sizeof(msg) );
		if ( n < 0 ) {
			perror( "[-] read call failed" );
			close( s );
			exit( 1 );
		}
		n--;
		if ( n == 0 ) break;
		msg[n] = '\0';

		n = write( s, msg, strlen( msg ) );
		if ( n < 0) {
			perror( "[-] write call failed" );
			close( s );
			exit( 1 );
		}

		n = read( s, msg, sizeof(msg) );
		if ( n < 0 ) {
			perror( "[-] read call failed" );
			close( s );
			exit( 1 );
		}
		msg[n] = '\0';

		cout << "Server: " << msg << endl;

		if ( strcmp( msg, "exit()" ) == 0 ) break;
	}

	close( s );
	cout << "[-] disconnected from server." << endl; 

	return 0;
}