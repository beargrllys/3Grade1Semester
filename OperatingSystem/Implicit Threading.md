### Implicit Threading

과도한 쓰레드 생성을 막기 위해 사용자가 쓰레드를 생성하는것이 아닌 컴파일러나 run-time library가 직점 쓰레드를 요청하는것 

개발자는 병령로 수행할수 있는 task를 식별하는데 집중하고, 쓰레드는 컴파일러나 런타임 라이브러리가 담당하는걸로.. 

### thread pool

시스템에 필요한 쓰레드를 미리 생성해놓고 필요할때 할당함 -> 상대적으로 빠름

어차피 생성되어있는 thread에서 할당되는것이기 때문에 pool의 크기가 넘어갈 일은 없음

Task의 실행과 Task 생성을 분리하여 Task가 효율적으로 처리될수 있도록 함



### Fork-Join Parallelism

어떤 Task를 실행할때 해당 Task를 실행하기 위해 2개의 Task로 Fork를 나누고 종료되었을때 다시 Join하고 다시Fork하는 방식으로 divide-conqure 방식과 비슷하기 구동된다. 

![Fork-Join算法解析- 知乎](https://pic1.zhimg.com/v2-89b583e5b8eaf07d22cf0bd955be91ec_720w.jpg?source=172ae18b)

### Open MP

컴파일러 지시문의 일종으로 특정 지시문을 기기의 코어의 개수만큼 쓰레드를 생성함

그래서 각 코어가 내뿜는 출력값으로 인해 출력들이 겹쳐보일수 있음

```c
#pragma omp parallel
```

N개의 iteration 만큼 코어의 갯수 만큼 분할 매핑 할수도 있다.

```c
#pragma omp parallel for
for(i=0; i< N; i++){
    c[i] = a[i] + b[i];
}
//위 예제의 경우 data의 종속성이 없어서 병령실행이 가능하지만 만약 종속되어 있을경우 주의해야한다.
```



### Grand Central Dispatch

MAC OS에서 Thread pool의 크기를 자동조절하는 기능 으로 POSIX pthread로 구현되어 C,C++, object-C로 구현가능

```c
^{printf("I'm a blcok")} //해당 지시문을 Thread로 실행한다.
```

유저 입장에서는 thread를 직접 생성할 필요없이 편하게 thread 이용가능

Block자체는 dispatch queue에 들어갔다가 사용가능한 thread가 생기면 그때 할당하는 방식

#### dispatch queue의 종류

1.  Serial: block을 한번에 1개씩 Queue에서 block를 제거하면서 동작을 진행함, main queue라고 불림. 프로그래머는 프로그램 내에서 추가적인 Serial queue를 생성할수 있음

2. Concurrent: 시스템전체에서 block이 한번에 어러개씩 실행될수 있도록 관리하는 queue로 4개의 level로 나뉨

   Concurrent(System-wide queue / global dispatch queue)의 4가지 종류

   - QOS_CLASS_USER_INTERACTIVE: 신속한 응답을 요구함

   - QOS_CLASS_USER_INITIATED : 적당한 시간의 응답을 요구함

   - QOS_CLASS_USER_UTILITY : 긴 시간을 요구함

   - QOS_CLASS_USER_BACKGROUND :시간에 관계없음

   위로 갈수록 우선순위가 높음



### Threading에서 생길수 있는 문제

1. Sementic of Fork()와 Exec()에 대한 이슈
2. 내부에서 발생하는 Sychronous Signal Handling (illegal memory access, divided by zero)
3. 외부에서 발생하는 Asynchronous Signal Handling (Ctrl+C, timer)
4. Thread 중지하기 : Asychronous, deffer(지연)



##### Sementic of Fork()와 Exec()에 대한 이슈

- fork()를 했을때 호출한 thread만 복사할 것인가? 아니면 모든 thread를 복사할것인가?(UNIX는 일단 둘다 지원하고는 있다.)
- exec()이 수행되었을때 어차피 진행중이던 프로세스에 새로운 프로세스가 덮어 씌워지는 것이므로 호출한 thread만 복사하는 것이 유리하다.



##### Signal Handling

Signal: UNIX시스템에서 특정 이벤트가 일어났다는 것을 프로세스에게 알리는 신호

이 신호를 처리해주는 것을 **handler**이라고 한다.

1. 특정이벤트에 의해 발생
2. 프로세스에게 전달됨
3. default: 시스템에 기존에 정의된 handler도 있고 user-define으로 사용자가 개조 가능한 handler도 있다.
4. user-define signal handler는 default signal handler를 덮어 씌울수있다.
5.  만약 Multi thread라면 여러가지 thread 중에 signal이 발생될 thread로 가야한다.
6. 경우에 따라 여러 thread 혹은 특정 thread에 전달되기 도 해야한다. (ex: kill, pthread_kill)
7. 대부분의 UNIX계열 OS에서는 스레드 마다 signal을 받을지 말지를 설정해줄수 있다.



### Thread Cancellation

-  쓰레드가 종료되기 전에 강제 종료시키는 행위

- Asynchronous cancellation: 목표 thread가 즉시 종료됨 -> 할당 자원을 미반납 할수가 있음
- Deferred cancellation: thread의 철회를 지연시켜서 철회 준비 작업후 종료시킴
- 위에서 종료시점을 blocking system call이 호출될때  cancellation point가 되어짐
- 각 프로세스는 Cancellation을 받을지 말지를 설정할수 있는  Mode가 있으며 off, defferred, Asynchronous 모드가 있다. 
- 만약 Cancellation signal이 들어왔는네  thread가 off라면 해당 신호는 적용되지 않고 멈취져 있다.

```c
pthread_testcancel() // 인위적으로 cancellation_point를 지정할수 있음
    // thread의 mode에 따라 리턴될수도 리턴되지 않을수도..
```



### Thread local storage

- Thread 자체도 자신만의 data가 필요할수 도 있기 때문에 마련된 자체 공간
- 튿히 thread pool과 같이 유저가  thread의 생성을 관할할 권한이 없는 경우에 유용함
- static변수와 비슷한 느낌