#include <iostream>
#include <ctime>
#include <string>
#include <Windows.h>
using namespace std;

#define MAX_BLKNUMS 10 // 각 hashtable의 할당 최대 개수

// 각 buffer의 구조체
struct Node
{
	int key; // num에다가 hash function을 적용한 값
	int num;
	string state; // 현재 상태
	Node* next;
};

// HashQueue class
class HashQueue
{
public:
	HashQueue(int num)
	{
		modnum = num;
		table = new Node*[modnum]; // 사용자가 입력한 modnum만큼 헤더를 생성
		for (int i = 0; i < modnum; i++)
		{
			table[i] = new Node;
			table[i]->key = i;
			table[i]->next = NULL;
		}
		freelist = new Node;
		freelist->next = NULL;
	}

	int hash(int blknum) // hash function : 사용자가 입력한 modnum으로 나눈 나머지를 반환
	{
		return blknum % modnum;
	}

	void SetState(Node*& node) // 랜덤으로 state를 배정
	{
		int i = rand() % 2; // 0, 1 중 랜덤
		switch (i)
		{
		case 0:
			node->state = "LOCKED";
			break;
		case 1:
			node->state = "UNLOCKED";
			break;
		default:
			break;
		}
	}

	void add(int blkey, int blnum); // hashqueue에 block을 추가
	void SetFreelist(int blkey, int blnum, string state);

	int getblkLength(int key);
	bool getBlock(int blkey, int blnum);
	bool FindBlock(int blkey, int blnum, Node*& preloc);
	void print(); // hash 출력 함수

	bool isPossible(int blnum);
	bool isInFreelist(int blnum, Node*& preloc);
	int getFreelistLength();

private:
	int modnum;
	Node** table;
	Node* freelist;
};

void HashQueue:: add(int blkey, int blnum)
{
	Node* node = new Node; // 삽입할 노드 생성
	node->key = blkey;
	node->num = blnum;
	node->next = NULL;

	int isfree, isdelay;
	isfree = rand() % 100; // 0~99까지 랜덤으로 숫자 생성
	if (isfree >= 90) // 10%의 확률로 freelist에 집어넣음
	{
		isdelay = rand() % 30; // delay는 1/3 확률로
		if (isdelay >= 20)
			node->state = "DELAYED";
		else
			SetState(node); // 랜덤으로 state 설정
		SetFreelist(blkey, blnum, node->state);
	}
	else
		SetState(node);
	
	Node* location = new Node;
	if (table[blkey]->next==NULL) // 해당 table이 비었다면 바로 헤더 뒤에 삽입
	{
		table[blkey]->next = node;
	}
	else // 아니라면
	{
		location = table[blkey];
		while (location->next != NULL) // 마지막 buffer에 도달할 때까지 next
		{
			location = location->next;
		}
		location->next = node;
	}
}

void HashQueue::SetFreelist(int blkey, int blnum, string state) // freelist에다 노드를 삽입하는 함수
{
	Node* node = new Node;
	node->key = blkey;
	node->num = blnum;
	node->state = state;
	node->next = NULL;

	Node* location = new Node;

	if (freelist->next == NULL)
		freelist->next = node;
	else
	{
		location = freelist;
		while (location->next != NULL)
		{
			location = location->next;
		}
		location->next = node;
	}
}

int HashQueue::getblkLength(int key) // 해당 key list의 length를 반환
{
	int length = 0;
	Node* location = table[key];
	while (location->next != NULL)
	{
		length += 1;
		location = location->next;
	}
	return length;
}

bool HashQueue::getBlock(int blkey, int blnum) // 해당 buffer를 반환하는 함수. 반환이 가능하다면 1을 리턴.
{
	while (true)
	{
		Node* node = table[blkey];
		while (node != NULL)
		{
			if (node->num == blnum) // block이 hash queue에 존재할 때
			{
				if (node->state == "LOCKED") // busy인 경우: scenario 5
				{
					cout << "\n<Scenario 5>" << endl;
					int seconds = 0;
					while (true)
					{
						Sleep(1000); // 1초 대기
						seconds++;
						cout << "경과 시간(Seconds) : " << seconds << endl;
						if (seconds == 3)
						{
							Node* pre;
							if (!isInFreelist(blnum, pre)) // freelist에 없다면
								SetFreelist(blkey, blnum, "UNLOCKED"); // 추가
							break;
						}
					}
					node->state = "UNLOCKED"; // 상태를 UNLOCKED로 바꿔줌

					Node* pre;
					if (isInFreelist(blnum, pre)) // freelist에 존재하는가 확인
						pre->next->state = "UNLOCKED";

					cout << "Buffer " << blnum << " State Changed : LOCKED -> UNLOCKED" << endl << endl;
					continue; // while문으로 돌아감
				}
				else // 시나리오 1
				{
					node->state = "LOCKED"; // 상태를 busy로 바꿈
					Node* pre = new Node; // 찾을 buffer의 앞의 node를 갖고와야함
					if (isInFreelist(blnum, pre)) // 찾을 buffer가 freelist에 존재하는가 확인
					{
						if (pre->next->next == NULL) // 지우려는 buffer가 freelist의 마지막에 존재한다면
							pre->next = NULL; // 해당 buffer의 앞의 buffer의 next를 NULL로 만듦으로써 삭제
						else
						{
							Node* location = pre->next; // 지울 buffer가 location
							pre->next = location->next; // buffer 앞의 node와 뒤의 node를 연결함으로써 해당 buffer를 지워줌
							delete location;
						}
					}
					cout << "\n<Scenario 1>, Return Buffer " << blnum << " Success" << endl << endl;
					return 1;
				}
			}
			node = node->next;
		}
		// buffer가 hash queue에 존재하지 않을 때
		if (getFreelistLength() == 0) // freelist의 길이가 0이라면
		{
			cout << "\n<Scenario 4> Sleep until any buffer becomes free..." << endl; // 시나리오 4
			
			int rmod;
			bool find = false;
			while (!find)
			{
				rmod = rand() % modnum; // 0~modnum-1 까지 랜덤으로 선정
				Node* rnode = table[rmod]->next;
				for (int i = 0; i < getblkLength(rmod); i++)
				{
					if (rnode->state == "LOCKED") // busy인 random buffer를
					{
						rnode->state = "UNLOCKED"; // free하게..
						SetFreelist(rnode->key, rnode->num, rnode->state); // freelist에 추가
						find = true;
						break;
					}
					rnode = rnode->next;
				}
			}
			continue;
		}

		Node* getnode = freelist->next; // freelist의 첫 번째 buffer
		Node* pre_find;
		int findkey = getnode->key; // freelist의 첫 번째 buffer의 key
		int findnum = getnode->num; // freelist의 첫 번째 buffer의 num
		freelist->next = getnode->next; // freelist의 첫 번째 buffer 삭제

		if (getnode->state == "DELAYED") // freelist의 첫 번째 buffer가 delay 상태라면, 시나리오 3
		{
			if (FindBlock(findkey, findnum, pre_find)) // hashqueue에서 그 buffer의 바로 전 block을 찾음
				pre_find->next->state = "WRITE"; // 상태를 write로 변경 // 
			cout << "\n<Scenario 3>, Delete Buffer " << findnum << " from freelist." << endl;
			cout << "And Buffer " << findnum << " State Changed : DELAYED -> WRITE" << endl << endl;
			continue;
		}

		// 시나리오 2
		cout << "\n<Scenario 2>, Delete Buffer " << findnum << " from freelist." << endl;

		if (FindBlock(findkey, findnum, pre_find)) // 해당 num의 buffer를 찾으면
			pre_find->next = pre_find->next->next; // 해당 buffer의 앞이 그 뒤를 가리키게 해서 해당 buffer를 삭제
		cout << "Delete Buffer " << findnum << " from HashQueue." << endl;

		Node* newnode = new Node; Node* loc = table[blkey];
		newnode->key = blkey;
		newnode->num = blnum;
		newnode->state = "LOCKED";
		newnode->next = NULL;
		while (loc->next != NULL) // 끝까지 loc을 이동
			loc = loc->next;
		loc->next = newnode; // newnode를 삽입
		cout << "\n<Scenario 2>, Return Buffer " << blnum << " success" << endl << endl;
		return 1;
	}
}

bool HashQueue:: FindBlock(int blkey, int blnum, Node*& preloc)
{
	Node* pre = table[blkey];
	Node* location = table[blkey]->next;
	while (location!= NULL)
	{
		if (location->num == blnum) // 만약에 block에 존재하면
		{
			preloc = pre; // 찾는 위치 앞에 있는 node를 넘겨줌
			return true; // true를 리턴
		}
		pre = location;
		location = location->next;
	}
	return false;
}

void HashQueue::print() // hashqueue 전체 출력
{
	for (int i = 0; i < modnum; i++)
	{
		cout << "mod num " << i << " : ";
		Node* location = table[i]->next;
		while (location != NULL)
		{
			cout << location->num << "(" << location->state << ")" << "  ";
			location = location->next;
		}
		cout << endl;
	}

	cout << "freelist : ";
	Node* location = freelist->next;
	while (location != NULL)
	{
		cout << location->num << "(" << location->state << ")" << "  ";
		location = location->next;
	}
}

bool HashQueue::isPossible(int blnum) // hashtable 안에 존재하는가를 확인
{
	for (int i = 0; i < modnum; i++)
	{
		Node* location = table[i]->next;
		while (location != NULL)
		{
			if (location->num == blnum)
				return false;
			location = location->next;
		}
	}
	return true;
}

bool HashQueue:: isInFreelist(int blnum, Node*& preloc) // Freelist 내에 존재하는가를 확인
{
	Node* pre = freelist;
	Node* location = freelist->next;
	while (location != NULL)
	{
		if (location->num == blnum) // 만약에 freelist에 존재하면
		{
			preloc = pre; // 찾는 위치 앞에 있는 node를 넘겨줌
			return true; // true를 리턴
		}
		pre = location;
		location = location->next;
	}
	return false;
}

int HashQueue:: getFreelistLength() // Freelist의 length를 반환
{
	int len = 0;
	Node* location = freelist;
	while (location->next != NULL)
	{
		len++;
		location = location->next;
	}
	return len;
}


int main()
{
	srand(time(NULL));

	int allnum;
	cout << "Please enter the buffer number that you want to make : ";
	cin >> allnum;
	int modnum;
	cout << "Please enter the Mod number : ";
	cin >> modnum;

	HashQueue hq(modnum);
	int randnum, key;
	for (int i = 0; i < allnum; i++)
	{
		randnum = rand() % 99 + 1; // 1~99까지 랜덤으로 숫자 생성
		key = hq.hash(randnum);
		cout << randnum << ", ";
		if (hq.getblkLength(key) == MAX_BLKNUMS || hq.isPossible(randnum)==false) // 각 block의 length 최대 길이를 넘어서거나, 이미 buffer 안에 만들어진 값을 가졌다면
			i--; // buffer를 add하지 않고, 다시 randnum을 만드는 과정을 반영하기 위해서 i--를 해줌
		else
			hq.add(key, randnum);
	}
	cout << endl << endl;

	int blknum;
	while (true)
	{
		hq.print();

		cout << "\n\nNumber of Buffer to GetBlock : ";
		cin >> blknum;

		int blkey = hq.hash(blknum);
		if (!hq.getBlock(blkey, blknum))
		{
			cout << "\n프로그램을 종료합니다." << endl;
			break;
		}
	}

	system("pause");
	return 0;
}