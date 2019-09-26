#include "IDECon.h"

void IDECon::DiskProcessing(DMACon* dma) // Disk Controller의 task 처리
{
	if (!dq.empty())
	{
		task temp = dq.front(); // queue의 가장 첫 번째 값을 pop하기 전에 임시 저장
		dq.pop(); // queue의 가장 첫 번째 값 pop, queue는 FIFO 형식.
		dma->SaveTask(temp); // DMAController에게 알려서 memory buffer에다가 pop한 task를 저장
		printf("[Disk Processing] ID: %d\n", temp.tid); // processing이 완료되었음을 알림
	}
}

void IDECon::MouseProcessing(DMACon* dma) // Mouse Controller의 task 처리, Disk Controller와 방식은 똑같음
{
	if (!mq.empty())
	{
		task temp = mq.front();
		mq.pop();
		dma->SaveTask(temp);
		printf("[Mouse Processing] ID: %d\n", temp.tid);
	}
}

void IDECon::PrinterProcessing(DMACon* dma) // Printer Controller의 task 처리
{
	if (!pq.empty())
	{
		task temp = pq.front(); // queue의 가장 첫 번째 값을 pop하기 전에 임시 저장
		pq.pop();
		dma->SaveTask(temp);
		printf("[Printer Processing] ID: %d\n", temp.tid);
	}
}