## 컴퓨터 네트워크 프로젝트1 보고서

> ICT융합학부 (컴퓨터 전공)
>
> 2017013072 윤지상
>
> 작성일: 2021.04.10

#### 1. Give a high-level description of your server’s design

본 서버 디자인은 다음과 같은 과정을 거치도록 제작되었습니다.

1. 포트번호를 인자값으로 받아서 인자로 받은 포트를 생성하고 bind해줍니다.
2. 브라우져에서 접속을 시도하면 서버는 listen하다가 request가 들어옵니다.

> 접속해야하는 URL:     **http://localhost:{PORT}**   (default : index.html)

3. 들어온 Request에 대한 정보를 받으면 받아온 메세지를 파싱합니다.

4. 파싱한 Request의 종류와 파일명을 구분합니다.

5. 서버 디렉토리에 파일이 있는지 검색하고, 파일이 있다면 해당 파일의 바이너리를 가지고 옵니다.
6. html파일에 존재하는 콘텐츠는 따로 Request message로 콘텐츠를 가져옵니다.

7. http message의 Header를 Request에서 요구하는 Content-type을 확인하고 그에 맞게 Header에 Content-type을 설정해줍니다

8. 모든 http message가 완성되면 client에 보내주고 통신을 마무리합니다.



#### 2. What difficulties did you face and how did you solve them?

이번 프로젝트를 진행하면서 어려웠던 점은 다음과 같습니다.

1. 프로젝트에서 어떤 출력물을 요구하는지 이해하기 어려웠습니다. 명확하게 어떤 행동을 구현해야 하는지가 조금 애매했던거 같습니다. => *과제 설명서와 친구들에게 물어봐서 어찌어찌 이해했습니다.*
2. http 구동 절차 전반에 대한 이해가 어려웠습니다. 코드 자체는 서버사이드에만 있는데 그게 아무것도 모르는 클라이언트가 뭔줄 알고 메세지를 반환하는지 이해하기 힘들었습니다. => *클라이언트에서 오는 Request message는 브라우져에서 자동적으로 보내진다는 것을 알았습니다.*
3. 소켓을 다룰때 IP와 포트를 저장하기 위해 헤더파일에서 가져온 어려가지 구조체들을 취급하는 것을 익히는 것이 어려웠습니다. => *구글에서 차근 차근 이해해 나갔습니다.*
4. http message를 이루는 여러 구성 요소들이 따로 따로 만들어져서 합쳐지는 과정을 구현하기 힘들었습니다. => *다른 코드를 참고하여 조립하듯 코딩할수 있었습니다.*

#### 3. Include and briefly explain some sample outputs of your client-server

- 제 서버를 구동하기 위한 과정입니다. 압축파일에는 다음과 같은 파일이 있습니다.

> httpServe.c : 서버 코드가 있는 소스파일입니다.
>
> Makefile : 지시에 따라 만든 Makefile입니다.
>
> Index.html : 코드 실행시 최초로 전시되는 html 코드입니다.
>
> MYH.jpeg, MYH.gif, MYH.mp3, MYH.pdf : html 코드에 필요한 리소스 파일입니다.

- Makefile이 있기 때문에 **make** 명령어로 정상적으로 컴파일 가능합니다.
- 코드 실행을 위해 **./httpServe {PORT}**로 실행 가능합니다.
- 이후  **http://localhost:{PORT}**로 브라우져를 실행하면 다음과 같은 출력물이 나옵니다.



> ### 출력값1
>
> ![1](/Users/multitab/Desktop/3Grade1Semester/ComputerNetwork/1.png)
>
> 우선 기본적인 텍스트와 jpg 사진이 정상적으로 출력됩니다.



> ### 출력값2
>
> ![1](/Users/multitab/Desktop/3Grade1Semester/ComputerNetwork/2.png)
>
> 그리고 움직이는 GIF파일이 정상적으로 전송되어 반복 재생되어야 합니다.



> ### 출력값3
>
> ![1](/Users/multitab/Desktop/3Grade1Semester/ComputerNetwork/3.png)
>
> 마지막으로 크롬 형식 브라우져에 한하여 mp3플레이어와 pdf 뷰어가 정상적으로 작동합니다.





이상으로 프로젝트 결과물 보고서를 마치겠습니다.

