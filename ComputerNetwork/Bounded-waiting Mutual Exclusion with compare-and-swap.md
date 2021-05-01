### Bounded-waiting Mutual Exclusion with compare-and-swap

CAS(compare-and-swap)을 사용하여 CS(Critical Section)에 순차적으로 들어가는 알고리즘

```c
while (true){
    waiting[i] = true;//다른 프로세스가 CS에 있으니 i는 가다려야한다.
    key =1;
    while(waiting[i] && key == 1)//waiting[i]가 False이면 누군가 CS에서 나왔음을 의미
        key = compare_and_swap(&lock,0,1);//key는 lock의 현재 값으로서 key=1이면 누군가 CS에 있다는 뜻이다. Key=0일때까지 기다린다.
    waiting[i] = false;
    /*Critical Section*/
    j = (i+1)%n;
    while((j != i) && !waiting[j])//돌아 돌아 자기 자리가 아니면 j를 하나 올린다.
        j = (j + 1) % n;
    if (j == i)//만일 다음 사라밍 나라면 기다리는 사람이 없음 lock을 풀어준다.
        lock = 0;
    else
        waiting[j] = false;//기다리는 프로세서가 있어서 lock을 걸고 들어가도록 함. lock을 다시 복원시켜주는 역할 
    /*remainder section*/
    
}
```



### Atomic Variables

변경하는 과정에 inturrupt 같은 중간 방해를 받지 않는 변수

``` c
//다음은 한번에 실행되는 Hardware instruction을 풀어 쓴것이다.
void increment(atomic_int *v){
    int temp;
    do{
        temp = *v;//v를 읽고 저장
    }
    while(temp != (compare_and_swap(v,temp,temp+1)))//저장된 temp와 비교해 다르다면 중간에 inturrupt된것 임으로 다시 while loop로 다시 수행
}
```

 ### Mutex Locks

- Hardware-based solution은 개발자 입장에서는 복잡하고 inaccessible함 => Hardware-based solution을 이용하는 소프트웨어상의 추상화된 도구

- 상호베타 진행을 지원하는 가장 단순한 형태이다

- acquire(): lock을 획득하는 행위(CS에 진입함) / release(): CS를 풀고 빠져나옴

- Mutext lock은 CPU를 소모하며 기다리는 Spin lock을 사용한다.(프로세스 경쟁이 치열하지 않은 다중코어 시스템에서는 Spin lock(busy waiting)을 많이 사용함)

  > Spin lock
  >
  > CPU를 소모하지만 context switching이 필요없기에 유리한 면도 있다. 그래서 다중코어 시스템에서는 우수한 성능을 보이고 Context Switching을 2번 할 시간보다 lock을 걸고 있는 시간이 짧으면 spinlock이 유리하다

```c
while(true){
    acquire lock
        /*critical section*/
    release lock
        /*reminder lock*/
}

aquire(){//atomic하게 수행되어야함 -> test_and_set과 compare_and_swap을 이용해 구현
    while(!avaliable)
        ; /*busu Waiting*/
    avaliable = false;
}

release(){//atomic하게 수행되어야함 -> test_and_set과 compare_and_swap을 이용해 구현
    avalibale = true;
}
```

### Semaphore

- 상호 베타 진행을 지원하는 가장 일반적인 방식
- Semaphore S - 정수형 변수 [S값이 0보다 크면 CS에 들어갈수 있다.]
- wait()와 signal()[atomic하게 이루어져야함]

```c
wait(S){
    while(S <= 0)
        ; // busy wait
    S--;
}

signal(S){
    S++
}
```



#### Semaphore 의 사용

- Binary Semaphore : 이진수를 가지는 Semaphore 
- Counting Semaphore: 정수형 변수를 가지는 Semaphore 

> 예시: P1, P2 프로세서가 있다. 여기 있는 서로다른 프로세서에 각각 S1, S2가 있는데 S1을 먼저 일으키고 S2를 일으키려고 한다. Semaphore 사용법은?
>
> ```c
> //처음에 Semaphore synch를 0으로 초기화한다.
> P1:
> 	S1;
>     signal(synch);
> P2:
> 	wait(synch);//S1, S2의 순서를 보장하기 위한 방법
> 	S2;
> ```

#### Semaphore  구현

- 2개 이상의 프로세스가 동일한 Semaphore 를 동시에 사용할수 없도록 함
- Semaphore 변수에 대한 CS문제를 다루는 거라고 볼수 있다.
- wait()는 synch가 0보다 클때까지 Busy waiting을 한다. 단, 프로세스가 cs에 오래 토록 머무르지 않을때 유리함



##### Busy waiting 없이 Semaphore 구현

- 각각의 Semaphore를 waiting queue에 집어 넣음

- waiting queue의 entry에는 Semaphore 변수 값과 리스트의 다음 변수를 가리키는 포인터를 설정한다.

- block() : Semaphore 변수가 0이라면 프로세스를 멈추로 waiting queue에 들어감

- wakeup(): wating queue에서 나와서 Ready queue에 들어감

  ```c
  // Single Core 에서 Semaphore 구현
  typedef struct ={
      int value;//Semaphore 값
      struct process *list;// 대기열
  }semaphore;
  
  wait(semaphore *S){
      /*멀티 코어 에서 있다면 다음처럼 test_and_set을 이용해 lock을 얻어서 동일하게 동작시킴
      while(test_and_set(&mutext))
      	SpinLock
      */
      disable inturrupts;
      S->value--;
      if(S -> value <0){
          add this process to S-<list;
      	block();
      }
      enable inturrupts;
  }
  
  signal(semaphore *S){
      /*멀티 코어 에서 있다면 다음처럼 test_and_set을 이용해 lock을 얻어서 동일하게 동작시킴
      while(test_and_set(&mutext))
      	SpinLock
      */
      disable inturrupts;
      S->value++;
      if(S -> value <= 0){//증가 시켰는데도 음수나 0이라면 waiting queue에 누군가 있다는 뜻이다. 
          remove a process P from S-<list;//waiting queue에서 Ready Queue으로 이동한다.
      	wakeup(P);
      }
      enable inturrupts;
  }
  ```

  

### Semaphore의 문제점

- Signal와 waiting을 둘중하나만 편향적으로 균형있지 않게 사용하면 굉장히 많은 오류를 범할수 있다.



## Monitors

단 하나의 프로세서만 Monitor 함수를 사용할수 있도록 하는 도구

어떠한 이유로 프로세스를 진행할 수 없다면 condition variable를 이용해  스스로 Suspend할수 있다.

>  Condition variable은: 대기열의 이름
>
> x.wait() : x대기열엣 가다려라, 누군가 x대기열에 신호를 보내줄때까지 대기하라
>
> x.signal(): x대기열에 대기하는 얘를 깨워라, 만약 없다면 아무 효과가 없는것이다.
>
> 프로세스가 Monitor 내에서 방해받지 않고 실행될것을 보장한다. 만일 타 프로세스와 병행 실행을 허용하고 싶다면 조건변수 wait()를 실행한다. 즉 Monitor함수는 스스로 preemption의 위치를 스스로 지정할수 있다.

![W4118: semaphore and monitor](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAQ8AAAC6CAMAAACHgTh+AAAA2FBMVEX////H6vvR09SVlZXNzc22w8vv7Ouv0eLK7v+/wcPR0dHo5+iSlJbGyMnX2Nr9/P3h4uO7vcDw8fAAAADl5Oa4uLjM8f/DxsfV1ticnJ+Ih4e93u6gu8i0tLTR9v+uzNuytrppbG96fYF2eHunqatgXmCFmqWTrLioxdOko6PB4/NVU1SDgoJlc3tsfIWFhIWZpqx4ipSIl6A5NziOpbGKk5hOUFMiHiAYFBc2NDYqKCqau8paXF9maGxfbXWftL5GQ0SSnqZRXGJZYmhHT1QUAwJ3kZ2jrbO/qW2DAAAQPUlEQVR4nO2dCUOjOheGU2FKgLKYCpatqCBbN6vMXEfHUe/cT///P/oCXaF7TdCOvjNaSCmSpycnJwsEMOBTi/UunULCZ+ah+N7FwPsx3eXBZ+ZRNxq+8X3g/WLzXdT951b+vDxYP5bOfKMx8M47eFe+vGGBePPjk/JQDFkVTsc8fgj8zYMKgHM7qH9SHsBH8pTH5a0h4iTEIGR+Vh7q0cw+Ln/Wx6ne6WflAfyTGY+nOQifiYfSmm2fHs14/LhD0/TPw0NxwntxtuvP8fjpT5M/Cw/k2wmEr6Odm06n0+Tn7ONmetwh85C3PlK4utdhrQZDJd89/99tUzTG8cfD04M2O/IQeSiaDFRJVn6osiArAMeXyFFRC4gIaC0gZK/HwtzxpykHMQ0smBUM6e6yeVsH/sg+fg3mDz1IHiaKxUvVRB7osJIm4ix4dU/tAl5m6j5+1WRGdNjJ0WqajGlgHo8gbpoIdLAjQTHm8WCIxXMfIo9LJ5ZNoIkm6GI4MQIKAyTVBKfypeMLA3CqPjg+Pzka2RMaNZj8bjrK5I3G2SBWyuc+RB5d0Ve6wDnpKh7OQDdLUbF9qF3ZVB2lq56rjOrMvnd/ah32nf04O42oLZ76IHkovIh9qaiIIm51sFmuFMmXcSoC/Al+9wRhE5kdLkYzA9E5YeVpcx0ij3mJ56NXrVTZKHN+4XXKo1bTn9ef79B5LJdo/Dqf7cncjEctqq/+GPg7eQgNz7z4NTIYx/G886E+LTCQ89d+9u/jwTYMo2FenHfzvfPmbdNgkzGNmvusrv/0X8ZD0YzjYz/n8aRmbZa72ybOYgpz2wh9cdMJPgIPVZbR5qO2keOosjTmcRkj858Y5I15TYcwSbUt/sp78qhrfOOxb9v396Ftp8NvBi+8kcspOjnCPOKMR+f26RQn5eUDhdbVsmhjUe/FA/FGem9BCPE3N1XChdd+a/OHV0pxxKOxfXTunuYI8BvCjqneh4d/FVo6nAsLpu4f6m5gnOx9Yked8Dh/6DztcYJ34CH6QW0JixkUPbrezrgnJ5xtIq019R+dn87qz6xS5TzUq/t1MCYNr3DrvAhX93PH+jN/2nm63P3yKuaBniN9I41RqJBuZSPaKwf1cLTdYMynO4ed8rhp7u6LquXh3+ubUUyJhJuIIC1NMrqWlO8yOPaS44l9/Lpr7FFbVcmDxX5jaxwZEWttNCmfhZNuryu8K17cNLFBCGc5j/POHs4DVMlDMbjtjWMs/X5lrpRnd+aHOFW47GjgMsZvNI6d+MJjV31ugyrjoQab3egSE0muV7VHH2d0YfrPeeYq8vLBnzXM7Xuay6qKh7aD5yiaSCgtP6NkTQ/5txfOuQpmYyNljSriYXAF47DKubZmv8tWtLKF/jzX7aWvb8Vvr0p4KM/JXC5xDPq7nY2GwPH/Gmz/1ke/cYDqWqVoDSbfl55Vmz/mnlCLsAoeyvV8WYFBAPtpCrlhqNtpLR0mMAj7+IgXu68Hgd7rw5egZCTXS3Mbzh+1X3WyoAp4lHCEoeX+Vwu4tN23/uVSt9ZPIw5bxpBz+y5nh7ZrJ2lUBKIHS4DEP+c8KhySuVj6PNB10ZMmaY/rt1PuJe1Z/fYwCNJhrd3T9T5s9+0geHx0w2BY4rEIBHm3zmRkJevbOFsYSdlP1HkoJRwwDIKo107d3/afqKdHQ2wQwwDbh/0SDINwGNjB8LEflr1qEYjcecoa8K+jbi/ulV/113cWdR7X5XoWcpZu4aok4WoWdp0WB6GFN2rZb+jiN7j8d1n6zIdod+ejuFXGIZl+/7pt38Y2os2jsZCxGZjCy0L6ApBRLaMwd8yUzFXNvlo/frCrKPNoJKt57Kxn3Ejp3hlznkIlVKvMRJeH5u4Ro68S/HPrPe3UUbSHqPJQwz2D9KVKmk2T5tXmoslDGZLEgWumYUCoVl0tmjxMojjyZsqG0ei3iyIPZ7HSfLOsw/Uf9ZSgL50I2mSr1wUR5sFq0x6+i6y0zJDkA05LtnYEoi9v6xLTTjyM7JcyiYYmzh5paLItzLrpfAtGNWva7WHZWHntm29Fxa0dRLnEMEAWQfYPCNmcGhEoOPqt5/1tKv6qBQQQTsgmJuEfEdSPwDEDUAu/i7rZ4KgK6gry5AHK7WJ2rQg32d1hbxSOYUcYPTQc+RsOr3U72+JhTdcfs63jHXlM5pDS4tEV7sAAmEpXNhEDuoopd9WGhOM+1jFks2WirmwIseYIccx2ESMwkoFMNmYNx8MmotzUTRQPZNMxslbEnHngS2//TvV8CDLoRdx540z+pkf/2VG2xeu1f8JwlLZrkTFo8jjXsK2bgFE9B2cMNFgTvzYa+NtWY1MSBhowzFYD5zwrWAMl9gfqqcY4DL6obuYuHMdQ4ljpjkLnKQ/hHufSHf5OsjIR9Zv/++PhvL9GabP5X7Z1ZtvN5s/x1m5FBkYbprS8SYwJfgAPPCET4Fwpl6IBNFlCXpZ5XzKAJzqYGSsyjnDKmvGJ6qmO7CjOqaTiY5QbdMmiOEZlHlkkBu2a5WYuU7dD3c3Li84Nk7zkYPsYWnnJOdajMNrJPiaz0OnwqPsekH0WqIaIi8gRkA0EjrN5qihWJWTIgI3r4BhnVtNwdmOBVxychPBu1sN/DCRFadV5FuQOZNIP4eR+FE6bsLBmpeHjI863Drlsy4ZzWzu6VHj/lg70TTxyuyejujepehYDdbikvp1L20n6FakrXhTmQc5fo3F50abV7Cy3FlYy3XpT6ErTg+wdjzE8QqDs6sc8xpEpLgej9j7UYdTpGvxjVsvaHY/R/DfFrhQNZD2PupLPv8KBCc48kJXMSyAFRycIDRRwiRPxjoJwQv1kxoMfRx3YY4bZAAu0frtZfat+09v/RtHIi+5TUKaiZyDreagmk//lc+ApwDz1VFMzhItjQzS1B0e8rHeBJ3WVDnuhGvkNFqf5pybegwv6+BeucMO8lm04z3bUbN56DcNv7B6YzguevQ8P8DT6xi9ULY/PGUwFx+SmjyN0Bw3AgBWAr8Y4jBsYmS/Nu++0yfRovZ9XuxByf1x3Yh/unrVskUdIaDhuRx5OzICsQWlm7RMPMJKmdAUJDHAEcungoAMHrMBDBujySMhGUJWMyvOkctFfJjaA69b+96vXvJZ1+9+fX7Ensd9SYCC5EYYdeCAeOHmcxWQNGoTLDm8o4gmQAO8LsiKzEpAZhFs8pyAeDyhLsjr53mHUmxtA06d163jrTR4Vpu/BI5OKq+PjeIczXk1nlNYgPXH7T/FYq63q210KqzJ8/FaFdvmKdhDx/jHaHXoT8XSa/QfLQ9l3hth6keah0myMF0R8aC4XaR5VmQeOhamEIIfLYxwMExZhHtUVl6zvhYII86AVNi4TFVMkzKPC4kLnj5HlgaiU6VWiYYxkeVRZXL54lEXDeR8yDxoOhCgP9JZ7I/cQBfxEeUjUp+8URcF7E+VBJUJaI5G8AzloHsqKW2PeILLlZW5bUUsal6V6OX39KRcOLxRJ8g6VJA91vsXJ1kVVbgmsxPPHmnbm+GNYvKiq8lGefKydnTmD9ed0TksqTM7+2PZxOv/dsQpCWdZbOO/SKc49O00XT/JkNkvmG+vPqR2VVOBBvoIhyaNwdSMesnw04SFN08c8pLfzIB+RkeRR6MGb2AdVHuQ7DUnyKHi3SniQdyCHbR/kKxiCPOTCtJ1K/Af4yOWldKVlHgv1CwkexCsYgjyK0Wk1PFqkW0wEeRRtt5ryQvLWuVwEeRRttxoexFswBHkUL60aHsQd6oGXly8eJR6kAxByPNCXfRSEijN2vnh88Sjoi0dRS3nQbs99ZH/6Ljy+7KP4/oHx+PIfE+3TvpXq2cODwfxkys/MQ2g+AdBsIuA0p09P/3t5bC4vqOMD0MUo1KdpW/kvq1++/MdEXzy+eBT0xaMopTg75dPz+OoPKumrP6io4lf1xWOZfdBuz7VIP06I8ngDdR4feHxuWf1Cm8dHHr99D3/6kcf3iwMO1Yxnf+T5H8vmw9Dm8ZHt4x3m05GfsU2SR2ECSCU80Eee71AqL6A0/3TGY8RJ2G7+aWsdD/L3JBPlMT8/WR1R0DTNyeWPm79Slp4lj9M3PF2N14pyCjNOyU+YJ8lDpPTMhdX62POTq7496qOXl8p5kHenh82DJX8/FlEexFubG0Th/iOiPCg9k2OlPjqPigsMhdvFCPMgtWrRdqJxOydZHlU+7oLO7YvUnndBLTabIf/4z3eYu0Se0hP16tOYg8Ldt8R5SDMKBpWbtZVZe+cQng8zX8P47FbiJQE3/rTtDmbnygiVB0yR5rHPY+PUeJ+Kk07rkTSPvVYE36uapvNonoN9Pt0Xj6JadEId4jyqCskocSe//suebn/H/CkHw0PbJ+4wd3rGKqDXcqSwPtDmb85saGKXiUHMOOCXHzMMummZWSrfHUii2d1iPQJaLUcKPDYWGE0GpuoARvRaHjIBy3ayFVdwWMY4LdAVTHmzrexVrW8jCjw29mrywpgHg3jEoAbwQBcMsueYOzLoqkq8uTBQ61igsZ7WxotlTFZ8aJwCv+ErPjBjRo0FAxgmK6nAUAeNjV++Sq31TIPHNlf7tpUn6fU7UVlvbYvLRW8JU+QPuL7HOqmUn1NHsVuSznp8dPtRKZoHJR6Iakc7nSdrj0RpvUaal8xTXD6K2vqV9IDQarmMRIuHRO1LpOubqK1vSmm9ibn+dSqixqNOaSyX6mqNNNe/daiMN/iUHzlLcX3kGNVJS+YpDGEXRJFH/YUjLeuQ188GEjdeM6r4UptbrnHVy/JjdGrLik1Fkwcw3rJk1pJFtCK6dUsmqjzANdEV5zmaUe9YdHnUw9VAFtdb27QCG+WqNhddHqN1kku5DjJvYFuRHmQrstmPaW20NlvNskIrg7JiaUvKK6uPRJkHYN2FzEWWmyac9RRFEG9EtpsEqa3bIRwOuaSWunhz8UM1PaB8pSPR5pGtO1+yj57bs/oh16v19BerH9n9GgbChXYa2oHVs4bcT+vPEhzVzOWkzmMBCOy7YRvz6Ld7tWHai+whdPtt7gUbSRRww7aV9tvD98JRAQ/gcwUg2D7Sdi/l+lbPfYl+Y/uwbjjuxU7x/8Dquzi93X8vHFXwKAGBbmJB10os19UjzrUs17If7czB1mwuwa/Qhe574aiEB2DtFdXudP11mC/GDldULtfVzQOvhAcQ0k2B2boFxiupaMeqhgdA18vtY2wQejJctaA25Gj1LC1VRTxwW4ZbEpA+9t3a8AVawzSAL8NkGY6Qdgu/qMp4AC3Uy5GI9aKn/XzZ8Sh4sazhYiybVOg6clXHA9S/c0UvAm27jStXOOy3k6Afpo9lHvp9tTcIgEp5YBMJSk6idx3Yw76Lf/DrdWkFesg9V2wcoGIeQInDIhELwiTBP1nRSIphW5JW6zlGqpYHAGIjLbuRZcI0Kl0Kc6qqeWA3YtrJirp1AgNy70TjPXhkpeb5Hq5CgtPDV/r9gqv0Hjyw1LPn+wSWoGT7SfhNo95pvEbvxAOrzh9/C+15HvbjN16oeInDkv4P1J6oGGiCJFcAAAAASUVORK5CYII=)

#### Semaphore와 Monitors의 차이점

| Semaphore                                                   | Monitor                                                      |
| ----------------------------------------------------------- | ------------------------------------------------------------ |
| **wait(s)** : [--s >= 0이면 진행 아니면 suspend]            | **x.wait()**: [무조건 suspended]                             |
| **signal(s)**: [++s <= 0이면 suspended process을 하나 깨움] | **x.signal()**: [Suspended process 가 있으면 깨우고 없으면 무시] |

> 그럼 P가 x.signal을 통해 Q를 깨웠다. P와 Q는 동시에 실행될수 없는데 이때는 어떻게 해야하는가?
>
> 1. Signal and wait: P가 Q를 깨우면 P가 Q가 끝날때까지 기다린다.
> 2. Signal and Continue: P가 Q를 깨우면 Q를 꺠워 놓고 P는 자기 할일 을 마저 한다. 그다음에 Q를 실행한다.
>
> 2가지 방법 모두 장단점이 있다.
>
> Consurrent Pascal: P가 signal을 받자 마자 빠져나간다.

aquire(t) 함수는 t 매개변수로서 t가 짧을수록 혹은 aquire 내부에 x.wait(t)에 t가 짧을수록 더 높은 우선순위를 가질수 있도록 해줄 수 있다. 

### 관련 코드를 확인하십시오



## Liveness

Mutex lock이나 semaphore를 동해 동기화하는 과정에서 무한 대기 상황이 발생할수 있다. 이러한 상황의 대표적인 예가 Deadlock이다. Liveness는 대기 상황 자체를 한정지음으로서 Deadlock 상황을 타개한다.

>  대표적인 Liveness-failure, Deadlock
>
> 진퇴양난의 상황으로 만약 S와 Q를 동기화 시킨다고 하면 P1과 P2가 동시에 S, Q를 wait 해버리면 아무것도 진행되지 않고 둘다 대기하는 Deadlock 상태에 진입하게 된다.

- Starvation : 프로세스가 전혀 제거되지 않고 Semaphore Queue에서 영원히 suspended 되어있는 상황
- Priority inversion: 낮은 우선순위의 프로세스가 lock되어있어 높은 우선순위의 프로세스가 진행되지 못하는 상황
  - 이 문제는 현재 자원을 소유한 프로세스의 우선순위를 자원을 요청하는 높은 우선순위의 프로세스의 우선 순위를 할당해주는 Priority-inheritance Protocal로 해결가능하다. 