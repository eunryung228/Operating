#include <iostream>
#include <ctime>
#include <string>
#include <Windows.h>
using namespace std;

#define MAX_BLKNUMS 10 // �� hashtable�� �Ҵ� �ִ� ����

// �� buffer�� ����ü
struct Node
{
	int key; // num���ٰ� hash function�� ������ ��
	int num;
	string state; // ���� ����
	Node* next;
};

// HashQueue class
class HashQueue
{
public:
	HashQueue(int num)
	{
		modnum = num;
		table = new Node*[modnum]; // ����ڰ� �Է��� modnum��ŭ ����� ����
		for (int i = 0; i < modnum; i++)
		{
			table[i] = new Node;
			table[i]->key = i;
			table[i]->next = NULL;
		}
		freelist = new Node;
		freelist->next = NULL;
	}

	int hash(int blknum) // hash function : ����ڰ� �Է��� modnum���� ���� �������� ��ȯ
	{
		return blknum % modnum;
	}

	void SetState(Node*& node) // �������� state�� ����
	{
		int i = rand() % 2; // 0, 1 �� ����
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

	void add(int blkey, int blnum); // hashqueue�� block�� �߰�
	void SetFreelist(int blkey, int blnum, string state);

	int getblkLength(int key);
	bool getBlock(int blkey, int blnum);
	bool FindBlock(int blkey, int blnum, Node*& preloc);
	void print(); // hash ��� �Լ�

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
	Node* node = new Node; // ������ ��� ����
	node->key = blkey;
	node->num = blnum;
	node->next = NULL;

	int isfree, isdelay;
	isfree = rand() % 100; // 0~99���� �������� ���� ����
	if (isfree >= 90) // 10%�� Ȯ���� freelist�� �������
	{
		isdelay = rand() % 30; // delay�� 1/3 Ȯ����
		if (isdelay >= 20)
			node->state = "DELAYED";
		else
			SetState(node); // �������� state ����
		SetFreelist(blkey, blnum, node->state);
	}
	else
		SetState(node);
	
	Node* location = new Node;
	if (table[blkey]->next==NULL) // �ش� table�� ����ٸ� �ٷ� ��� �ڿ� ����
	{
		table[blkey]->next = node;
	}
	else // �ƴ϶��
	{
		location = table[blkey];
		while (location->next != NULL) // ������ buffer�� ������ ������ next
		{
			location = location->next;
		}
		location->next = node;
	}
}

void HashQueue::SetFreelist(int blkey, int blnum, string state) // freelist���� ��带 �����ϴ� �Լ�
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

int HashQueue::getblkLength(int key) // �ش� key list�� length�� ��ȯ
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

bool HashQueue::getBlock(int blkey, int blnum) // �ش� buffer�� ��ȯ�ϴ� �Լ�. ��ȯ�� �����ϴٸ� 1�� ����.
{
	while (true)
	{
		Node* node = table[blkey];
		while (node != NULL)
		{
			if (node->num == blnum) // block�� hash queue�� ������ ��
			{
				if (node->state == "LOCKED") // busy�� ���: scenario 5
				{
					cout << "\n<Scenario 5>" << endl;
					int seconds = 0;
					while (true)
					{
						Sleep(1000); // 1�� ���
						seconds++;
						cout << "��� �ð�(Seconds) : " << seconds << endl;
						if (seconds == 3)
						{
							Node* pre;
							if (!isInFreelist(blnum, pre)) // freelist�� ���ٸ�
								SetFreelist(blkey, blnum, "UNLOCKED"); // �߰�
							break;
						}
					}
					node->state = "UNLOCKED"; // ���¸� UNLOCKED�� �ٲ���

					Node* pre;
					if (isInFreelist(blnum, pre)) // freelist�� �����ϴ°� Ȯ��
						pre->next->state = "UNLOCKED";

					cout << "Buffer " << blnum << " State Changed : LOCKED -> UNLOCKED" << endl << endl;
					continue; // while������ ���ư�
				}
				else // �ó����� 1
				{
					node->state = "LOCKED"; // ���¸� busy�� �ٲ�
					Node* pre = new Node; // ã�� buffer�� ���� node�� ����;���
					if (isInFreelist(blnum, pre)) // ã�� buffer�� freelist�� �����ϴ°� Ȯ��
					{
						if (pre->next->next == NULL) // ������� buffer�� freelist�� �������� �����Ѵٸ�
							pre->next = NULL; // �ش� buffer�� ���� buffer�� next�� NULL�� �������ν� ����
						else
						{
							Node* location = pre->next; // ���� buffer�� location
							pre->next = location->next; // buffer ���� node�� ���� node�� ���������ν� �ش� buffer�� ������
							delete location;
						}
					}
					cout << "\n<Scenario 1>, Return Buffer " << blnum << " Success" << endl << endl;
					return 1;
				}
			}
			node = node->next;
		}
		// buffer�� hash queue�� �������� ���� ��
		if (getFreelistLength() == 0) // freelist�� ���̰� 0�̶��
		{
			cout << "\n<Scenario 4> Sleep until any buffer becomes free..." << endl; // �ó����� 4
			
			int rmod;
			bool find = false;
			while (!find)
			{
				rmod = rand() % modnum; // 0~modnum-1 ���� �������� ����
				Node* rnode = table[rmod]->next;
				for (int i = 0; i < getblkLength(rmod); i++)
				{
					if (rnode->state == "LOCKED") // busy�� random buffer��
					{
						rnode->state = "UNLOCKED"; // free�ϰ�..
						SetFreelist(rnode->key, rnode->num, rnode->state); // freelist�� �߰�
						find = true;
						break;
					}
					rnode = rnode->next;
				}
			}
			continue;
		}

		Node* getnode = freelist->next; // freelist�� ù ��° buffer
		Node* pre_find;
		int findkey = getnode->key; // freelist�� ù ��° buffer�� key
		int findnum = getnode->num; // freelist�� ù ��° buffer�� num
		freelist->next = getnode->next; // freelist�� ù ��° buffer ����

		if (getnode->state == "DELAYED") // freelist�� ù ��° buffer�� delay ���¶��, �ó����� 3
		{
			if (FindBlock(findkey, findnum, pre_find)) // hashqueue���� �� buffer�� �ٷ� �� block�� ã��
				pre_find->next->state = "WRITE"; // ���¸� write�� ���� // 
			cout << "\n<Scenario 3>, Delete Buffer " << findnum << " from freelist." << endl;
			cout << "And Buffer " << findnum << " State Changed : DELAYED -> WRITE" << endl << endl;
			continue;
		}

		// �ó����� 2
		cout << "\n<Scenario 2>, Delete Buffer " << findnum << " from freelist." << endl;

		if (FindBlock(findkey, findnum, pre_find)) // �ش� num�� buffer�� ã����
			pre_find->next = pre_find->next->next; // �ش� buffer�� ���� �� �ڸ� ����Ű�� �ؼ� �ش� buffer�� ����
		cout << "Delete Buffer " << findnum << " from HashQueue." << endl;

		Node* newnode = new Node; Node* loc = table[blkey];
		newnode->key = blkey;
		newnode->num = blnum;
		newnode->state = "LOCKED";
		newnode->next = NULL;
		while (loc->next != NULL) // ������ loc�� �̵�
			loc = loc->next;
		loc->next = newnode; // newnode�� ����
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
		if (location->num == blnum) // ���࿡ block�� �����ϸ�
		{
			preloc = pre; // ã�� ��ġ �տ� �ִ� node�� �Ѱ���
			return true; // true�� ����
		}
		pre = location;
		location = location->next;
	}
	return false;
}

void HashQueue::print() // hashqueue ��ü ���
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

bool HashQueue::isPossible(int blnum) // hashtable �ȿ� �����ϴ°��� Ȯ��
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

bool HashQueue:: isInFreelist(int blnum, Node*& preloc) // Freelist ���� �����ϴ°��� Ȯ��
{
	Node* pre = freelist;
	Node* location = freelist->next;
	while (location != NULL)
	{
		if (location->num == blnum) // ���࿡ freelist�� �����ϸ�
		{
			preloc = pre; // ã�� ��ġ �տ� �ִ� node�� �Ѱ���
			return true; // true�� ����
		}
		pre = location;
		location = location->next;
	}
	return false;
}

int HashQueue:: getFreelistLength() // Freelist�� length�� ��ȯ
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
		randnum = rand() % 99 + 1; // 1~99���� �������� ���� ����
		key = hq.hash(randnum);
		cout << randnum << ", ";
		if (hq.getblkLength(key) == MAX_BLKNUMS || hq.isPossible(randnum)==false) // �� block�� length �ִ� ���̸� �Ѿ�ų�, �̹� buffer �ȿ� ������� ���� �����ٸ�
			i--; // buffer�� add���� �ʰ�, �ٽ� randnum�� ����� ������ �ݿ��ϱ� ���ؼ� i--�� ����
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
			cout << "\n���α׷��� �����մϴ�." << endl;
			break;
		}
	}

	system("pause");
	return 0;
}