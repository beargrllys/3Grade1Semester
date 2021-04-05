### Application Layer

굳이 사용자가 Network 뒤에서 일어나는 일을 알 필요는 없다.

Server Client Diagram

Server: 항상 켜져 있음, 고정 IP

Client: 일시적인 연결이 가능, 항상 켜져있지 않음, 유동 IP

Process Communicating: 서로 다른 머신 사이에 서로 다른 프로세스간의 통신

Socket: application과 OS, network 사이의 interface 

Addressing Processes: 각 소켓은 고유의 IP와 Port Number를 가지고 있다.

### 네트워크의 종류와 판단 지표

Transport service requirement: data loss, thourput, time sensitive

Internet Application(FTP, HTTP...) And Transport Protocols(TCP,UDP)

HTTP(Hypertext Tranfer Protocol): 오직 HTTP request와 HTTP response로 이루어짐

기본적으로 80번 포트를 쓰며 HTTP연결 전에 TCP연결을 수행한다. 전송정보를 따로 기록하지 않는다.

HTTP의 종류: Non-persistent HTTP, Persistent HTTP 

### Non -persistent HTTP EX)

Client: TCP Connect Request   --(1)-->  Server: HTTP "Accept" Request

​                                                      <--(2)--

Client: HTTP Request message --(3)--> Server: Receive Request Message

​                                                       <--(4)-- Server: Send response message And Http Close TCP Connection

Client: Repeat this steps for recieve all data



RTT(Route Time): Client가 requst를 보내고 Server가 response가 받기까지 드는 시간

최종적으로 Non -persistent HTTP에서는 2RTT+Transimmision time이 소요된다. 

Persistent라면 RTT가 지속적으로 들지 않는다.



### HTTP Request Message

일정한 구조가 있으나 굳이 상세히 알필요는 없음

![http message format: request](http://www.cs.umd.edu/~shankar/417-F01/Slides/chapter2a-aus/img016.gif)

### Coockies

클라이언트에 임시파일을 제공하고 Client와 관련된 정보를 담았다가 나중에 동일 서버와 연결할때 좀더 바르게 연결할수 있고 State를 관리하기 편하기 위해 Cookie를 사용한다. 하지만 개인정보를 담고 있기 때문에  Privacy issue가 존재하기도 한다.



###  Web Cache

Origin Server를 거치지 않고 Client의 응답을 처리하기 위해 만들어진 서버

들어온 Request가 Web cache에 있는자 없는지 확인하는 과정을 거쳐야한다.

있으면 Webcache선에서 처리하고 없으면 origin server에 접속하여 데이터를 가져온다.

Client는 빠르게 응답을 받아 볼수있고 Server에 Traffic의 회선 사용률을 분산시켜 정리 할수있다.

> Web Cache도 일종의 서버인가? 웹캐시 서버는 얼마나 배치해야 하는가?

Caching EX: 만약 웹캐시 없이 순수하게 Origin server에 접속하게 된다면 

![img](https://user-images.githubusercontent.com/45806836/98209027-9e2e2a80-1f81-11eb-9c9c-0c7b46450d1d.png)

브라우져의 avg rate가 1.50인데 instiutional network에서 public internet으로 들어가는 access link가 1.54거의 모든 대역폭을 사용하고 있다. 이처럼 client에서 100KB오브젝트를 초당 15개씩, 총 1.5Mb/s씩 사용중이지만 access link에 1.54Mbps를 거의 사용하여 bottle neck이 형성된다. 그래서 총 지연시간은 Origin Server에 connect 하는 RTT 2초 + (총 요구 데이터량/1.5Mbps) +client에 접속하는 얼마안되는 시간이 들게된다.

(institute route - Server RTT) + (Web object Transmission Time) + (institutional network delay:거의 없지만 어쨌든)

다음과 같은 다음과 같은 상황을 타개하기 위한 방법은 2가지가 있다.

1. access Link의 대역폭을 늘려버린다.

access link를 100 배 늘리면 대역폭 사용량도 원래 100% 가까이 사용하던걸 1% 대로 무리없이 데이터 전송이 가능해진다.

단순무식한 방법이지만 성능 확실하다. 대신 추가 비용이 든다.

2. Web cache를 설치한다.

외부회선과 내부 회선에 Webcache를 설치하고 Webcache 서버는 사용자의 40% 요청을 Webcache에서 처리 가능하다고 가정한다. (hit rate) 실질적인 사용자가 사용하는 사이트가 일반적으로 일정하기에 hit rate 자체는 높다.

그래서 40% 트래픽은 내부에서 해결 가능하며 나머지 60% 만 평균적으로 1.5Mbps중 60%인 0.9Mbps의 트래픽이 외부 public server로 들어가게된다. 1.54Mbps의 Access link는 이정도 처리량은 처리할수 있으며 이 부분의 2초의 RTT가 소요 된다. 

그래서 평균 소요 시간은 0.6\*2.01 + 0.4\*(ms단위의 찰나) = 평균 1.2s 정도 걸려 모든 데이터가 2초씩은 기본적으로 소모되는 대역폭 증설 의 방법 보다도 더 좋은 효과를 볼수 있게 된다.



### Conditional GET

HTTP request Msg에 **if-modified-since:\<date>**라는  항목을 이용해 Client의 cache에 저장된 object가 변경된 점이 있는지 없는지를 확인하여 서버단에 web cache랑 바뀐게 없어 굳이 새로 받아올 필요가 없을 경우 따로 data를 받지 않는 방법이다. 

304 not Modified: web cache의 object가 최신 버전이 다

200 OK \<data>:  web cache의 object가 최신 버전이 아니라 새로 가져왔다.



### SMTP: 이메일 서버

이메일 서버들 간의 메일을 주고 받고 클라이언트에게 메일 정보를 제공하는 서버의 종류이다.

(Clinet1 )----(SMTP)---->(Mail Server1)-------(SMTP)----->(Mail Server 2) ----(SMTP)------>(Client2) 

HTTP는 Client가 데이터를 가져오는거고

SMTP는 Client가 데이터를 넣어주는 데에서 차이가 있다.

최근에는 메일 브라우져를 많이 사용하기 때문에 Client 와 Mail server 사이에는 HTTP를 이용하기도 한다. 



### Persistent, Non Persistent HTTP, Pipeline non Persistant HTTP, End to End Delay

<img src='http://drive.google.com/uc?export=view&id=18eUwW1NCKPhqS2gzOdYjGQOZ81vA0el9' />

<img src='http://drive.google.com/uc?export=view&id=1utPRWmVMLLvKNsp-2gn-C7LQFROb1Jrw' />





### DNS Domain name system

 두 머신간의 프로세스를 네트워크 통신을 이어가기 위해서는 IP주소와 포트번호를 알아야한다. 하지만 숫자의 배열로 이루어진 IP를 일일히 기억하기란 쉽지 않다. 그래서 우리는 보통 naver.com과 같은 문자로 사이트 주소를 기억하고는 한다.  이렇게 숫자인 IP와  문자인 hostname을 매칭해주고 연결해주는 역할을 하는 서버가 DNS서버이다. 

전세계에는 수없이 많은 사이트가 존재하는데 만약 DNS서버가 1개라면 어떨까 생길수 있는 문제는 다음과 같다.

1. 엄청난 수의 사이트 IP를 일일히 검색해야한다.
2. 모든 Client들의 트래픽을 감당해야한다.
3. 물리적 거리가 있을시 불편이 발생한다.
4. DNS서버 자체가 마비되면 IP를 기억하지 않는 이상 사이트에 접근할수 없게 된다.

이때문에 DNS서버는 모두 계층화 되어 있고 분산화 되어있다. 이를 DNS server is distributed, heirarchical database라고 표현하고 있다.

![DNS (Domain Name System)란?](https://blog.kakaocdn.net/dn/3k159/btqzb8L6Qnu/Jk5Z1RzHNuvqwZMcMGEwSk/img.png)

root name 서버의 경우 마비되면 진짜 ㅈ되기 때문에 전세계의 13개의 서버가 동일한 데이터를 가지고 트래픽을 분산처리하고 있다. 하위 다른 DNS 서버도 각가의 도메인을 구동하고 있다.



- TLD(Top Level Domain server): .org, .com 등등 같은 상위 도메인을 책임진다. 
- authoritative DNS server는 상위 DNS 서버에서 요청이 들어왔을때 hostname들(예를 들어 HYU DNS서버에도 소융대 페이지, 과기대 페이지.. 등등이 포함될수 있는 부분)과 IP를 매칭해줄수있는 자체 서버를 구축하여야 사용자가 DNS로 접근가능하다.
- 만약 내부 네트워크에 있어서 Local DNS server를 사용한다면 Local DNS server가 관리하는 host name에 대해서는 Local DNS server선에서 처리하여 Client는 더 빠르게 접근할수 있다. 
- Local name server에서 가지고 있는 도메인 정보와 authoritative DNS server에서 가지는 정보를 동기화 시켜야하기 때문에 Local name server 데이터에 TTL이라는 항목을 통해 특정 시간이 지날 때까지 기다렸다가 정기적으로 업데이트를 하게 된다.

<img src='http://drive.google.com/uc?export=view&id=1R2qcPUyW4ReS7AqnqwgqpxKyHYFpd7Cz' />

### DNS records

> DNS format: RR format(name, value, type, ttl);

#### DNS의 타잎

- Type A: name에 host name을 value에 IP를 입력 (IP 매칭)
- Type NS: name에 domain을 value에 authoritative name의 hostname을 입력



### DNS 서버의 구조

<img src='http://drive.google.com/uc?export=view&id=1HcljIHwE02U_aq4Am87R5ZTzwTLphvsq' />

- DNS서버는 TCP통신을 사용하는 HTTP와 다르게 UDP 통신을 사용한다.
- 



