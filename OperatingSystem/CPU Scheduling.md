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



### Scheduling Algorithm



### Thread Scheduling



### Multi-Processor Scheduling



### Real-Time CPU Scheduling

 

### Algorithm Evaluation