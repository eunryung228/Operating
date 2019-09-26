OS Project 01 Buffer Cache 구현
  메모리 관리 기법 중 하나인 Buffer Cache를 구현해봄으로써, kernel이 어떻게 메모리를 관리하고 접근하는지 이해하고 숙지한다.


프로그램을 실행하면 buffer를 몇 개 만들 것인지를 사용자에게 묻는다. 사용자는 만들 buffer의 개수를 입력한다.
그리고 hash queue를 구현하기 위해서 mod number를 입력하게 되는데,
각 header마다 가질 수 있는 buffer의 최대 개수가 10개인 것을 감안하여 입력한다. (buffer의 num은 중복되지 않게 저장됨)
(ex: buffer를 40개 만들었는데, 만약 mod number를 3을 입력한다면 header의 개수가 3개이다.
이 때 프로그램은 겹치지 않는 buffer를 40개 만들기위해서(불가능) 프로그램이 멈추지 않고 계속해서 돌아간다.
이러한 오류가 나지 않게끔 만들 buffer의 개수가 modnumber*10 미만이어야 한다.)

그리고 프로그램이 랜덤으로 만든 number에 따라, 각 hashqueue의 header에서 리스트를 이룬다.
hashqueue가 만들어지고, 콘솔창에 print 된다.
콘솔창을 확인하고 사용자는 찾고싶은 숫자를 입력한다.


<Scenario 1>
만약 해당 buffer가 hashqueue에 존재하고 busy 상태라면, 시나리오 1의 경우가 실행된다.
해당 buffer가 free해질 때까지 프로그램은 sleep하고, (time library를 이용하여 3초를 sleep 해주었다.)
continue를 통해 while문으로 돌아간다.

<Scenario 5>
만약 해당 buffer가 hashqueue에 존재하고 busy 상태가 아니라면, 시나리오 5의 경우가 실행된다.
buffer를 busy 상태라고 표시하고, (상태를 LOCKED로 변경) freelist에서 buffer를 지운다.
마지막으로 buffer를 return 한다.

<Scenario 4>
만약 해당 buffer가 hashqueue에 존재하지 않고 freelist의 길이가 0이라면, 시나리오 4의 경우가 실행된다.
원래 프로그램에서는 어떠한 buffer가 free 상태가 될 때까지 sleep하는데,
이 프로그램에서는 가능한 header의 number 중에 random number를 돌려서 해당 header 중 LOCKED 상태,
즉 busy인 임의의 buffer를 찾아 UNLOCKED로 바꿔주었다. 만약 busy 상태인 buffer가 없다면 다시 random number를 돌려주었다. 그리고 free 상태가 되었기 때문에, freelist에 추가해준다.
따라서 어떠한 buffer가 free 상태가 되었기 때문에, continue를 통해 while문으로 돌아간다.

<Scenario 3>
만약 해당 buffer가 hashqueue에 존재하지 않고 freelist의 첫 번째 buffer의 상태가 DELAYED라면, 시나리오 3의 경우가 실행된다.
우선 freelist의 첫 번째 buffer를 삭제한다. 그리고 hashqueue에서의 해당 buffer의 상태를 WRITE로 변경한다.
그리고 continue를 통해 while문으로 돌아간다.

<Scenario 2>
만약 해당 buffer가 hashqueue에 존재하지 않고 freelist의 첫 번째 buffer의 상태가 DELAYED가 아니라면, 시나리오 2의 경우가 실행된다.
우선 freelist의 첫 번째 buffer를 삭제한다. 그리고 그 buffer를 hashqueue에서도 삭제한다.
그리고 입력한 number의 buffer를 새롭게 hashqueue에 생성해준다.
마지막으로 buffer를 return 한다.