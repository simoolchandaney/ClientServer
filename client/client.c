/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <time.h>
#include <sys/time.h>

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

double timestamp() {
    struct timeval current_time;
    if (gettimeofday(&current_time, NULL) < 0) {
        return time(NULL);
    }
    return (double) current_time.tv_sec + ((double) current_time.tv_usec / 1000000.0);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes, filename;  
    char buf[BUFSIZ];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 4) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // argv[1] - IP
    // argv[2] - port number
    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo); // all done with this structure

    // send file name to server 
    // argv[3] - file name
    double t_init_f = timestamp();
    if ((filename = send(sockfd, argv[3], strlen(argv[3]), MSG_CONFIRM)) == -1) {
        perror("recv");
        exit(1);  
    }

    // receive # of bytes from server (transform from network to host order)
    // loop until we receive complete file
    if ((numbytes = recv(sockfd, buf, BUFSIZ-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    printf("numbytes: %d\n", numbytes);
    numbytes = htons(numbytes);
    printf("numbytes: %d\n", numbytes);

	char new_filename[BUFSIZ] = "client/";
	strcat(new_filename, argv[3]);

    FILE *fp = fopen(new_filename, "w");
    char buffer[BUFSIZ];

    while(1) {
        if(recv(sockfd, buffer, BUFSIZ, 0) <= 0)
            break;
        //printf("LINE: %s\n", buffer);
        fprintf(fp, "%s", buffer);
    }
    double t_final_f = timestamp();
    double time_elapsed = t_final_f - t_init_f;
    double speed = numbytes*(0.000001) / time_elapsed*(0.000001);
    printf("%d bytes transferred over %lf seconds for a spoeed of %lf MB/s\n", numbytes, time_elapsed*(0.000001), speed);

    fclose(fp);
    close(sockfd);
    return 0;
}
