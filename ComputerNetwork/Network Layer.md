## Network Layer

- Tranport Layer에서 보내고자하는 Segmant를 Network Packet에 담아서 잘 전송 해주는 계층

- 이제부터는 내부 네트워크에 대해서 거론된다.

- End host는 네트워크 5계층이 모두 갖춰져 있지만 Louter은 network계층까지 밖에 없다. -> 오직 Packet을 전달하기 위해 특화된 기기
- IP Packet의 header를 이해 할수 있는 계층이다.

![img](https://user-images.githubusercontent.com/45806836/99146883-8bbd9a80-26bf-11eb-9b47-9ce7006b84da.png)

### Per-router control panel 

- Forwarding: 패킷을 받아서 목적지 까지 가기까지 바로 다음 라우터로 전달하는것
  - 어떻게: Packet header에 있는 forwarding table을 Look up하여 entry를 찾아 설정하여 packet을 쏴줌
  -  어떻게 Louter들은 forwarding table은 어떻게 만들어지는 것인가? -> Louting algorithm
  - Forwrding을 Data plane이라고 부르는 이유: User Data가 담긴 패킷을 전송하는 일이기 때문
  - Control Plane: forwarding table을 채우기 위해 Louter들 끼리 서로 정보를 주고 받아 주는 행위 =?
- Network Layer가 Transport 계층에게 해주는 서비스
  - IP서비스는 Packet Loss, Order Tranmission, Timing등.. 아무것도 노력은 해주지만 보장하지는 못한다,(Best Effort Delivery:  최선을 다하지만 보장은 못한다.)
- Forwrading Table의 구조 
  -  다음 사진을 보명 forwarding Table의 구조를 나타내고 있다. 이 Table의 구조는 "Destination Address Range" 범위에 있는 Destination에 해당하는 Link에 Forwarding해라 정도로 이해하면 된다.
  - ![4.1. Network layer Overview & Router Structure : 네이버 블로그](https://lh3.googleusercontent.com/proxy/JHkMPvCaBqTBYAZh9ah7bi6QDyfmCnB_35PVPyAumoThrMUCnnpFO2kPag7V9VVFH5eDOWbJ6_RKN1nqgnp4Spv119qPNu25E0C_vJ5O8eYTGfit_73S-o9mK4ipJ1pQhXG5f64uEk6Gb72aUrP1Jc3GXYl9c-HofcBcfb-xpXLflt-dvcf2QldWB9suy35luunJpmD6zDTbqPFHVYcCFJv33qVtKbHuiqTyxvF_L8uatW2GZkXF7nS18HNxn_ZZ99WGagafOWec0v34x6lfw3nWbpNRIdx0_ZTZcw)
  - forwarding link에 따라 더 구체적인 값을 표현할수 있다. 예를 들어 넓은 범위 내에 특정범위를 다른 곳으로 보내는 방식으로 응용하여 Forwarding을 수행할수 있다.
  -  다른 표현 방식으로는Longest prefix matching이라는 방식이 있다. 예를 들어 11001000 00010111 0010*********이라고 하면 앞에 내용이 동일하면 뒤에 뭐가 오든 가장 일치하는 값에 해당하는 특정 link로 보내라, 라는 방식으로 전송이 가능하다.
  -  이번 챕터는 IP Protocol에 대한 내용이다.



## IP Protocol

![IP datagram format IP protocol](https://image.slidesharecdn.com/ipocalypse-110218171838-phpapp02/95/ipocalypse-10-728.jpg?cb=1298229038)

Source IP Address: 

Destination IP Address: 

time to live: Louter을 넘어가게 되면 -1을 하여 값이 0이 되면 Packet을 버린다. : 갈길 잃은 패킷이 불필요하게 네트워크 자원을 사용하면 안되니까 

Upper Layer: TCP로 전송할지 UDP로 전송할지 저장

Header check sum: 헤더 파일 오류 검출용

Overhead: 20byte(TCP Header) + 20byte(IP Header) + Application Data

- 일반적인 Packet들은 1500byte까지 전송 가능하시만 실제 네트워크에서는 40byte짜리 ACK패킷이 많이 돌아 다닌다.

- 현재의 IP들은 32bit크기로 이루어져 있으며 일반적으로 사용하길 8진수로 나누어 0~255를 가지는 8비트*4자리로 IP를 지칭한다.
- Network Interface는 각각 고유의 IP주소를 가지는데 한대의 기기에  Network Interface가 여러개인 기기는 대표적으로 Louter가 있다. => IP는 Network Interface이다.

- Subnet

  - 라우터를 거지치 않고서도 접속 가능한 호스트의 집합
  - 자체적으로 네트워크를 형성할수 있음에도 여러 subnet들은 Louter는 아주 특수한 컴퓨터라고 할수있다.
  - 동일한 Subnet들 구성하는 host들의 IP를 보면 앞부분이 동일하다는 공통점이 있다. 
  - IP 총 32bit에서 앞의 24bit는 subnet ID하고 생각할수 있다. == Subnet Mask 
  - 라우터와 라우터 간의 네트워크도 subnet에 포함된다.

  <img src="https://ipcisco.com/wp-content/uploads/2019/10/subnetting-mask-example3.jpg" alt="Subnetting Mask | Fixed Length | Variable Length | Subnetting Examples" style="zoom: 67%;" />