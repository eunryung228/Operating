/*
Simulation of Contiguous Allocation Memory Management
작성자: 권은령

paper에 나오는 example을 기반으로 프로그램을 구현함

- MainMemory의 리스트를 linkedlist로 구현
- 모든 과정은 Best Fit에 의거함
- Release Process시 만약 연속되는 hole block이 있다면, 큰 hole block으로 통합해줌
*/

#include <iostream>
using namespace std;

#define MAX_SIZE 256

struct Process // process 구조체
{
	int pid; // process ID
	int size; // memory size
	Process* next; // linkedlist로 구성하기 위해 next라는 다음 블럭을 가리키는 포인터를 넣음

	void SetProcess(int id, int sz)
	{
		pid = id;
		size = sz;
		next = NULL;
	}
};

class MainMemory
{
public:
	MainMemory() // 생성자 초기 설정: 하나의 큰 블럭으로 구성되어 있고, 그 블럭은 hole
	{
		m_length = 1;
		m_freeSize = 256;

		m_list = new Process[m_length];
		m_list[0].pid = -1; m_list[0].size = 256;
		m_list[0].next = NULL;
		ResetList();
	}
	~MainMemory() {}

	int AddProcess(Process& pro); // process request를 받고 리스트에 추가하는 함수
	int ReleaseProcess(Process& pro); // process free request를 받고 리스트에서 해당 process를 free하는 함수
	void Compaction(); // 나누어진 연속된 hole을 하나의 큰 hole로 바꾸는 함수

	void Print(); // m_list에 저장된 내용을 출력하는 함수

	void ResetList(); // m_curP를 m_list의 맨 처음으로 돌려놓는 함수

private:
	Process* m_list;
	Process* m_curP;
	int m_freeSize;
	int m_length;
};

void MainMemory::ResetList()
{
	m_curP = &m_list[0];
}

int MainMemory::AddProcess(Process& pro)
{
	if (m_freeSize - pro.size >= 0) // 현재 가용 size가 들어올 process의 size보다 큰지를 확인
	{
		int min = 256;
		int minIndex = -1;

		ResetList(); // m_curP 초기화
		for (int i = 0; i < m_length; i++)
		{
			if (m_curP->pid == -1) // 빈 블럭
			{
				if (m_curP->size >= pro.size && m_curP->size <= min)
				{
					min = m_curP->size; // 최적의 블럭을 찾기 위해 min과 minIndex를 update
					minIndex = i;
				}
			}
			m_curP = m_curP->next;
		}
		
		if (minIndex == -1) // 가능한 블럭을 찾지 못함
			return 0;

		ResetList();
		for (int i = 0; i < minIndex - 1; i++) // 추가할 부분의 앞의 블럭으로 m_curP를 이동
		{
			m_curP = m_curP->next;
		}

		Process* newPr = &pro;
		if (minIndex == 0) // 맨 앞인 경우
		{
			if (m_curP->size == pro.size) // size가 딱 맞는 경우 그 자리에 프로세스를 넣음
			{
				newPr->next = m_curP->next;
				m_list = newPr;
				// 원래 있던 자리에 프로세스를 할당했기 때문에 m_length가 증가하지 않음
			}
			else // 더 큰 경우 그 자리에 프로세스를 추가, 빈 공간에 대해 size 감소
			{
				m_curP->size -= pro.size;
				newPr->next = m_curP;
				m_list = newPr;
				m_length++;
			}
			m_freeSize -= pro.size; // 가용 size를 줄임
			return 1;
		}
		else
		{
			if (m_curP->next->size == pro.size) // size가 딱 맞는 경우 그 자리에 프로세스를 넣음
			{
				newPr->next = m_curP->next->next;
				m_curP->next = newPr;
			}
			else // 더 큰 경우 그 자리에 프로세스를 추가, 빈 공간에 대해 size 감소
			{
				m_curP->next->size -= pro.size;
				newPr->next = m_curP->next;
				m_curP->next = newPr; // 앞 뒤 연결
				m_length++;
			}
			m_freeSize -= pro.size;
			return 1;
		}
	}
	else
		return 0;
}

int MainMemory::ReleaseProcess(Process& pro)
{
	ResetList();
	for (int i = 0; i < m_length; i++)
	{
		if (m_curP->pid == pro.pid) // 해당 process를 찾음
		{
			m_curP->pid = -1; // hole block으로 바꿔줌
			m_freeSize += m_curP->size; // 가용 size 증가

			Compaction(); // Compaction 실행, 불가능하면 아무 일도 일어나지 않음
			return 1;
		}
		m_curP = m_curP->next;
	}
	return 0; // 못 찾은 경우 return 0
}

void MainMemory::Compaction()
{
	int count = 0;
	ResetList();
	for (int i = 0; i < m_length - 1; i++)
	{
		if (m_curP->pid == -1) // pid가 -1이라면 빈 블럭
		{
			if (m_curP->next->pid == -1) // 연속으로 빈 블럭
			{
				m_curP->size += m_curP->next->size; // size 추가
				m_curP->next = m_curP->next->next; // 다음 블럭 삭제
				m_length -= 1;
			}
		}
		m_curP = m_curP->next;
	}
}

void MainMemory::Print()
{
	int s = 0;
	ResetList();
	for (int i = 0; i < m_length; i++)
	{
		if (m_curP->pid != -1) // pid가 -1이라면 빈 블럭
			cout << "p" << m_curP->pid << ": " << m_curP->size << "k" << endl;
		else // 빈 공간
		{
			cout << "hole: " << m_curP->size << "k" << endl;
			s++;
		}
		m_curP = m_curP->next;
	}
	cout << m_freeSize << "K free, " << s << " block(s), average size = " << m_freeSize / s << "K" << endl;
	cout << endl;
}


int main()
{
	MainMemory mm;

	cout << "### Simulation of Contiguous Allocation Memory Management ###" << endl;
	cout << "Memory Size: 256k" << endl << endl;

	Process p1, p2, p3, p4, p5; // process를 만드는 과정
	p1.SetProcess(1, 64);
	p2.SetProcess(2, 64);
	p3.SetProcess(3, 32);
	p4.SetProcess(4, 16);
	p5.SetProcess(5, 32);

	cout << "REQUEST 1: 64K" << endl;
	mm.AddProcess(p1);
	mm.Print();

	cout << "REQUEST 2: 64K" << endl;
	mm.AddProcess(p2);
	mm.Print();

	cout << "REQUEST 3: 32K" << endl;
	mm.AddProcess(p3);
	mm.Print();

	cout << "REQUEST 4: 16K" << endl;
	mm.AddProcess(p4);
	mm.Print();

	cout << "FREE REQUEST 1 (64K)" << endl;
	mm.ReleaseProcess(p1);
	mm.Print();

	cout << "FREE REQUEST 3 (32K)" << endl;
	mm.ReleaseProcess(p3);
	mm.Print();

	cout << "REQUEST 5: 32K" << endl;
	mm.AddProcess(p5);
	mm.Print();

	cout << "FREE REQUEST 2 (64K)" << endl;
	mm.ReleaseProcess(p2);
	mm.Print();

	system("pause");
	return 0;
}