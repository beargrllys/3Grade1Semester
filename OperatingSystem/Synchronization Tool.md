## Synchronization Tool

- 프로세스는 병행적으로 실행되는데 그래서 언제든 다른 프로세서에 의해 inturrupt될 수 있음
- 만약 프로세서들이 Shared memory를 사용한다면 작업 도중에 inturrupt되어서 작업에 불일치성(inconsistency)가 생겨버리는 문제가 발생한다.
- 그래서 프로세서들이 순서대로 시행될수 있도록 보장하는 메커니즘이 데이터에 일치성을 유지하는데 요구 되어짐

> producer-consumer problem 
>
> 버퍼를 운용할때 버퍼가 빈 상태와 꽉찬 상태를 구분할수 없음 그래서 count 변수를 이용하여 한정 된버퍼에서 공간이 비었는지 꽉찼는지 구분할수 있음
>
> ```c
> //Producer
> while(true){
>     while(counter == BUFFER_SIZE){
>         ;/*Do nothing*/
>     }
>     buffer[in] = next_produced;
>     int = (in + 1) % BUFFER_SIZE;
>     counter ++;
> }
> ```
>
> ```c
> //Consumer
> while(true){
>     while(counter == 0){
>         ;/*Do nothing*/
>     }
>     next_consumed = buffer[out];
>     out = (out + 1) % BUFFER_SIZE;
>     counter --;
>        
> }
> ```

### Race condition

- 순서가 바뀌면 결과가 달라지는 경우 count변수를 이용한 동기화에서는 counter++하는 작업과 counter--하는 작업은 각각 3가지 작업으로 이루어진다.

- > counter++
  >
  > load: 레지스터 1에 counter를 가져옴
  >
  > inc: 레지스터 1에 1dmf 더함
  >
  > store: counter에 register를 저장함

  > counter++
  >
  > load: 레지스터 1에 counter를 가져옴
  >
  > inc: 레지스터 1에 1dmf 더함
  >
  > store: counter에 register를 저장함

  이 때 만약 사용자가 counter에 1을 더한후 1을 빼는 작업을 한다고 했을때, 레지스터에 counter를 저장한 상태에서 작업이 완료되지 않은 상태에서 counter--가 작업을 게시하여 레지스터에 아직 1이 더해지지 않은 값을 가져와 1을 빼고 마직막에 1이 빠진 수를 counter 메인메모리에 저장하면 사용자의 의도와는 다른 일이 생길 것이다. 이를 **Race condition**이라고 부른다.

- 다른 예로 서로다른 프로세스 2개가 동시에 fork를 했을때 운이 안좋으면 동일한 pid를 가지는 두 프로세서가 생성될수도 있다.

### Critical Section Problem

- 만약 시스템에 프로그램이 n개가 있을때 각 프로세스가 접근가능한 공유된 자원을 critial section이라고 한다. 이때 따로 처리를 통해 한 프로세서가  critial section을 사용중이면 다른 프로세서 가 접근하지 못하도록 막아야한다.
- 그래서 코드상에 critial section이 시작되는 부분을 entry section,  끝나는 부분은 exit section이라고 부른다.

## Solution

- Mutual Exclusion(상호 베타): 어떤 프로세서가 critial section에 들어와 있으면 다른 프로세스는 들어오지 못하도록한다
- Progress: critial section에 아무 것도 사용되지 않으면 다른 요구 프로세스를 진행 시켜야한다.
- Bound waiting: 사용간에 다른 프로세스를 막되, 그 프로세스들을 무한히 기다리게 해서는 안된다.
  - 각각의 프로세스의 속도는 waiting을 처리하는데 있어서 고려하지 않는다.

### Critical-Section  handling in OS

-  Preemptive한 OS와 non-preemptive한 OS에 따라 2가지 접근법으로 critial section을 다루어야한다.
- Preemptive한 OS에서는 SMP 구조를 디자인하기 쉽지 않다.
- non-preemptive한 OS는 비교적 디자인하기 수월하다.
- 단일코어 시스템에서는 하는일이 1개이니 interrupt disabling을 사용하여 critial section를 보호할수 있지만 다중코어 시스템에서는 여의치 않음

### Peterson's solution ★

- Software 단에서 critial section를 다루기 위한 방법
- 2개의 프로세서에서만 적용되는 방식이며 3개 이상에서는 사용이 불가하다.
- 하나의 프로세서가 진행 중에는 inturrupt되지 않는 가는 가정을 깔고 간다.
- 2가지 동작으로 구동된다. 1. int turn(누구 차례인가?) 2. flag[2]={true, false}

```c
while(true){
    flag[i] = true;
    turn = j;
    while(flag[j] && turn == j){// 상대방이 들어갈 의사가 없거나 내 차례가 돌아오면 Critical Section을 실행한다.
        /*Critical section*/
        flag[i] = false;
        /*Remainder section*/
    }
}
```

-  turn 변수를 이용해 선택될 프로세스가 상호 베타적으로 선택되어짐으로 Mutual exclusion이 일어남
- 두 프로세스가 반드시 진행 되어짐
- 한번만 기다리면 반드시 변수가 바뀌어짐
- 고로  critial section 문제를 해결하기 위한 모든 조건을 만족함
- 하지만 현대의 Architecture에서는 적용되지 못함 왜냐하면 컴파일러가 최적화를 위해 종종 코드의 줄 순서를 바꿔 실행하는 경우가 있는데 이떄 Single thread에서는 괜찮지만 Multi thread에서는 코드의 종속성 때문에 문제가 생겨버릴수 있음



### Synchronization Hardware

- 많은 시스템이 critial section 코드를 구현하기 위해 하드웨어적 지원을 하고 있음

- 만약 단일 프로세서였다면 disabling interrupt를 사용할수 있었겠지만 멀티프로세서에서는 너무 비효울적임
- 그래서 하드웨어 단에서 이용되는 지원방법이 1. Memory barriers 2.Hardware instruction 3. Atomic variables 가 있다.

#### Memory berriers ★

- Strongly order: 특정 메모리에 변경이 일어나면 즉시 다른 값들에도 변경 사실을 전파하는 행위
- wealy orderd: 즉시는 아니고 적당히 변경 사실을 전파함
- 그래서 memory barrier가 일어나면 현재 진행 중인 load와 store이 종료되고 나서 새로운 load와 store이 일어나도록 함

```c
//Thread 1
while (!flag)
	memory_barrier();//X값이 로딩되기 전에 flag값이 로딩되는것을 보장함
print x;

//Thread2
x = 100;
memory_barrier();//x = 100; flag=true보다 먼저 완료되는것을 보장함
flag = ture;
```

Memory barrier는 아주 낮은 수준의 low-level operation으로 커널 개발자가 multual extension 코드를 특별히 작성할때만 쓰임

#### Hardware instruction

특수한 hardware instruction은 word의 내용을 test-and-modify해주거나 두 word의 내용을 atomically(uninterruptibly)하게 swap해준다. 그래서 이러한 instruction에는  **Test-and set** instruction와 **Compare-and-Swap** instruction이 있다.

다음과 같은 상황이 벌어질 경우 Critical Section Problem이 일어날수 있다.

```c
//Process 1
//1. 기존에 진행중이던 프로세스 정지: lock = FALSE
while(lock){//2. 반복문 중지 됨
    /*do nothing*/
}   
//3.이때 Process2 가 inturrupt함
lock = TRUE;
//7. Process1 동시에 CS진입 => 에러발생
C_S();

lock=FALSE;



//process 2
//4. 이미 lock은 FALSE임
while(lock){//5. 반목문은 넘어감
    /*do nothing*/
}   
lock = TRUE;//6. lock이 TRUE가 됨
//7. Process2 CS진입
C_S();

lock=FALSE;
```



##### test-and-set instruction

- 아래 코드와 동일한 기능을 가진 단일 instruction을 만든다면 이전 값을 받아서 현재 쓰려는 코어에 누군가 사용중인지  확인할수 있는 방법이 될것이다. single machine instruction

```c
boolean test_and_set (boolean *target){// 단일 instruction이어야한다.

	boolean rv = *target;// 현재의 값을 받아와 저장하고
    *target = true;// target은 true로 만들어 줌 
    return rv;// 저장 해뒀던 이전 값을 리턴

}
```

그런 위 항수를 이용하면 어떤 효과를 볼수 있을까?

```c
do{
    while(test_and_set(&lock))//반목문을 통해 lock해줌 => spin-lock(스핀락) :CPU를 소모함
        /*위에서 문제가 되었던 lock값이 바뀌고 나서 inturrupt되는 문제는 test and set으로 이전 값을 받고 사전에 target을 true로 만들어주기 때문에 inturrupt가 생기더라도 반복문이 패스 되지 않는다.*/ 
        ; /*do nothing*/
    		/*Critical section*/
    	lock =false;
    		/*reminder section*/
}while(true);
```

리턴 값이 TRUE이면 다른 프로세스가 CS에 있다는 의미이고 리턴값이 FALSE이면 CS에 아무것도 없다는 의미이다. 그리고 lock을 이미 TRUE로 만들었기 때문에 다른 프로세서는 CS에 들어오지 못하게 된다. 



##### Compare and SWAP instruction(CAS instruction)

다음 함수는 Compare and SWAP instruction을 위해 single instruction으로 수행되어져야하는 함수이다.

atomically instruction

```c
int compare_and_swap(int *value, int expected, int new_value){
    int temp = *value;
    
    if(*value == expected)
        *value = new_value;
    return temp;
}
```

값이 예측값과 일치하면 new_value로 바꾸어 리턴해줌, 다르면 변화없이 original 값을 리턴해줌

```c
// 만약 이전에 lock값이 0으로 초기화 되어 있다면
while(true){
    while (compare_and_swap(&lock,0,1) != 0)
        /*lock이 0이므로 0과 비교하여 동일 하다면 아무도 쓰지 않는 것으로 판단하여 0이 리턴 되고 new value가 1이되어 다음부터는 외부 프로세스 접근을 막고 본인이 실행함*/
        /*lock이 1이라면 0과 비교하여 다름으로 누군가 쓰고 있는 것으로 판단하여 1이 리턴 되여 대기함*/
        
        ;/*Do nothing*/
    /* critical section*/
    
    lock = 0;
    
    /*reminder section*/
}
```

전적으로 test_and_set과 기능적으로 동일 함.