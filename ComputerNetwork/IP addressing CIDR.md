## IP addressing: CIDR

### CIDR: Classless InterDomain Routing

- 예전에는 IPv4 주소를 Subnet ID  8bit 8bit 8bit에 Host ID 8bit로 Class로 엄밀하게 나누어 IP주소를 구성하였다.
- 하지만 최근에는 따로 정해진 Class 없이 Subnet ID의 주소값의 자리수를 따로 구분하지않고 자른 후 A.B.C.D/x의 형태로 IP를 구성한다.

예를 들어 

11001000 00010111 0010000 00000000 이라는 IP가 있을때 최근의 IP 주소에서는  앞에 24bit가 아닌 23bit만 Subnet ID 로 사용이 가능하다.

그러면  11001000 00010111 001000 | 0 00000000 에서 앞에 23를 사용해 왼쪽에서 부터 8bit씩 나누어 200.23.16.0의 형태로 표현하고 그후에 subnet ID를 표현하는 자리수를 따로 표기한다, 

그러면 결과적으로 200.23.16.0/23 이라고 IP를 표현할수 있다.

그리고 이를 통해  200.23.16 subnet에서 가질수 있는 host의 개수는 2^9-1개의 host를 가질수 있가는것을 알수 있다.

그리고 subnet ID의 자리수가 작을수록 더 상위 서버라는것도 알수 있다.



### 그럼 IP는 어떻게 받아오는데?

> 이 질문에는 2가지를 설명해야한다.
>
> 1. host는 어떻게 해당 network의 IP 주소를 받아오는가?
> 2. network 자체는 어떻게 자신의 IP 주소를 배정 받는가?

#### host는 어떻게 해당 network의 IP 주소를 받아오는가?

- 일단 고정 IP를 사용하는 방법이 있다. => 단순하지만 host들이 항상 네트워크를 사용하는것이 아니기에 IP가 낭비 될수있다.
- DHCP(Dynamic Host Configration Protocol): 동적으로 서버에서부터 IP주소를 받아오는것

<img src='http://drive.google.com/uc?export=view&id=158FU11wT2LDbjnR_quExlao4HXIqn246' style="zoom:50%;" />

> - DHCP discover: 아직 IP를 배정 받지 못한 Client가  불특정 다수의 host와 server에 DHCP 서버가 존재하는지 알리는 Broadcast 패킥을 보낸다.
>
>   - > DHCP discover Packet 구조
>     >
>     > src: 0.0.0.0,68 								(아직 IP 없고 DHCP Client 전용 68 port 바인딩)
>     >
>     > dest: 255.255.255.255,67 			(broadcast  IP로서 DHCP Server 전용 67 port 바인딩)
>     >
>     > yiaddr: 0.0.0.0
>     >
>     > transaction ID: 654						(DHCP Packet ID)
>
>   - Broadcast Packet으로 보냈기 때문에 다른 host들도 이 패킷을 받게 되나 DHCP Server만 전용 67번 포트가 개방되어있기 때문에 다른 host들은 discard 되어버림
>
> - DHCP Offer :수신한 DHCP 서버가 사용가능한 IP를 포장해 Client에게 IP를 권유함.
>
>   - 이 또한 Braodcast로 사용되지만 특정 transaction ID로 구분하여 Client가 선택적으로 수신가능
>
>   - > DHCP Offer Packet 구조
>     >
>     > src: 223.1.2.5,67								(서버 IP의 DHCP Srver 전용 포트 출발)
>     >
>     > dest: 255.255.255.255,68 			(DHCP Client의 68번 전용 포트 도착)
>     >
>     > yiaddr: 223.1.2.4							(Server에서 할당되지 않은 권장 IP)
>     >
>     > transaction ID: 654						(DHCP Packet ID)
>     >
>     > lifetime: 3600sec							(해당 IP 사용시간)
>
> - DHCP request: 권장 받은 IP를 수용해 공식적으로 사용요청
>
>   - 특정 host로 보내는데 Broadcast로 보내는 이유: 다른 DHCP 작업중인 Client들에게 본인이 이 IP를 사용하겠 다고 알리는 역할
>
>   - 아직은 IP를 본격적으로 사용할수 없음
>
>   - > DHCP Offer Packet 구조
>     >
>     > src: 255.255.255.255,67				(서버 IP의 DHCP Client전용 포트 출발)
>     >
>     > dest: 223.1.2.5,68 						(DHCP Server의 68번 전용 포트 도착)
>     >
>     > yiaddr: 223.1.2.4							(Server에서 할당되지 않은 권장 IP)
>     >
>     > transaction ID: 655						(DHCP Packet ID)
>     >
>     > lifetime: 3600sec							(해당 IP 사용시간)
>
> - DHCP ACK: Server 측에서 공식적으로 IP를 할당함
>
>   - 이후 3600초 후에 DHCP request 패킷부터 다시 IP를 갱신함, 갱신시에는 굳이 broadcast 패킷으로 보낼필요없음
>
>   - > DHCP ACK Packet 구조
>     >
>     > src:  223.1.2.5,67				           (서버 IP의 DHCP Client전용 포트 출발)
>     >
>     > dest: 255.255.255.255,68 			(DHCP Server의 68번 전용 포트 도착)
>     >
>     > yiaddr: 223.1.2.4							(Server에서 할당되지 않은 권장 IP)
>     >
>     > transaction ID: 655						(DHCP Packet ID)
>     >
>     > lifetime: 3600sec							(해당 IP 사용시간)
>
> - 이후 client는 223.1.2.4 IP사용가능
>
> - 이외에도 Client는 Server를 통해 Gateway Router의 IP주소, DNS Server의 IP주소, Subnet mask의 IP주소(/23)도 받아올수 있다.
>
> - DHCP 패킷은 UDP로 작동한다.

#### network 자체는 어떻게 자신의 IP 주소를 배정 받는가?

- 통신사나 인터넷 업체가 ISP(Internet Service Provider) server에서 특정 address space를 각 조직에 분배 해준다.

- 굉장히 큰 네트워크이기 때문에 subnet이 작고 연결가능한 host가 큰편이다.
- 만약 ISP block의 IP가 200.23.16.0/20으로 왼쪽부터 20자리가 1100100 00010111 0001이라면 뒤에 부족한 2자리를 0으로 채우고 나머지 총 23자리를 subnet IP로 취급하고 뒤에 9자리를  host의 배정 블록으로 확인한다. 그래서 한 ISP IP마다 연결가능한 host의수는 (2^9-1)개 이다.

- ISP같은 경우에는 따라 IP주소 배정 기관에서 address block의 사용권한을 받아온다
- 2011년에 IPv4 공간을 모두 배정 하고 NAT라는 방식를 이용해서 연명 중이며 이제 IPv6로 대처하는중이다.