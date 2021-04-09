## Transport layer

application layer 바로 밑에  있는 계층으로 transport layer 관점에서는  logical End-End transport 단위로 그중간에 있는 과정은 관심이 없다.

app message를 Header과 Data로 이루어진 Segment에 저장된다.

TCP과 UDP로 크게 2가지가 있다.

### network layer VS transport layer

network layer: 메세지를 목적지까지 전달해주는 계층

transport layer: 들어온 메세지를 기기의 여러가지 프로세스 중에 하나로 전달해주는 계층

### transport layer에서 일어나는 일련의 과정

![KakaoTalk_20210331_113954326](C:\Users\js774\Documents\카카오톡 받은 파일\KakaoTalk_20210331_113954326.jpg)

### TCP vs UDP

- **TCP**
- -  무결성이 보장되며, 보낸 순서대로 받을수 있도록 해준다.
  - 받는 사람의 눈치를 봐가면서 전송속도를 조절함(flow control)
- **UDP**
- - TCP에서 지원하는 부수적인 기능 따윈 없음



### Multiplexing & Demultiplexing

Demultiplexing: 들어온  network segment를 어느쪽 socket으로 보낼지 판단한는 것

여러개의 들어오는 패킷들을 어느쪽으로 보낼지 판단한는 기준 : port number와 IP주소를 가지고 Demultiplexing를 수행한다.

<img src="https://skminhaj.files.wordpress.com/2016/02/92926-tcp_udp_headers.jpg" alt="TCP Segment Vs UDP Datagram Header Format – IT Tips for Systems and Network  Administrators" style="zoom:50%;" />

Demultiflexing: socket과 binding된 소켓으로 보내는것 => port번호로 Demultiflexing을 수행한다.



### Connectionless(UDP) demultiplexing

Connectionless인 이유: UDP를 이용해 불특정 다수의 신호를 받으므로 연결이 되어있지는 않다.주로 UDP에서 많이 쓰이는 방식으로 오직 목적지 포트 번호를 이용해 목적지를 구분한다.

<img src='http://drive.google.com/uc?export=view&id=1OXdGSimwjSu38CUZs4rtAYr4TexeQIP1' />

### Connection-oriented demultiplexing

오직 2개의 포트가 연결되어 이 둘끼리만 통신한다. 이 둘 왔다 갔다 하는데에는 TCP Socket에서 4-tuple이라는 자료를 이용해 서로간의 오고 갈수 있는 정보를 유지한다. 4-tuple에 있는 정보는 다음과 같다.

- Source IP Address
- Source Port Address
- Destination IP Address
- Destination Port Address

위 다음의 정보들이 하나라도 다를 경우, 모든 client들에 연결되는 Connection이 서로 달라진다.

  <img src='http://drive.google.com/uc?export=view&id=12Vgb6Si98KJOHUnTHaUBCAl0UoewS5JG' />

### UDP User Datagram Protocol

효율에 극대화된 프로토콜로 UDP Segment는 

- "손실이 있을수 있고", 
- "delivered out of order"==전송된 순서가 바뀔수있다.
- 단순하다
- 가볍다.
- 그래서 위 단점이 큰 영향을 끼치지 않는 DNS, Streaming Service, Voice에 사용된다. 

> UDP 프로토콜 구동 과정
>
> <img src='http://drive.google.com/uc?export=view&id=1-eDuq99mm6cfk-qZyKCKYW9Nuhyv6QnO' style="zoom:80%;" />

> UDP 프로토콜 Segment Header
>
> <img src="C:\Users\js774\Documents\카카오톡 받은 파일\KakaoTalk_20210406_114126823 - 복사본.jpg" alt="KakaoTalk_20210406_114126823 - 복사본" style="zoom: 67%;" />



## Reliable data transfer

데이터 전송시, 데이터 손실도 순서도 바뀌지 않는 채널이다. 하지만 현실은 unraliable한 채널 위에서 raliable을 보장해야 한다. 

어떻게? 일단 다음과 같은 상황이라 가정하자

1. Sender, Receiver side의 Reliable Data Transfer (RDT protocol)에 대해 다룬다.
2. 단방향 통신만을 생각할것
3.  특정한 송수신기, finite state machine(FSM)를 사용한다 가정한다.



#### rdt 1.0

>  Reliable transfer over a reliable channel

채널 자체가 reliable하다고 보장이되는 채널이라면 bit error가, pacjet이 loss되지도 않는다.



#### rdt 2.0

> Channel with bit error

- Error checking mechanism이 필요함
- Sign에 대한 Feedback이 필요함(Acks, Naks)

> Error detection 을 위한 Checksum
>
> - Acknowledgements: 신호가 잘 들어오고 있어!
> - Negative Acknowledgements: 신호에 오류가 있어! => 다시 보내줘(Retransmission)

> FSM specification's fatal flaw
>
> 1. 만약 feedback(ACKs, NAKs)에 Error가 발생한다면?
>
> =>그래서 긱 packet마다 sequence number를 붙혀주어서 수신되는 ACKs, NAKs에 오류가 있는지 없는지를 판단한다. 이를 Handling duplication이라고 한다. 그래서 들어온 패킷의 seqence number를 보고 이 packet이 retransmit된 데이터인지 최초로 들어온 데이터인지 확인하며 다음데이터를 전송하게 된다. 즉, sender는 packet이 정상적으로 전송 될때까지 계속 retransmit하는 샘이다.
>
> <img src='http://drive.google.com/uc?export=view&id=1RKDLjdT6EoZ0XOYYpd8K-7PAl2X_h1Op' />
>
> 이때 seqence number는 앞뒤가 같은지 다른지만 알면 됩으로 1bit짜리 0,1로 구분한다.

> rdt 2.2
>
> - rdt2.1에서 사용된 ACKs, NAKs 중 ACKs만 사용하는 방법
> - NAK대신에 가장 최근에 받은 packet의 ACK number를 같이 보내줌으로서 동일한 효과를 얻을 수 있다.

### rdt 3.0

> error와 packet loss가 모두 발생 가능한 경우
>
> - timer를 동원한다. sender 측에서 전송을 하고 일정시간동안 reciever쪽에서 feedback이 안오면 재전송하도록한다.
> - 만약, 판단기준 time이 너무 짧다면? 대응 속도는 빠르지만 유실이 아닌데 유실 취급당하는 일이 생길것이고
> - 반대로 너무 길다면 유실에 대한 대응이 너무 늦을수 있다는 점을 고려해 적당한 시간 설정을 해야한다.
> - 단점은, **stop과 wait을 시간적으로 사용하기 때문에 utilization이 굉장히 낮다.**
> - 그래서 **pipelining을 통해 한번에 여러개의 packet**을 보내게 된다.
> - TCP Utilization = (L/R) / (RTT +(L/R)) 

<img src='http://drive.google.com/uc?export=view&id=1xX8Vh51vtFiOWTn_lefmlIpcuzaNcQgd' />



## TCP

> TCP의 특징
>
> - Point to Point방식의 통신
> - Reliable, message boundairy가 아닌 in order byte 방식임
> - 양방향 통신(bu-directional data flow)
> - Pipeline
> - cumulative ACKs: Sequence number를 축적해가면 쌓아 올리는 방식이다.

> ##### TCP Protocol Segment Structure
>
> <img src='http://drive.google.com/uc?export=view&id=1tbjEc4oCPUpPP4168ICnjyoG0KN8YBZB' style="zoom:80%;" />
>
> 다음처럼 UDP에 비해 굉장히 복잡한데 주요한 항목을 설명하면
>
> - Source Port, destniantion Port: 기본적인 Multiplexing을 지원하기 위한 기본 항목이다.
>
> - Sequence number: App data의 첫 byte를 해당 Sequence의 고유 번호로 사용한다.
>
> - Acknowledgement number: Packet을 잘 주고 받은 패킷의 개수를 누적하여 송수신을 확인한다.
> - Review window: 나중에 배울 flow control을 파악하기 위한 Data를 저장한다.
> - Check sum: error check를 위한 데이터이다.
> - Out of order에 대하여 대응하기 위해 각 Socket은 고유의 recieve Buffer를 가지고 있어거 들어온 Acknowledgement number 을 순서대로 나열하여 순서대로 내보내게 된다.

> #### TCP Senario
>
> <img src='http://drive.google.com/uc?export=view&id=1i9aM62ZQxVPXNExO873_6m72OyK56gLs' />
>
> 위와 같이 Host A와 Host B는 서로 필요한 Packet을 주고 받으면서 통신을 이어나가게된다.
>
> <img src='http://drive.google.com/uc?export=view&id=1YrJm5qZSc3BOdiB57JTcO0mK7BkmEq0K' />
>
> 1. 각 Socket은 OS내부에서 Send Buffer와 Revicve Buffer 를 할당하게 된다.
> 2. Send Buffer에서 Host A가 최초의 패킷을 전송하고 Host B의 Recive Buffer도착한다. Host B의 Recive Buffer는 Sequence number 순서대로 정렬해준다.
> 3. 그때까지 Host A는 Packet loss에 대비해 다시 보낼수도 있기에 Host B에서 ACK신호가 올때까지 기다린다. 그러다 오면 삭제한다.
> 4. 해당 과정을 반복한다. 
>
> 결국 Socket과 Socket간에 자원 배정이 되는 셈이다.

