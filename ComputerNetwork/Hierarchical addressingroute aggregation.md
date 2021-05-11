## Hierarchical addressing:route aggregation

- 아이피도 계층화가 되어있어서 Fly by Night ISP의 경우 Prefix 말단 IP가 달라지는데 그에 따라 어떤 IP가 Fly by Night ISP의 대표 IP로 설정된다. 그래서 외부 인터넷에서 대표 IP로 설정되어지면 "대표 IP가 도착지인 모든 패킷은 이쪽  Fly by Night ISP로 오라"라고 연결되고 내부에서 계층화된 IP들이 분류되어 본연의 목적IP에 도달하게 된다.

<img src="https://images.slideplayer.com/31/9736845/slides/slide_2.jpg" alt="Network Layer 4-1 Hierarchical addressing: route aggregation “Send me  anything with addresses beginning /20” / / / ppt download" style="zoom:50%;" />

![IP datagram format IP protocol](https://image.slidesharecdn.com/ipocalypse-110218171838-phpapp02/95/ipocalypse-10-728.jpg?cb=1298229038)

### IP fragmentation/reassembly

- 모든 network link는 MTU(Max transfer size) 가 다름
- 라우터는 서로다른 용량의 MTU 사이에서 MTU보다 큰 패킷을 보내고자 할때 패킷을 나누어서 보내주고 목적지에 도착해서 목적지 IP Protocol에서 다시 조립하여 패킷을 전송하게 된다.

![까망눈연구소 :: IPv4 데이터그램 단편화](https://t1.daumcdn.net/cfile/tistory/99FADD425C32000402)

<img src='http://drive.google.com/uc?export=view&id=16i-mWbTk3knMb_b5N9Vgtu4KcgHlA4I1' />



### Data plane

- 90년대 이후 IPv4는 32bit를 사용했는데 모두 사용됨

- 그래서 IPv6를 사용하러 갔는데 128bit를 사용하기로 함, 근데 상용화가 잘 안됨
  - 왜? 현재 인터넷를 구성하는 라우터들은 현재 IPv4를 IPv6도 이해 할수 있도록 장비를 업그레이드를 해줘야하는데 이게 만만한 비용이 아님.  
  - 그리고 이걸 먼저 바꾸면 남들 다 IPv4쓰는데 나만 IPv6를 사용하면 그거도 그거대로 고립되는 것이기도 하고 이래저래 손해이다.



### NAT : network address translation

주소공간없는데 NAT로 끌어다 쓰고 있음

내부IP와 외부IP가 따로 존재함

IP부족 문제에 대한 대안 

메커니즘 이해하기