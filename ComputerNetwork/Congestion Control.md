## Congestion Control

> Congestion? : 엄청 나게 많은 소스가 네트워크가 감당할수 없을 만큼 보내지려고 할 때 나타나는 현상
>
> 해당 상황 발생시 증상: Long delay, Packet Loss
>
> Flow control에 비해 불특정 다수에 의해 발생하는 문제 이기때문에 본질적으로 다른 문제이다.

<img src='http://drive.google.com/uc?export=view&id=1w-QOiSl7NeN163Q_o-jr0biVllaar61S' />

Congetion이 발생할 경우 Host A에서 전송한 Data가 건너편 서버로 전송하는 상황을 나타낸 것이다. 이 상황에서 중간에 거쳐가는 라우터의 경우 Host A에서 전송하 ㄴ데이터를 관할하느라 다른 서버에서 전송한데이터를 차마 처리하지 못하고 패킷이 송실되는 경우가 생길수 있다.

그래서 서버의 전송의 속도가 너무 빨라지면 라우터가 그 속도를 감당하지 못하고 모든 네트워크 노드들이 손해를 보는 상황이 생긴다.  다시 말해 Upstream에서 DownStream으로 갈때 Upstream에서 빠르게 내뿜는 데 사용한 자원이 낭비되고 다른 노드에도 손해가 가기 때문에 분명 주의 해야한다.

그래서 각 노드는 Flow Control을 통해 적절한 속도로 데이터를 내보내야 모두가 행복한 시나리오가 실현될수 있다.



#### Congestion Control에서 네트워크 상황 파악하기

End host가 Data전송에 대한 ACK가 잘 돌아 왔는 지 파악해 네트워크의 상태를 간접적으로 파악한다.



#### TCP Congestion Control: AIMD

