## Synchronization Example

> - Bounded-Buffer
>
> - Reader-write
>
> - dining philosopher

### Bounded-Buffer Problem

n개의 버퍼가 각각 1개의 아이템을 가질 때 mutex Semaphore는 1로 초기화하는 binary Semaphore이고 full Semaphore(몇개의 방이 채워져있는가)와 empty Semaphore(몇개의 방이 비워져있는가)는 각각 0과 n으로 초기화하여 리소스의 개수 만큼 count하는 counting Semaphore이다.

```c
//Producer 공유 버퍼에 값 추가하기
while (true){
    ...
    wait(empty);//만약 Semaphore에서 빈방 없이 0 이하이면 blocking 시킨다.
    wait(mutex);//이후 값을 추가하기위해 mutex를 이용해 한번에 1개씩만 공유자원에 접근하도록 통제한다.
    
    ...//Critical Section
        
    signal (mutex);//값이 추가가 완료 되어진다.
    signal(full);//값이 늘어 났으미 Semaphore의 개수를 늘려준다
}

//Consumer 공유 버퍼에 값 빼내기
while(true){
    wait(full);// 값이 가득 차있으면 blocking 한다.
	wait(mutex);//이후 값을 제거하기위해 mutex를 이용해 한번에 1개씩만 공유자원에 접근하도록 통제한다.
    
    ...//Critical Section
        
    signal (mutex);//값이 제가가 완료 되어진다.
    signal(full);//값이 줄었으니 Semaphore의 개수를 줄여준다
}
```





### Reader-write Problem

- Readers: 어떠한 dataset을 읽기만 할수 있고 데이터에 어떠한 변경도 수행하지 않음
- Writers: Read와 Write를 둘다 수행가능함
- 문제는 다수의 Reader는 동시에 접근가능하게 하면서 Writer은 한번에 한명씩만 접근가능하게 해야함
- 가장 대표적인 방법인 "읽기자 선호 알고리즘"으로 해결가능하다.
  - Data set: 공유 데이터 자원
  - 1로 초기화된 **rw_mutex** Semaphore: Reader와 Writer가 상호 베타적으로 구동될수 있도록하는 세마포
  - 1로 초기화된 **mutex** Semaphore: Reader가 Read_count를 업데이트 하기 위한 mutex Semaphore
  - 0으로 초기화된 int형식 read_count: 현재 진행중인 Reader의 수를 세기 위한 Count 변수

```c
//Writer 프로세스
while(true){
    wait(rw_mutex);//오직 1개의 Reader만 들어갈수 있도록 접근통제 한다.
    
    /*Critical Section*/
    
    signal(rw_mutex);
    //이 경우 multiple reader를 허용하지만 writer가 지속적으로 굶을 수 있다. 그래서 이러한 알고리즘을 "reader 선호" 알고리즘 이라고 한다.
}

//Reader 프로세스
while(true){
    wait(mutex); //한번에 한개씩 출입 절차를 받을수 있도록 통제한다.
    read_count++;
    if(read_count == 1)
        wait(rw_mutex);
    	/*read_count를 1 올렸는데 1이다? 그 뜻은 일단 C.S에 Reader는 없고 Writer만 있을수도 있도  
    	없을수도 있는 상황이다. 따라서 rw_mutex를 통해 내부에 Writer가 있는지 확인하는 절차를 거친
    	다.*/
    signal(mutex);
    
    /*Reading performed*/
    
    wait(mutex); //다시 출입 통제
    read_count--;
    if(read_count == 0)
        signal(rw_mutex);
    	/*본인이 작업을 수행하는 동안 들어오지 못했던 프로세스가 들어올수 있도록 block을 풀어줌*/
    signal(mutex);
}
```



**Writer 선호 알고리즘**: Reader의 정복을 허용하되 대기중인 Writer가 있으면 Reader가 Writer을 앞지르지 못하도록 하는방식, 반대로 Writer는 Reader를 앞지를수 있다.(읽기자 선호 알고리즘에서는 Reader가 Writer를 앞지를수 있기 때문에 Writer가 굶주리는 현상이 생기기도한다.)

> 어쨌든 상황에 따라서 읽기자 선호 알고리즘, Writer 선호 알고리즘 둘다 Starvation이 발생가능하다.

**공정한 reader-writer**: 선착순으로 C.S에 들어가면서도 Reader의 중복을 허용하나 Writer가 줄을 서면 이전에 C.S에 진입한 중복되어 접근한 Reader들이 다 끝날때까지 기다린다. 그리고 Writer 뒤에 들어온 Reader는 앞지르기를 못하고 Writer가 끝날때까지 기다렸다가 Wrietr가 끝나면 C.S에 진입한다.



### Dining philosopher Problem

![Dining-Philosophers Solution Using Monitors - GeeksforGeeks](https://media.geeksforgeeks.org/wp-content/uploads/operating_system_din.png)

5명의 철학자가 가운데 밥을 두고 철학자와 철학자 사이사이에 젓가락을 두고 밥을 먹어야한다 어떻게 동기화 시켜야 효율적으로 밥을 먹을 수 있을까?



간단한 Semaphoer Solution을 제시하자면

```c
While(true){
    wait(chopstick[i]);//왼쪽 저가락을 집음
    wait(chopstick[ (i+1) %5 ]);// 오른쪽 젓가락을 집음
    
    /*eat for awhile*/
    
    signal (chopstick[i]);
    signal (chopstick[ (i+1) %5 ]);
    
    /*think for awhile*/
}
```

위 알고리즘에는 큰 문제가 있는데 만약 모든 철학자가 각자 왼쪽에 있는 젓가락을 동시에 집어 버리면 아무도 밥을 먹지 못하는 **Deadlock**이 생겨버린다.

이런 Deadlock을 피하는 방법에는 크게 3가지 정되.,가 있는데

- 테이블에 최대 4명만 앉는다.
- 젓가락을 두개를 다 집을수 있을 때에만 젓가락을 집는다
- 짝수번째 철학자는 왼쪽->오른쪽, 홀수번째 철학자는 오른쪽->왼쪽 순서로 젓가락을 집는다. 



이러한 방법말고 Monitor Solution이 있다.

```c
monitor DiningPhilosphers{
    enum{ THINKING; HUNGRY, EATING} state [5];
    condition self [5];//배고프지만 젓가락을 집을수 없을때 기다리기 위한 조건변수
        
        void pickup(int i){//i 철학자가 젓가락을 집는 행위
            state[i] = HUNGRY;//본인의 상태가 배고프다고 저장
            test(i);// 식사를 할수 있는 지 없는지 확인, 확인후 먹을수 있었으면 EATING으로 바뀜
            if(state[i] != EATING) self[i].wait;//밥을 먹지 못했으면 먹을 때까지 기다림
        }
        void putdown(int i){// 다먹었으면 THINKING으로 전환
            state[i] = THINKING;
            test((i + 4) % 5);//좌우에 난 다먹었으니 먹을사람 먹으라고 꺠워줌
            test((i + 1) % 5);
        }
    
    	void test (int i){
            if((state[(i + 4) % 5] != EATING) && (state[i] == HUNGRY) && (state[(i + 1) % 5] != EATING)){//왼쪽 오른쪽이 EATING이 아닌 상태에서 본인이 배고픈상태라면 
                state[i] = EATING;// 먹을수 있는 상황이므로 밥을 먹는다.
                self[i].signal();//본인을 깨워줌
            }
        }
     
    	initialization_code(){
            for(int i = 0; i < 5; i++){
                state[i] = THINKING;//처음에는 모두 Thinking상태이다.
            }
        }
}
```

이 알고리즘에서는 Deadlock이 발생하지도 않지만 상황의 아다리가 안 맞아서 Starvation은 일어날수 있다.\



## Kernel Synchronization [Window]

- 프로세서가 1개인 경우 Interrupt을 차단함으로서 본인이 하던일을 끝까지 하도록 보장한다.
- 멀티 프로세서에서는 Spinlock을 이용해 짧은 처리기간 동안은 Busy waiting을 사용한다.
- window에서는 spinlock 중인 thread는 preemption이 되지 않도록 막고 있다. 
- 사용자 레벨에서는 동기화를 위해 **Dispatcher Object**를 지원한다, mutex, Semaphore, timer등의 툴과 비슷한 역할을 한다.
- Dispatcher Object의 Events에서는 어떤  조건을 만족하면 기다리던 쓰레드에게 notify해주는 conditional variable과비슷 한역할을 해주는 툴이 있다.
- Dispatcher Object의 Timers에서는 일정 시간이 만료되면 스레드들에게 notify해주는 기능도 있다.
- Dispatcher Object에는 signaled-state[object에 사용가능한] 혹은 non-signaled state[쓰레드가 block될수 있는]가 있다.
- 그래서 Dispatcher Object마다 waiting queue가 존재해서 Object가 자체가 signal-state로 바뀌면 queue에 있는 모근 스레드나 일부를 깨워준다. 



## Kernel Synchronization [Linux]

- 2.6버전 이전에는 interrupted를 disable하도록 구현
- 2.6버전에는 fully preemptive를 사용한다.
- 그래서 현재 제공하는  동기화 툴은 Semaphore, atomic integers, spinlock, mutex lock을 제공한다.
- Linux에서 spinlock과 mutex lock은 lock되어진 상태에서 다시 lock될수 없다(None recursive)하다.
- single-cpu에서는 enabling, diabling kernel preemption이 spin lock을 대체하지만, SMP에서는 Deadlock의 위험성 때문에 spinlock을 사용한다.

### Atomic Variable

연산과정 중간에 inturrupt 당하지 안도록 보장하는 atomic_t라는 타잎을 따로 제공한다.

값을 읽어와서 계산하고 저장하는 일련의 과정을 Atomic하게(중간에 깨지지 않도록 보장하는) 발생하도록 하는 과정

## POSIX Synchronization

- mutex lock, Semaphore, condition variable을 기본적으로 제공한다.

### Mutex lock

```c
//Mutex Lock의 사용
#include <pthread.h>

pthread_mutex_t mutex; // 변수선언

pthread_mutex_init(&mutex, NULL);//mutex lock 초기화

pthread_mutex_lock(&mutex); //mutex lock이 걸어 잠금

/*Critical Section*/

pthread_mutex_unlock(&mutex); //mutex lock을 품
```



Named Semaphore와 Unnamed Semaphore가 있다.

Named Semaphore: 서로 관계가 없는 프로세스에 사용되는 Semaphore

Unnamed Semaphore: 서로 관계가 있는 프로세스에 사용되어짐



```c
//Named Semaphore의 사용
#include <semaphore.h>
sem_t *sem;

sem = sem_open("SEM", O_CREAT, 0666, 1);//Semaphore을 생성해 1로 초기화한다.
//0666으로 권한이 있어야 다른 프로세스가 사용가능하다.

sem_wait(sem);//Semaphore를 걸어 잠근다.

sem_post(sem);//Semaphore를 풀어준다.
```



```c
//Unamed Semaphore의 사용
#include <semaphore.h>
sem_t *sem;

sem_init(&sem, 0, 1);//Semaphore을 생성해 1로 초기화한다.
//두번째 인자가 0이면 같은 프로세스 내에 있는 스레드 사이에서 공유하며 sem은 모든 스레드가 보이는 곳에 있어야함
//두번째 인자가 0이 아니면 프로세스 사이서 공유되긴하는데 서로 Shared Memory가 있어야함

sem_wait(sem);//Semaphore를 걸어 잠근다.

sem_post(sem);//Semaphore를 풀어준다.

//MAC에서는 Unnamed Semaphore가 사라짐
```



- POSIX에는 Monitor를 제공하지 않는다. 하지만 대신 Condition Variable을 제공함
- Condition Variabe을  사용하기 위해서는 mutaul exclusion을 위해 mutex 변수도 하나 **반드시** 동행해야 한다.

```c
pthread_mutex_t mutex;
pthread_cond_t cond_var;

pthread_mutex_init(&mutex, NULL);
pthread_cond_init(&cond_var, NULL);

//만약 쓰레드의 Condition이 a==b가 되길 기다리는 상황일때
pthread_mutex_lock(&mutex);
while(a != b){
/* pthread_cond_wait에서 mutex lock을 푸는 순간 return되었을때 a, b값이 변경되었을수도 있기 때문에 if대신 while로 조건을 다시 검사하는 것이 매우 중요하다.*/
    
    pthread_cond_wait(&cond_var, &mutex);
/*위에 a!=b 조건이 성립하면 mutex lock을 풀고 condition Value를 받아 signal을 받으면 mutex lock을 받으려한다. */
}
pthread_mutex_unlock(&mutex);


/*위 코드에서 진행되는 함수의 흐름은 다음과 같다.*/
pthread_mutex_lock(&mutex);
a = b;
pthread_cond_signal(&cond_var);
/*Signal을 보내면 조건변수에 대기 중이던 스레드를 깨우고 이때 signal을 받은 쓰레드는 mutex lock을 획득해야하므로 아직 진행이 안된 상태이다.*/
pthread_mutex_unlock(&mutex);
/*Signal을 보낸 후 unlock을 해야 비로소 signal을 받았지만 아직 대기상태에 있던 쓰레드가 mutex lock을 획득한후 진행 가능하다.*/
```





## Alternative Approach

- Transcational Memory

  - 모든연산이 올바르게 처리되어 확정이 되거나 취소되어 원점으로 롤백하는 두가지만 중간과정없이 끝나는 연산

- OpenMP

  - ```c
    #pragma omp critical {//Atomic하게 실행하라
    	count += value;
    }
    ```

  - 

- Functional Programming Languages

  - 언어마다 Synchronization 기법이 있다.

