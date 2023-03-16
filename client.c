//209351147
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    int i = 1;
    char *strh = "http://";
    char *host = NULL;
    char *path = NULL;
    char *port = NULL;
    char *textOfP = NULL;
    char arguments[500] = "";
    int num_port = -1;
    int nOfR = -1;
    int nOfP = -1;
    int flagForR = 0;
    int flagForP = 0;
    int flagForHTTP = 0;
    int index_of_nOfP = -1;
    char *post = "POST";
    char *get = "GET";
    int pathAlloc = 0;

    if(argv[i] == NULL){
        printf( "Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");     //Usage message
        exit(1);
    }

    while (i < argc) {
        // check the char '-' //

        if (argv[i][0] == '-') {   //check that the char arr[i][1] == (p or r)
            if (argv[i][1] != 'r' && argv[i][1] != 'p') {
                printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                exit(1);
            }

//////////////////////////////////check -r ////////////////////////////////////////////

            if (argv[i][1] == 'r') {
                if(argv[i + 1] == NULL){
                    printf( "Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");     //Usage message
                    exit(1);
                }
                if(flagForR == 1){     // there is only 1 '-r' in the command
                    printf( "Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");     //Usage message
                    exit(1);
                }
                flagForR = 1;        //update
                i++;  // i = 2
                int temp = i;
                for (int t = 0; t < strlen(argv[temp]); t++) {
                    if (isdigit(argv[i][t]) == 0) { //check that this string is a number and not include other chars, return 0 if false
                        printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");   //Usage message
                        exit(1);
                    }
                }
                nOfR = atoi(argv[i]);    //convert string to int        --Content-length--
                int counterOfEqual = 0;
                int flag = 0;  //flag to show if we found '=' in the word
                i++; //i = 3
                int argumIndex = 0;         //index of chars in arguments[]
                arguments[argumIndex] = '?';
                argumIndex++;
                for (int j = 0; j < nOfR && i + j < argc; j++) { //numbers of words searching by spaces
                    int y = 0; //index of chars in arr
                    flag = 0;
                    while (argv[i + j][y] != '\0') {   //this is not the end of the word
                        if (argv[i + j][y] == '=' && y != (strlen(argv[i + j])) && y != 0) {        //the char '=' is not the first or the last char in the string
                            counterOfEqual++;
                            flag = 1;
                        }
                        arguments[argumIndex] = argv[i + j][y];                     //copy the args into string
                        argumIndex++;
                        y++;
                    }
                    if (flag == 0 && j != 0) {
                        printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");   //Usage message
                        exit(1);
                    }
                    if (j < nOfR - 1) {                  //add & except to the last word
                        arguments[argumIndex] = '&';
                        argumIndex++;
                    }
                }
                arguments[argumIndex] = '\0';       //close the string with '\0'

                if (counterOfEqual < nOfR) {
                    printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");     //Usage message
                    exit(1);
                }
                if (argv[i + 1])
                    i += nOfR - 1;
                if (i > argc) {
                    break;
                }
            }

//////////////////////////////////check -p ////////////////////////////////////////////

            if (argv[i][1] == 'p') {  // i = 1
                if(argv[i + 1] == NULL){
                    printf( "Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");     //Usage message
                    exit(1);
                }
                if(flagForP == 1){  // there is only 1 '-p' in the command
                    printf( "Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");     //Usage message
                    exit(1);
                }
                flagForP = 1;   //update
                i++; // i = 2
                for (int t = 0; t < strlen(argv[i]); t++) {            //check that this string is a number and not include other chars, return 1 if true
                    if (isdigit(argv[i][t]) == 0) {
                        printf( "Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");     //Usage message
                        exit(1);
                    }
                }
                nOfP = atoi(argv[i]);    //convert string to int      --Content-length
                index_of_nOfP = i;
                if (nOfP == -1) {
                    printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");         //Usage message
                    exit(1);
                }
                i++; // i = 3

                if (nOfP > strlen(argv[i])) {      //n is bigger than the length of the text
                    printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");     //Usage message
                    exit(1);
                }
                textOfP = (char *) malloc(1 + strlen(argv[i]) * sizeof(char));               //allocation
                if (textOfP == NULL) {
                    printf("allocation fail");
                    exit(1);
                }
                int d = 0;  //index for textOfP
                for (; d < nOfP; d++) {
                    textOfP[d] = argv[i][d];             //copy the text to textOfP
                }
                textOfP[d++] = '\0';       //close the string with '\0'
            }
        }

///////////////////////// there is no char '-', then this is URL /////////////////////////////////////////
        else {
            if(flagForHTTP == 1){ // there is only 1 HTTP in the command
                printf( "Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");     //Usage message
                exit(1);
            }
            flagForHTTP = 1;
            int count = 0;
            int startPathIndex = -1;
            int colonIndex = -1;
            int first = 0;
            char *temp1 = (char *) malloc(8 * sizeof(char));                       //allocation
            if (temp1 == NULL) {
                perror("MALLOC FAIL");
                exit(1);
            }
            for (int e = 0; e < 7; e++) {    //fill temp1 with the 7 chars of the string in order to check if its "http://
                temp1[e] = argv[i][e];
            }
            temp1[7] = '\0';

            if (strcmp(temp1, strh) == 0) {   //return 0 when success, and it's mean that its start with http://
                int t = 7;
                while (argv[i][t] != '/' || argv[i][t] != ':') {
                    if (argv[i][t + 1] == '/') {
                        startPathIndex = t + 1;    //the start index of the path
                        if (first == 0) { first = 2; }
                        break;
                    } else if (argv[i][t + 1] == ':') {
                        colonIndex = t + 1;
                        if (first == 0) { first = 1; }
                    }
                    count++;
                    t++;
                    if (t == strlen(argv[i]) - 1) {
                        break;
                    }
                }

                host = (char *) malloc(sizeof(char) * (count + 2));                 //allocation
                if (host == NULL) {
                    perror("MALLOC FAIL");
                    exit(1);
                }
                int indexOfHost = 0;
                if (first == 1) {
                    for (int k = 7; k < colonIndex; k++) {          //copy www.../  to the host
                        host[indexOfHost] = argv[i][k];
                        indexOfHost++;
                    }
                } else if (first == 2) {
                    for (int k = 7; k < startPathIndex ; k++) {          //copy www.../  to the host
                        host[indexOfHost] = argv[i][k];
                        indexOfHost++;
                    }
                } else {
                    for (int k = 7; k <= count + 7; k++) {          //copy www.../  to the host
                        host[indexOfHost] = argv[i][k];
                        indexOfHost++;
                    }
                }
                host[count + 1] = '\0';

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// checked

                if (startPathIndex != -1) {  //there is a path
                    path = (char *) malloc(1 + (strlen(argv[i]) - startPathIndex) * sizeof(char));          //allocation
                    if (path == NULL) {
                        perror("MALLOC FAIL");
                        exit(1);
                    }
                    pathAlloc = 1;
                    int index1 = 0;  //init
                    for (int s = startPathIndex; s < strlen(argv[i]); s++) {
                        path[index1] = argv[i][s];                        //copy the path
                        index1++;
                    }
                    path[index1] = '\0';
                }


                if (colonIndex != -1) {       //there is a port
                    int index2 = 0;
                    port = (char *) malloc ((strlen(argv[i]) - colonIndex) * sizeof(char));
                    if (port == NULL) {
                        perror("MALLOC FAIL");
                        exit(1);
                    }

                    for (int l = colonIndex + 1; argv[i][l] != '/' || l < strlen(argv[i]) - 1; l++) {          //copy the chars after the char ':'
                        port[index2] = argv[i][l];
                        index2++;
                    }
                    port[index2] = '\0';
                    num_port = atoi(port);
                    if (num_port <= 0 || num_port > 65536) {
                        printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                        exit(1);
                    }
                } else {
                    num_port = 80;       //default
                }
            }
            else {
                printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                free(temp1);
                exit(1);
            }
            if(temp1 != NULL) {
                free(temp1);
            }
        }
        i++;
    }
    if (path == NULL) {
        path = "/";
    }
    char* request = NULL;
    request = (char*) malloc(sizeof(char));
    if(request == NULL){
        perror("MALLOC FAIL");
        exit(1);
    }
    if (textOfP == NULL){                      //there is no -p in there request => print GET
        request = (char*) realloc(request, strlen(get) + 1 + strlen(path) + strlen(arguments) + strlen(" HTTP/1.0\r\nHost: ") + strlen(host)+ strlen("\r\n\r\n") + 1);
        strcat(request, get);
        strcat(request, " ");
        strcat(request, path);
        strcat(request, arguments);
        strcat(request, " HTTP/1.0\r\n");
        strcat(request, "Host: ");
        strcat(request, host);
        strcat(request, "\r\n\r\n");
    }
    else {                                    //there is -p => print POST
        request =(char*) realloc(request, strlen(post) + 1 + strlen(path) + strlen(arguments) + strlen(" HTTP/1.0\r\nHost: ") +
                                          strlen(host)+ strlen("\r\nContent-length:")+ strlen(argv[index_of_nOfP]) +
                                          strlen("\r\n\r\n") + strlen(textOfP)+
                                          strlen("\r\n") + 1);
        strcat(request, post);
        strcat(request, " ");
        strcat(request, path);
        strcat(request, arguments);
        strcat(request, " HTTP/1.0\r\n");
        strcat(request, "Host: ");
        strcat(request, host);
        strcat(request, "\r\n");
        strcat(request, "Content-length:");
        strcat(request, argv[index_of_nOfP]);
        strcat(request, "\r\n\r\n");
        strcat(request, textOfP);
//        strcat(request, "\r\n");
    }
    memset(arguments,'\0', sizeof(arguments));
    if(strcmp(request, "") == 0){
        printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
        exit(1);
    }
    printf("HTTP request =\n%s\nLEN = %lu\n", request, strlen(request));   //asked

    if (host == NULL) {
        printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
        exit(1);
    }

///////////////////creating socket

    int sd;        /* socket descriptor */
    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    struct hostent *hp; /*ptr to host info for remote*/
    struct sockaddr_in peeraddr;

    peeraddr.sin_family = AF_INET;
    hp = gethostbyname(host);
    if (hp == NULL){
        herror("there is no ip address");
        if(host != NULL){                      //frees
            free(host);
        }
        if(pathAlloc == 1){
            free(path);
        }
        if(textOfP != NULL){
            free(textOfP);
        }
        if(port != NULL){
            free(port);
        }
        close(sd);
        exit(1);
    }
    peeraddr.sin_addr.s_addr = ((struct in_addr *) (hp->h_addr))->s_addr;
    peeraddr.sin_port = htons(num_port);

////////////////////////connect//////////////////////////////////////////////////

    if (connect(sd, (struct sockaddr *) &peeraddr, sizeof(peeraddr)) < 0) {
        perror("connect");
        exit(1);
    }
    int sent = 0;
    sent = write(sd, request, strlen(request));
    if (sent < 0) {
        perror("write error");
        exit(1);
    }
    int num_of_chars_in_response = 0;
    int countRead;
    unsigned char buffer[1024] = "";
    while(1) {
        if ((countRead = read(sd, buffer, sizeof(buffer))) < 0) {
            perror("read error");
            exit(1);
        }
        if(countRead == 0){ break;}
        buffer[countRead] = '\0';
        write(STDOUT_FILENO,buffer, sizeof(buffer));
        num_of_chars_in_response += countRead;
    }
    printf("\n Total received response bytes: % d\n",num_of_chars_in_response);
    close(sd);
    if(host != NULL){           //frees
        free(host);
    }
    if(pathAlloc == 1){
        free(path);
    }
    if(textOfP != NULL){
        free(textOfP);
    }
    if(port != NULL){
        free(port);
    }
    free(request);
}
