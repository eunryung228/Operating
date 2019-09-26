#include "DMACon.h"

void DMACon::SaveTask(task& t)
{
	mBuffer.push(t); // memory buffer에 IDEController가 처리를 끝낸 task 저장
}

void DMACon::PrintBuffer() // memory buffer에 저장된 task들을 순차적으로 출력
{
	task temp;
	int bf_size = mBuffer.size(); // memory buffer size 저장
	cout << "\n[Memory Buffer]" << endl;

	queue<task> tempQueue = mBuffer;
	for (int i = 0; i < bf_size; i++)
	{
		temp = tempQueue.front(); // memory buffer queue의 맨 첫 번째 값 임시 저장
		tempQueue.pop(); // queue의 첫 번째 값 pop, queue는 FIFO 형식
		cout << "ID : " << temp.tid << ",   Type : " << temp.type << endl; // pop한 task의 id와 type을 출력
	}
}