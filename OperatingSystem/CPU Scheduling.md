## CPU Scheduling

### Basic Concept

CPU Scheduling의 목적 : CPU의 이용률을 극대화하자

프로그램을 구동할때는 CPU와 IO를 번갈아 구동되는데, 대부분의 시간을 IO에서 보낸다.

CPU에서 보내는 시간을 CPU burst라고 하고 IO에서 보내는 시간을 IO burst라고 한다.

대부분의 프로그램이 짧은 CPU burst를 가지고 있고 짧은 구간동안 앞쪽에서 몰리게 되어 있다.

CPU Scheduler는 프로세스의 Ready Queue를 통해 어떤 프로세스를 할당할지 결정한다.

<img src="https://www.researchgate.net/profile/Amar-Dash/publication/276732451/figure/fig1/AS:361016480288775@1463084444461/Different-schedulers-and-Process-states-in-CPU-Scheduling.png" alt="Different schedulers and Process states in CPU Scheduling. | Download  Scientific Diagram" style="zoom:67%;" />

<center>CPU Scheduler life cycle</center>

비선점 알고리즘(nonpreemptive): 점유가 되어 있지 않은 채로 종료과 waiting상태가 아니면 CPU를 놓치지 않음

선점알고리즘(preemptive): shared memory와 Kernel mode에 따라 CPU를 한정된 시간동안만 할당하는 알고리즘

- shared memory를 접근합에 있어서 race condition이 발생하고 있음
- Kernel mode 작업중 선정 당해 버리면 문제가 발생할수 있음
- 불가피한 OS 작업의 경우 interrupt를 disable했다가 끝나는대로 다시 enable 시켜서 보호할수 있음
- 커널의 스케줄링을 선점 비선점 둘다 가능하나 현재의 대부분의 OS는 선점으로 스케줄링을 처리하고 있음



#### Dispatcher

현재 진행중인 프로세서를 다른 프로세서로 바꿔주는 역할을 하는 SW

- Context Switching을 진행 시켜준다.
- P0 현재의 상태를 저장함 -> P1의 상태를 다시 복원해준다. -> P1을 실행한다.
- 위 작업을 진행하는데 드는 시간을 Dispatcher latency라고 한다. 

### Scheduling Criteria

- CPU Utilization(CPU 이용률) -> CPU를 얼마나 많이 사용하는 가에 대한 지표
- Throughput(처리량): 일정 시간동안 처리한 데이터의 양
- Turnaround time: 프로세스가 시작해서 끝날때까지 걸린시간
- Waiting time: 프로세스가 Ready Queue에 머무는 시간 (대기시간 = Ready Queue에 저장된 시간)
- Response time: 요청에 대하여 첫 응답이 나오기까지의 시간, 반환 시간이 같아도 응답시간이 짧았을때 더 빠르게 결과를 볼수 있기 때문에 좋다,  최근에는 프로그램 관계 없이 응답시간의 평균보다는 편차를 줄이는 것이 중요하다는 시각이 많다.

그래서 Scheduling 효율을 올리기 위해서 다음과 같은 행동이 가능하다

- CPU 사용률을 증가시킨다.
- CPU처리량을 증가 시킨다.
- 프로세스의 총 처리 시간을 줄인다.
- Ready queue에 머무는 시간을 줄인다
- 응답시간을 줄인다.

### Scheduling Algorithm

#### First Come First Served(FCFS) Scheduling

Nonpreemptive Algorithm: 선착순이기 때문에 먼저 온 놈이 무조건 CPU를 붙들고 있다.

![CPU 스케줄링(gantt chart)](https://blog.kakaocdn.net/dn/5dBWy/btqNU1q9iGA/syWyCmjXOPxvMKjjq3V7d1/img.png)

CPU Scheduling 알고리즘의 효율성을 따짐에 있어 평가 가능한 부분은 위 사진과 같이 평가할 수 있다. 다음을  보면 P1이 21초 동안 CPU를 점유하는동안 나머지 P2,P3는 waiting time을 보내고 있다. 그래서 평규 값을 매기면 18.75라는 평균 waiting time이 있다.

그러면 만약 처리 시간이 작은 순서대로 P4->P2->P3->P1 순서대로 처리한다면 어떨까? 아마 그러면 앞에 3개의 프로세스는 금방금방 처리되고 길게 처리 되어야하는 P1은 앞에 놈을 먼저 내보내고 자기 일을 하면 불필요하게 waiting time를 늘릴 필요없이 18.75에 비해 압도적으로 시간을 줄일 수 있다.

그렇게 앞에 긴 Cpu-bound Job때문에 짧은 Job이 계속적으로 CPU Bound를 기다리는 현상을 **Convoy effect라고 한다**.

#### Shorthest Job First(SJF) Scheduling

선착순 알고리즘에서의 문제점을 보고 무조건적으로 가장 짧은 작업량을 필요로 하는 프로세스를 우선적으로 배당해주는 것이다.

- 평균 Waiting time으로 보면 가장 짧고 효율적이지만 구현하기가 어렵다.

- 일반적으로 계수는 0.5로 한다.-

- SJF의 Preemptive 버전을 우리는 **Shortest remaining time first**이라고 부른다. 기존 작업 중 더 짧은 CPU 작업량의 작업이 들어오면 그 짧은 작업 먼저 처리해준다.

  > 정리하자면 다음과 같다.
  >
  > Nonpreemitive SJF: 새로 도착한 Job이 현재 실행중인 job보다 짧아도 기다림
  >
  > Preemptive SJF(Shortest remaining time first): 새로 도착한 Job이 현재 실행중인 job보다 짧으면 교체

  ![Scheduling](https://www.cs.odu.edu/~cs471w/spring11/lectures/Scheduling_files/image010.jpg)

  > 다음을 보면 예상값이 얼추 실제 CPU점유값을 따라가고 있음을 볼수있다. 
  >
  > a에 따른 Exponential averaging의 변화 예시
  >
  > a=0 : 이전 history 값을 전혀 상관하지 않겠다.
  >
  > a=1: 오직 이전 CPU Burst count를 예상값으로 사용하겠다.
  >
  > 일반적으로는 그 중간의 값을 사용함으로 a가 1에 가까울수록 이는 최근의 측정치를 과거의 측정치 보다 더 높은 비율로 반영하겠다는 성질을 띄게 된다.

> Shortest remaining time first의 예시
>
> 다음은 Shortest remaining time first의 처리 예시이다 Gantt chart를 보며 이해해보자
>
> ![Job Scheduling | Lisa Tech Blog](http://4.bp.blogspot.com/-tertfPoc5JE/VJnOfNmuOJI/AAAAAAAAAls/qrWckbxkFko/s1600/1.PNG)



> 그럼 문제는 다음 프로세스가 CPU를 얼마나 잡아먹을 지를 계산해야하는데 그거를 유추하기위해 다음 공식을 사용한다.
>
> - 답은 지금까지 사용된 프로세스의 점유시간을  기반으로 다음 CPU Burst를 예상한다. 이 공식을 **Exponential averaging**일고 한다.
>
> > (현 CPU 점유시간) = (계수(a))*(n번째 CPU 점유시간) + (1-계수(a))(이전까지의 CPU 평균 점유시간)
> >
> >  ![5. CPU Scheduling](https://t1.daumcdn.net/cfile/tistory/277E5B4157479EE416)



#### Round Robin

FCFS와 preemption의 특성을 적용한 것이다.

일정 time quantum을 두고 정해진  time quantum을 지나면 무조건 다른 process로 교체하는 방식이다.

- 절대 무한정 대기하는 경우는 없다
- 일반적으로 10~100ms의 시간을  time quantum으로 선정한다.
-  time quantum을 지내면 Time Inturrpt을 시켜주는게 기능의 핵심이다.
-  time quantum이 너무 크면 큐의 FIFO와 비슷하게 되어버리고
-  time quantum이 너무 작으면 Overhead가 발생할 요량이 너무 크기 때문에 최소한 Context switch 시간보다는 크게  time quantum를 설정해야한다.
- 일반적으로 SJF보다는 turnaround time은 더 길지만, response time은 더 짧다.
- 보통 context switching에 드는 시간은 10마이크로초이다.

> Round Robin Algorithm의 시뮬레이션 예시
>
> ![Round Robin Scheduling Algorithm | Studytonight](https://s3.ap-south-1.amazonaws.com/s3.studytonight.com/tutorials/uploads/pictures/1604904974-71449.png)
>
> 여기서 Round Robin의  time quantum은 5이다. 잘 이해하도록하자

> ##### 경험의 법칙(rule of thumb)
>
> 보통적으로 보아하니 프로세스의  Time quantum은 각 프로세스의 80%에 해당하는 time을  time quantumdm으로 선정하는것이 좋더라는 법칙이다.
>
> 많은 경험을 기반으로 얻어진 데이터이다.
>
> ![CPU 스케쥴링](https://blog.kakaocdn.net/dn/QTuKh/btqAnY9hOkB/WVVCs1NkaK3YBGfQSW2rs1/img.png)
>
> -> 이가 의미하는 바는 대부분의 프로세스가 Time quantum내에 종료될수 있다면 turnaround time을 줄일수 있다.라고 생각할수 있다.



#### Priority Sheduling

- 각 프로세스가 우선순위를 의미하는 integer 값을 기준으로 스케줄링한다.

- 윈도우에서는 우선순위의 값이 높으면 우선이지만  Linux에서는 우선순위가 낮은걸 우선순위로 한다.

- 이 또한 Preemptive 방식과 Nonpreemptive방식이 있을수 있다.
- SJF는 Predicted CPU Burst time의 역순으로 프로세스를 실행함으로 Priority Scheduling의 일종이라고 할수 있다.  

문제점: 우선순위가 낮은 놈들은 영원히 실행되지 못하는 경우가 생길수 있다. **Starvation**

해결법: 기다린지 오래된 프로세스들의 우선순위를 일부러 올려주는 방법이 있을수 있다. **Aging**

![Priority Scheduling Algorithm | Studytonight](https://www.studytonight.com/operating-system/images/priority-scheduling.png)

> 다양한 Scheduling Senario
>
> ##### Priority Scheduling + Round Robin
>
> <img src='http://drive.google.com/uc?export=view&id=1rl0Dy7Be1oLXA0u6seuozeuqXWfqnhPD' style="zoom:50%;" />

#### Multilevel Queue

우선 순위마다 큐를 배정해주되, 같은 큐 안에서는 Round Robin이나 다른 방식으로 Process를 Scheduling하는 방식이다.

일반적으로는 Process type에 따라 우선순위를 결정한다.

높은 우선순위의 프로세스를 무조건 먼저 실행할수도, 큐에 따라 Time Slice를 배정할수도 있다.

목적에 따라 Scheduling Algorithm은 완전히 다를수도 있다.

![best finisher: Operating System | Multilevel Queue Scheduling | Multilevel  Feedback Queue Scheduling](https://lh4.googleusercontent.com/proxy/lYxelQRhUPRflIM9oycgdw517LlY4B5v8lMNCQLEPiyKwi45u8i-v9o8WKTFEyChyUjSafPQHWwMm3chpsD6sUvuOCH2EZTo0pNDQiwbm25Kkqe98_wrdya-5jgkIqWv6O_6UQ=s0-d)

#### Multilevel Feedback Queue

큐 자체가 너무 오래 CPU를 잡고 있으면 Queue 자체를 내리고 너무 오래 기다리는 Queue를 올려주는 방식

이를 위해서는 큐의 개수, 어떤 스케줄링 알고리즘인지, 어떤 방식으로 순위 상승/하강을 결정할건지, 등을  결정해야 운영이 가능하다.

가장 일반적인 CPU 스케줄링 알고리즘이기도 하지만, 가장 복잡한 알고리즘 이기도 하다.

앞에서 언급한 모든 개념을 적용할수 있기도 하다.

![The Performance Of Multi-Level Feedback Queue | by Malith Jayasinghe |  Medium](https://miro.medium.com/max/954/1*unzQaXEn8R92AK1cgmEgwQ.png)

https://jhnyang.tistory.com/156



### Thread Scheduling

- Thread Scheduling에 있어서 Thread는 "User level Thread"와 "Kernel level Thread"로 구분됨

- Thread기능을 지원할경우 Scheduling의 단위를 Precess가 아닌 Thread로 봄
- Many to one 방식이나 Many to Many 방식에 있어서 한개의 프로세서안에서 여러개의 Thread들이 경쟁하게 되는데 이때 Thread는 LWP라고 불리는 가상의 프로세서에서 Scheduling 되어지며 
- 위 설명 처럼 한개의 프로세서 안에 여러개의 쓰레드를 담고 있는 상태를 Process-contention Scope(PCS)라고 부른다. 여기서 쓰레드들의 우선순위는 전적으로 프로그래머의 몫이다.
- C언어에서 Thread를 다루는 Pthread 라이브러리는 OS에서 user-level에서 구현될수도 Kernel-level에서 구현될수도 있기 때문에 2가지 Scope가 존재하며 
- PTHREAD_SCOPE_PROCESS / PTHREAD_SCOPE_SYSTEM 이렇게 2개로 나누어진다. \
- 다만 Linux와 MAC OS에서는 PTHREAD_SCOPE_SYSTEM만을 허용하고 있다.

<Pthread 함수를 다루어 SCOPE를 다루는 코드>

### Multi-Processor Scheduling

CPU가 여러개 가지고 있으면 CPU Scheduling 알고리즘은 더 복잡해짐

그래서 Multiprocessor는 다음과 같은 구조에서 주로 쓰인다

- Multicore CPU
- Multi Thread Core
- NUMA(Non Uniform Memory Access) system 
- Heterogeneous multiprocessing: 모바일에서 전원을 절약하기 위해 동일하지만 성능이 떨어지는 코어를 추가로 가지는 형태

Symmetric multiprocessing: 각각의 프로세서가 각자의 스케줄링을 알아서 관리함

Asymmetric multiprocessing: 스케줄링을 담당하는 프로세서가 따로 존재

task를 관리하는 방법에는 여러가지가 있다.

1. 모든 쓰레드는 일반적인 Ready queue를 공유하는 방식: Ready Queue를 공유하기 때문에 race condition(모든 코어가 하나의Queue에서 경쟁하면서 발생하는 동기화 문제)이 생길 우려가 있어서 high Contention이 발생할수 있다. => 별도의 동기화 알고리즘 필요
2. 각 프로세서가 각자의 Queue를 가지고 있는 방식: 가장 일반적이고 성능이 좋긴한데 Work load를 조절해주는 Workload balancing 알고리즘이 필요함

##### Multicore Processor

최근에는 같은 칩 내에 여러개의 코어가 드렁가 있음 => 빠르고 경제적임

Multiple Thread: 하나의 코어에서 여러개의 Thread를 지원하고 있음 왜? memory stall 때문에 

Memory stall: 메모리를 엑세스하는동안 메모리가 정리 해야하는 상태

![](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter6/6_11_MultithreadedMulticore.jpg)

'M'에서는 아무것도 안하는 상태임 => utilization 저하

그래서 그 사이에 계산을 하자! =>Hardware Thread인 Multi threading

그리고 구분을 위해 꼭 고유의 IP와 Register set을 가지고 있어야 한다.

그래서 각코어는 한 쌍의 Thead를 번갈아 시행할수 있으며 이를 **Chip-multithreading(ref. intel hyperthreading)**이라고 한다.그래서 예를 들면 퀴드토어를 가진다면 논리적으로는 8개의 hardware가진 거처럼 보이게 된다.



##### Multithreaded Multicore System

>  ![Chapter 5 CPU Scheduling Operating System Concepts 10](http://drive.google.com/uc?export=view&id=1gNAPQxK35UzQl7iAeFYwid4JIMypXh8X)
>
> level1에서는 모든 알고리즘적 스케줄링 알고리즘이 적용 
>
> level2에서는 하드웨어 쓰레드를 시행하여 적용함

> ##### Load balancing
>
> SMP에서 Load balancing를 통해 각 코어에 일을 분배 해야해..
>
> Push mighration: 주기적으로 일의 분배 상황을 검사하여 적절히 옭겨줌
>
> pull migration: idel processor가 자발적으로 다른 일하는 core의 일을 가져오는 방식
>
> HMP에서는 자체적으로 전원 절약을 위해 성능이 떨어지는 CPU를 구동할수도 있다.

> ##### Processor Affinity
>
> 프로세서가 사용한 데이터는 cache에 저장되게 되는데 프로세서 데이터의 무결성을 보장할수 있도록 해주는 것이 Processor Affinity라고 한다.
>
> 그런데 Load blancing을 위해 migration을 하며 이것이 깨질수도 있는데 이를 위해 Processor Affinity가 보장되는 공간에 할당하려고 노력한다.
>
> - Soft Affinity : 최대 보장할수있도록 신경서서 운영하는데 보장은 못한다
> - Hard Affinity : 앗싸리 프로세서 데이터 저장공간을 지정하여 무결성을 보장함
>
> 리눅스의 경우 Soft Affinity를 보장하지만 따로 함수를 통해 Hard Affinity를 지정할수 있음

> ##### NUMA and CPU Scheduling
>
> CPU가 관장하는 메모리 영역을 정하고 Thread를 구동하면서 가장 가까운 메로리 영역을 할당하도록 하는 운영방식을 NUMA-aware라고 한다.
>
> 참고로 다른 CPU의 메모리 영역에 접근하려고 할 경우 속도가 느리다.
>
> <img src='http://drive.google.com/uc?export=view&id=1VFJHDOSgBRCbx9EuSXYzqLJYO5vKSlA0' style="zoom: 67%;" />

### Real-Time CPU Scheduling

 Soft realtime System: 지정된 시간내에 최대한 끝내 보겠지만 보장은 못한다.

Hard realtime System: 지정된 시간내에 반드시 프로세스를 끝내야한다. 못끝내면 실행 못한다.

Event latency: 이벤트가 발생하고 인지해서 그에 받는 서비스를 시행하기 까지의 지연시간

Realtime system의 성능에 영향을 주는 latency의 종류는 2가지가 있다.

1. Interrupt latency

    이벤트가 발생하고 Cpu에 이벤트 발생 사실을 알려 하던 일을 멈추게 하기 까지의 시간 

2. Dispatcher latency

   - 현재 CPU가 실행 중이던 일을 멈추고 다른 일로 교체하는데 사용되는 시간 

   - Hard real time Sytem에서는 이게 마이크로초 단위이다.

   - Preemption: 하던일을 멈추는 작업시간

   - Release: 새로운 코드를 실행하는데 드는 시간

     ---

     

<img src="https://img1.daumcdn.net/thumb/R720x0.q80/?scode=mtistory2&fname=http%3A%2F%2Fcfile2.uf.tistory.com%2Fimage%2F99C1973C5B01AE0C06C33F" alt="Real-Time CPU Scheduling"  />

---

![Real-Time CPU Scheduling](https://img1.daumcdn.net/thumb/R720x0.q80/?scode=mtistory2&fname=http%3A%2F%2Fcfile10.uf.tistory.com%2Fimage%2F99157A355B01B3D80657AE)

---

#### Realtime Scheduling Algorithm

- Real-time scheduling을 위해서는 일단 Soft realtime에 있어서는 반드시 Preemptive하고 priority-based scheduling이어야만한다.

> #### Periodic Algorithm
>
> 주기적으로 프로세스를 발생시키되 Processing time t, dealline d, period p를 가지고 반드시 p는 d 이하이도록 하는 방식을 말한다.
>
> Admission-control algorithm: 프러세스에게 deadline을 요청했을때 받아들여서 주어진 시간 내에 종료하든지 앗싸리 시작을 하질 말든지하는 이분법적인 알고리즘으로 Real time System에 많이 쓰인다.
>
> ![Periodic scheduling concepts | Download Scientific Diagram](https://www.researchgate.net/profile/Andreas-Mauthe/publication/220937635/figure/fig2/AS:669375438790658@1536602949187/Periodic-scheduling-concepts.png)

> #### Rate Monotonic Scheduling
>
> - Periodic Process Scheduling에 Static Priority Policy + Preemption을 적용
>
> - 프로세스의 우선순위는 Perodic 주기의 반비례한다.
> - 만약 Periodic Process의 CPU Burst가 모두 동일하다면 P(주기)가 짧을수록 더 높은 우선순위를 가진다.
> - ![why p1 misses it first deadline? Rate monotonic scheduling - Computer  Science Stack Exchange](https://i.stack.imgur.com/rrHEL.png)
> - CPU Utilization P = t/p
> - 정적 우선순위를 사용하는 알고리즘중 가장 optimal한 알고리즘이다.

> #### Rate Monotonic Scheduling시 발생가능한 문제
>
> 다음 예제를 보도록하자.
>
> <img src='http://drive.google.com/uc?export=view&id=11RtYkUORpVMWMCTWL3qP_4h3FWDA8nXx' />
>
> 위 상황의 경우 P2는 80에 있는 P2의 Deadline을 지키지 못한다.그런데 이게 CPU가 게을러서가 아니라 우선순위가 더 높은 P1에게 양보를 하다보니 P2는 어쩔수 없이  Deadline을 지키지 못한것이다.  다음 값에서 이 상황을 피할수는 없지만 여러개의 Process을 동시에 진행하는 경우에 CPU utilization이 일정 수준을 넘어서면 Deadline을 못지킨다는 계산을 할수는 있다.
>
> 다음 식의 경우 CPU Utilazation은 25/50+35/80 = 94%으로 꾀 높다. 
>
>  Rate Monotonic Scheduling문제 발생 예측식은 
>
> N개의 프로세스를 스커줄링할때  CPU Utilazation이 N(2^(1/N) -1)을 넘어서는 안된다.
>
> 2개의 프로세르의 경우 83%인데 위예제는 94%로 스케줄링이 불가능하다고 할수 있다.

> #### Earliset Deadline First Schedule(EDF)
>
> - Deadline이 더 적게 남을 수록 우선순위가 높다.
>
> - 주기일 필요도 CPU Burst가 정해져있을 필요도 없다.
>
> - 오직 Deadline만 알려준다면 CPU Untilization 면에서는 최적의 알고리즘이다.
>
> <img src='http://drive.google.com/uc?export=view&id=1-PTX7NpnpOliWqsE4zIs6ffZSMiWlPzk' />
>
> 위 예제에서 최전방에 P1의 데드라인 있으니 일단 P1을 먼저 해결하고 첫번째 P1이 끝나면 그다음 P2가 가장 급한 데드라인이 된다 그 시점부터 P2을 진행하고 이런식으로 제일 급한 데드라인 task 부터 해결해 나가는 것이다.
>
> - EDF는 이론적으로 CPU Utilazation을 100%까지 올릴수 있는 최적이 알고리즘이 맞으나 현실에서는 CPU Context switching같은 추가비용이 들기 때문에 100% 이용은 불가능하다.

> #### Proportional Shared Acheduling
>
> - 프로세스들이 CPU의 자원을 100이라고 봤을떄 각 프로세스가 차지하는 자원의 비율을 따지고 들어가는 알고리즘
> - 이와 같이 CPU Util을 매번 집계하다가 다른 프로세스가 들어오려고할때 자원이 부족하다고 판단하고 거절할수도 있다.

> #### POSIX System에서 Real-Time System
>
> pthread_attr_getsched_policy(pthread_attr_t *attr, int *policy)
>
> pthread_attr_setsched_policy(pthread_attr_t *attr, int *policy)
>
> - pthread_attr_setsched_policy의 경우
> - SCHED_FIFO: 선착순
> - SCHED_RR: Round Robin
> - SCHED_OTHER:구현이 정의 되어있지 않음

### Algorithm Evaluation

> #### Linux's Scheduling
>
> - 현재 Linux는 Completely Fair Scheduler(CFS)를 사용한다.
> - Time quantum의 최소한 한개의 task를 수행할 수 있는 target latency를 책정하여 nice value(값이 높을수록 우선순위가 낮아진다.)를 선정해 Time quantum을 정한다. 
> - 각 프로세스의 가상 실행시간이 작을수록 Priority가 높다. 기본적으로는 nice value가 0이라서 본인의 런타임이 가상 실행시간이 된다.  우선순위가 낮으면 본인의 실제 런타임보다 가상 런타임이 크게, 동적으로 책정된다. 
> - 처음 실행되는 일은 virtual runtime이 0이라서 빠르게 처리된다.
>
> - Real time Scheduling은 정적인 우선순위를 가지고 있다.
> - Scheduling domain은 Load balancing이 가능한 코어의 집합이다. 이들은 같은 도메인 내에서 쓰레드가 이동가능하도록한다.





> #### Window's Scheduling
>
> - Priority-based Preemptive
> - 번호가 높을수록 우선순위가 높음
> - Variable Class로 우선순위가 변경이 가능하다

