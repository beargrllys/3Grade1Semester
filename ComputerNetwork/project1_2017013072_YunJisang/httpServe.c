#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1000
#define HEADER_FMT "HTTP/1.1 %d %s\nContent-Length: %ld\nContent-Type: %s\n\n" 


int bind_socket(int lsock, int port){
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    return bind(lsock, (struct sockaddr *)&sin, sizeof(sin));//소켓을 binding합니다. 실패할경우 -1을 반환합니다.
} 

void fill_header(char *header, int status, long len, char *type){
    char status_text[40];
    switch (status) {
        case 200:
        strcpy(status_text, "OK");
        break;
        case 404:
        strcpy(status_text, "Not Found");
        break;
        default:
        strcpy(status_text, "Internal Server Error");
        break;
    }
    sprintf(header, HEADER_FMT, status, status_text, len, type);
}

void file_type(char *cy_type, char *uri){//distribte http msg type
    char *ext = strrchr(uri, '.');
    if(!strcmp(ext, ".html"))
        strcpy(cy_type, "text/html");
    else if(!strcmp(ext, ".jpg") || !strcmp(ext, ".jpeg"))
        strcpy(cy_type, "image/jpeg");
    else if(!strcmp(ext, ".png"))
        strcpy(cy_type, "image/png");
    else if(!strcmp(ext, ".gif"))
        strcpy(cy_type, "image/gif");
    else if(!strcmp(ext, ".pdf"))
        strcpy(cy_type, "application/pdf");
    else
        strcpy(cy_type, "text/plain");
}

void http_handler(int asock){
    char header[BUF_SIZE];
    char buf[BUF_SIZE];
    if(read(asock, buf, BUF_SIZE) <0){//asock 소켓에서 받아온 메세지를 buf버퍼에 저장한다.
        perror("ERROR Failed to read request : 500");
        return;
    }
    printf("Request Msg:\n%s\n\n", buf);
    char *method = strtok(buf, " ");//accept된 메세지를 파싱한다.
    char *dir = strtok(NULL, " ");
    if(method == NULL || dir == NULL){
        perror("ERROR Failed to identify method, uri\n");//메세지 수신이 안됨
        return;
    }

    char safe_uri[BUF_SIZE];
    char *local_uri;
    struct stat st;
    strcpy(safe_uri, dir);
    if(!strcmp(safe_uri, "/")){//default page
        strcpy(safe_uri, "/index.html");
        local_uri = safe_uri + 1;
    }
    else{
        local_uri = safe_uri + 1;
    }
    if(stat(local_uri, &st) <0){//해당하는 파일이 없을 때 에러처리
        perror("No file in URI\n");
        return;
    }

    int fd = open(local_uri, O_RDONLY);//파일 열람
    if(fd < 0){
        perror("ERROR Fail to open\n");
        return;
    }

    int ct_len = st.st_size;
    char ct_type[40];
    file_type(ct_type, local_uri);// check File type
    fill_header(header, 200, ct_len, ct_type);//header 구성하디
    //printf("HEADER is\n\n%s", header);
    write(asock, header, strlen(header));//send http msg

    int cnt;
    while ((cnt = read(fd, buf, BUF_SIZE)) > 0){// send http msg in application data
        write(asock, buf, cnt);
    }
}

int main(int argc, char **argv)//parameter is only request filename
{
    int port, pid;
    int lsock, asock;
    struct sockaddr_in remote_sin;//소켓의 주소를 담되 sa_family가 AF_INET인 경우이다.
    socklen_t remote_sin_len;//소켓 관련 매개변수 저장소
    
    if(argv[1] < 0 || argc <= 0){//포트 관련 입력값 유효성 확인
        printf("Port Error\n");
        exit(0);
    }
    //printf("Try to Connect Port number: %s\n", argv[1]);

    port = atoi(argv[1]);

    lsock = socket(AF_INET,SOCK_STREAM, 0);//try conquer socket internal
    if(lsock < 0){
        perror("ERROR failed to create lsock\n");
        exit(1);
    }
    if(bind_socket(lsock, port) < 0){//try bind socket internal
        perror("ERROR failed to bind socket\n");
        exit(1);
    }
    if(listen(lsock, 10) < 0){//try to open & listen socket
        perror("ERROR failed to listen socket\n");
        exit(1);
    }
    signal(SIGCHLD, SIG_IGN);

    while(1){
        //printf("Waiting for you \n");
        asock = accept(lsock, (struct sockaddr *)&remote_sin , &remote_sin_len);//change port mode for accept
        if(asock < 0){
            perror("ERROR failed to accept\n");
            continue;
        }

        pid = fork();//Order making http msg to child process
        if(pid == 0){
            close(lsock);
            http_handler(asock);//child process's work
            close(asock);
            exit(0);//Child process work Done!!
        } 
        if(pid != 0){
            close(asock);
        }
        if(pid < 0){
            perror("ERROR failed to fork\n");
        }
    }


}