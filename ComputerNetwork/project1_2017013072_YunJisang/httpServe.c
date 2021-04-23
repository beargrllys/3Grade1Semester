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

void fill_header(char *header, int status, long len, char *type){//status에 따라  헤더 메세지 구성
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
    sprintf(header, HEADER_FMT, status, status_text, len, type);// 헤더 형식에 작성
}

void type_detect(char *file_type, char *uri){//distribte http msg type
    char *extension= strrchr(uri, '.');//뒤에서 부터 문자열 검색하는 기능
    if(!strcmp(extension, ".html"))
        strcpy(file_type, "text/html");
    else if(!strcmp(extension, ".jpg") || !strcmp(extension, ".jpeg"))
        strcpy(file_type, "image/jpeg");
    else if(!strcmp(extension, ".png"))
        strcpy(file_type, "image/png");
    else if(!strcmp(extension, ".gif"))
        strcpy(file_type, "image/gif");
    else if(!strcmp(extension, ".pdf"))
        strcpy(file_type, "application/pdf");
    else
        strcpy(file_type, "text/plain");
}

void packet_Maker(int accept_sock){
    int status = 200;
    char header[BUF_SIZE];
    char buf[BUF_SIZE];
    if(read(accept_sock, buf, BUF_SIZE) <0){//accept_sock 소켓에서 받아온 메세지를 buf버퍼에 저장한다.
        perror("ERROR Failed to read request : 500");
        return;
    }
    printf("Request Msg:\n%s\n\n", buf);
    char *method = strtok(buf, " ");//accept된 메세지를 파싱한다. GET, POST...
    char *dir = strtok(NULL, " ");// 필요한 디렉토리 저장
    if(method == NULL || dir == NULL){
        perror("ERROR Failed to identify method, uri\n");//메세지 수신이 안됨
        return;
    }

    char safe_uri[BUF_SIZE];
    char *local_uri;
    struct stat st;//파일 검색을 위한 stat 구조체
    strcpy(safe_uri, dir);//필요한 콘텐츠 접수
    if(!strcmp(safe_uri, "/")){//default page
        strcpy(safe_uri, "/index.html");
        local_uri = safe_uri + 1;//앞에 /없에기
    }
    else{
        local_uri = safe_uri + 1;
    }
    if(stat(local_uri, &st) <0){//stat함수를 이용해 local_uri에 대한 stat구조체 저장
        perror("404 Not Found\n");
        status = 404;
        return;
    }

    int fd = open(local_uri, O_RDONLY);//파일 디스크립터 열람
    if(fd < 0){
        perror("ERROR Fail to open\n");
        status = 500;
        return;
    }

    int file_len = st.st_size;//해당 파일의 크기
    char file_type[40];
    type_detect(file_type, local_uri);// 파일 종류 확인
    fill_header(header, status, file_len, file_type);
    //header 구성함, HEADER_FMT에 필요한 데이터 저장
    write(accept_sock, header, strlen(header));
    //최종적으로 만들어진 헤더를 소켓 file discriptor에 쓰기

    int cnt;
    while ((cnt = read(fd, buf, BUF_SIZE)) > 0){// send http msg in application data
        write(accept_sock, buf, cnt);
    }
}

int main(int argc, char **argv)//parameter is only request filename
{
    int port, pid;
    int listen_sock, accept_sock;
    struct sockaddr_in remote_sin;//소켓의 주소를 담되 sa_family가 AF_INET인 경우이다.
    socklen_t remote_sin_len;//소켓 관련 매개변수 저장소
    
    if(argv[1] < 0 || argc <= 0){//포트 관련 입력값 유효성 확인
        printf("Port Error\n");
        exit(0);
    }

    port = atoi(argv[1]);//포트번호 저장

    listen_sock = socket(AF_INET,SOCK_STREAM, 0);//스트림 방식의 입출력 소켓생성
    if(listen_sock < 0){//소켓 생성 실패
        perror("ERROR failed to create listen_sock\n");
        exit(1);
    }

    if(bind_socket(listen_sock, port) < 0){//소켓 바인딩
        perror("ERROR failed to bind socket\n");
        exit(1);
    }
    if(listen(listen_sock, 10) < 0){//접속이 들어올때까지 대기
        perror("ERROR failed to listen socket\n");
        exit(1);
    }

    while(1){
        //printf("Waiting for you \n");
        accept_sock = accept(listen_sock, (struct sockaddr *)&remote_sin , &remote_sin_len);
        //클라이언트의 sockaddr_in을 서버에게 전달
        if(accept_sock < 0){
            perror("ERROR failed to accept\n");
            continue;
        }

        pid = fork();//request를 듣고 response를 돌려주는 자식프로세스 생성
        if(pid == 0){
            close(listen_sock);
            packet_Maker(accept_sock);//response를 제작하며 전송해주는 함수
            close(accept_sock);
            exit(0);//자식프로세스 종료
        } 
        if(pid != 0){
            close(accept_sock);
        }
        if(pid < 0){
            perror("ERROR failed to fork\n");
        }
    }


}