/*
Simulation of Contiguous Allocation Memory Management
�ۼ���: ������

paper�� ������ example�� ������� ���α׷��� ������

- MainMemory�� ����Ʈ�� linkedlist�� ����
- ��� ������ Best Fit�� �ǰ���
- Release Process�� ���� ���ӵǴ� hole block�� �ִٸ�, ū hole block���� ��������
*/

#include <iostream>
using namespace std;

#define MAX_SIZE 256

struct Process // process ����ü
{
	int pid; // process ID
	int size; // memory size
	Process* next; // linkedlist�� �����ϱ� ���� next��� ���� ���� ����Ű�� �����͸� ����

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
	MainMemory() // ������ �ʱ� ����: �ϳ��� ū ������ �����Ǿ� �ְ�, �� ���� hole
	{
		m_length = 1;
		m_freeSize = 256;

		m_list = new Process[m_length];
		m_list[0].pid = -1; m_list[0].size = 256;
		m_list[0].next = NULL;
		ResetList();
	}
	~MainMemory() {}

	int AddProcess(Process& pro); // process request�� �ް� ����Ʈ�� �߰��ϴ� �Լ�
	int ReleaseProcess(Process& pro); // process free request�� �ް� ����Ʈ���� �ش� process�� free�ϴ� �Լ�
	void Compaction(); // �������� ���ӵ� hole�� �ϳ��� ū hole�� �ٲٴ� �Լ�

	void Print(); // m_list�� ����� ������ ����ϴ� �Լ�

	void ResetList(); // m_curP�� m_list�� �� ó������ �������� �Լ�

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
	if (m_freeSize - pro.size >= 0) // ���� ���� size�� ���� process�� size���� ū���� Ȯ��
	{
		int min = 256;
		int minIndex = -1;

		ResetList(); // m_curP �ʱ�ȭ
		for (int i = 0; i < m_length; i++)
		{
			if (m_curP->pid == -1) // �� ��
			{
				if (m_curP->size >= pro.size && m_curP->size <= min)
				{
					min = m_curP->size; // ������ ���� ã�� ���� min�� minIndex�� update
					minIndex = i;
				}
			}
			m_curP = m_curP->next;
		}
		
		if (minIndex == -1) // ������ ���� ã�� ����
			return 0;

		ResetList();
		for (int i = 0; i < minIndex - 1; i++) // �߰��� �κ��� ���� ������ m_curP�� �̵�
		{
			m_curP = m_curP->next;
		}

		Process* newPr = &pro;
		if (minIndex == 0) // �� ���� ���
		{
			if (m_curP->size == pro.size) // size�� �� �´� ��� �� �ڸ��� ���μ����� ����
			{
				newPr->next = m_curP->next;
				m_list = newPr;
				// ���� �ִ� �ڸ��� ���μ����� �Ҵ��߱� ������ m_length�� �������� ����
			}
			else // �� ū ��� �� �ڸ��� ���μ����� �߰�, �� ������ ���� size ����
			{
				m_curP->size -= pro.size;
				newPr->next = m_curP;
				m_list = newPr;
				m_length++;
			}
			m_freeSize -= pro.size; // ���� size�� ����
			return 1;
		}
		else
		{
			if (m_curP->next->size == pro.size) // size�� �� �´� ��� �� �ڸ��� ���μ����� ����
			{
				newPr->next = m_curP->next->next;
				m_curP->next = newPr;
			}
			else // �� ū ��� �� �ڸ��� ���μ����� �߰�, �� ������ ���� size ����
			{
				m_curP->next->size -= pro.size;
				newPr->next = m_curP->next;
				m_curP->next = newPr; // �� �� ����
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
		if (m_curP->pid == pro.pid) // �ش� process�� ã��
		{
			m_curP->pid = -1; // hole block���� �ٲ���
			m_freeSize += m_curP->size; // ���� size ����

			Compaction(); // Compaction ����, �Ұ����ϸ� �ƹ� �ϵ� �Ͼ�� ����
			return 1;
		}
		m_curP = m_curP->next;
	}
	return 0; // �� ã�� ��� return 0
}

void MainMemory::Compaction()
{
	int count = 0;
	ResetList();
	for (int i = 0; i < m_length - 1; i++)
	{
		if (m_curP->pid == -1) // pid�� -1�̶�� �� ��
		{
			if (m_curP->next->pid == -1) // �������� �� ��
			{
				m_curP->size += m_curP->next->size; // size �߰�
				m_curP->next = m_curP->next->next; // ���� �� ����
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
		if (m_curP->pid != -1) // pid�� -1�̶�� �� ��
			cout << "p" << m_curP->pid << ": " << m_curP->size << "k" << endl;
		else // �� ����
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

	Process p1, p2, p3, p4, p5; // process�� ����� ����
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