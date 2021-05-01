// int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval_ *timeout);
//여러개의 fd를 파이프를 동시에 처리 한다.
//모든 디바이스가 처리 될수 있도록한다.
// nfds는 전체 discriptor의 개수의 +1개로 할당한다.
// 비트 마스크 를 사용하여 Fileset를 설정한다.

//FD_ZERO fdset을 0으로 초기화
//FD_SET(int fd, fd_set fd);// 특정 fd 번호를 1로 변경
//FD_ISSET(int fd, fd_set *fdset); // 특정 fd번호가 fdset에서 1로 되어 있는지 반환
//void FD_CLR(int fd, fd_set *fdset); // 특정 fd번호를 fd set에서 0으로 초기화

//timeout은 ms단위로 시간을 설정할수 있으며 일정시간 멈춘후(sleep) select함수를 실행한다.
//반환값 에러시 -1, 준비된 descriptor가 없으면 0, 이외에는 준비된 파일의 개수를 반환한다.
//그래서 함수 내부에서는 준비된 파일의 개수를 바로 알수 없으며 반목분을 통해 체크해야함
//비효율적임
