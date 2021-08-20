/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calcula.h"


void
prog_100(host)
char *host;
{
	CLIENT *clnt;
	int  *result_1;
	operandos  add_100_arg;
	int  *result_2;
	operandos  sub_100_arg;
	clnt = clnt_create(host, PROG, VERSAO, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}
	result_1 = add_100(&add_100_arg, clnt);
	if (result_1 == NULL) {
		clnt_perror(clnt, "call failed:");
	}
	result_2 = sub_100(&sub_100_arg, clnt);
	if (result_2 == NULL) {
		clnt_perror(clnt, "call failed:");
	}
	clnt_destroy( clnt );
}


main(argc, argv)
int argc;
char *argv[];
{
	char *host;

	if(argc < 2) {
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}
	host = argv[1];
	prog_100( host );
}
