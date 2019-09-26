OS Project 02 DMA Transfer 구현
 DMA Transfer를 직접 구현해봄으로써 Multi-Thread를 알아보고, DMA Transfer가 어떻게 이루어지는 지 알아본다.

프로그램이 실행되면 우선 사용자에게 몇 개의 task를 생성할 것인지를 입력받는다.
입력받은 숫자만큼 MakingTask(int task_num, task* tList) 를 통해 task를 생성한다. task_num은 생성할 task의 수, tList는 생성한 task를 임시 저장할 list이다.
task는 int type의 tid, string type의 type 변수를 가지고 있는 구조체이다.

그러면 conList라는 string type의 길이가 3인 list를 생성하는데, 이는 { "Disk", "Mouse", "Printer" }로 구성된다.
그리고 Allocation(int task_num, task* tList, IDECon* idecon)을 통해 각각 task의 type에 따라 각각 IDEController(Disk Controller, Mouse Controller, Printer Controller)에 할당한다.

할당이 끝났다면 이제 Multi-Thread 형식으로 task 처리를 실행한다.
더 이상 처리할 task가 없다면 Processing이 끝났다고 출력해준다.

각 task 처리 과정은 다음과 같다.
우선 각 queue는 task type이다.
해당 queue의 첫 번째 task를 pop하고(처리가 끝남), DMA Controller에게 처리가 끝났음을 알리면서 pop한 task를 memory buffer에 추가한다.
이 때, c++ <queue>의 pop 함수는 리턴값이 없으므로, pop하기 전에 임시로 queue의 첫 번째 값을 저장해야한다.

thread 실행이 끝나기 전에 main 함수가 종료될 수 있기 때문에, 각 thread를 join()을 통해 끝날 때까지 기다린다.
모든 thread가 실행이 끝났다면, dma_controller의 PrintBuffer() 함수를 호출해 Memory Buffer에 저장된 task들을 순차적으로 출력하고, 프로그램을 종료한다.