#include "DMACon.h"

void DMACon::SaveTask(task& t)
{
	mBuffer.push(t); // memory buffer�� IDEController�� ó���� ���� task ����
}

void DMACon::PrintBuffer() // memory buffer�� ����� task���� ���������� ���
{
	task temp;
	int bf_size = mBuffer.size(); // memory buffer size ����
	cout << "\n[Memory Buffer]" << endl;

	queue<task> tempQueue = mBuffer;
	for (int i = 0; i < bf_size; i++)
	{
		temp = tempQueue.front(); // memory buffer queue�� �� ù ��° �� �ӽ� ����
		tempQueue.pop(); // queue�� ù ��° �� pop, queue�� FIFO ����
		cout << "ID : " << temp.tid << ",   Type : " << temp.type << endl; // pop�� task�� id�� type�� ���
	}
}