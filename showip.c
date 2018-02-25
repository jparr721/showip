#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>



const char* print_usage() {
	return "Usage: showip [options] hostname\noptions:\n-h Display help\n-s  Show ip and save a log of the request";
}

void store_shown_data(char* data) {
	printf("Storing output...");
	FILE* data_file = fopen("data.txt", "w");
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	fprintf(data_file, "%d-%d-%d:	 %s", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, data); 
	fclose(data_file);
}

void show_ip(char* host) {
	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(host, NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(0);
	}

	printf("IP address for %s:\n\n", host);
	for (p = res; p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;

		// Get the pointer to the address
		if (p->ai_family == AF_INET) {
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		char* long_addr = (char *) malloc(1 + strlen(ipver) + strlen(ipstr));
		strcpy(long_addr, ipstr);
		strcpy(long_addr, ipver);
		store_shown_data(long_addr);
		printf(" %s: %s\n", ipver, ipstr);
	}

	freeaddrinfo(res); // free the linked list
}

int main(int argc, char* argv[]) {
	if (strcmp("-h", argv[1]) == 0) {
		printf("%s", print_usage());
		exit(1);
	} 
	if (strcmp("-s", argv[1]) == 0) {
		show_ip(argv[2]);
		exit(1);
	}
}
