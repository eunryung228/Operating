#include "IDECon.h"

void IDECon::DiskProcessing(DMACon* dma) // Disk Controller�� task ó��
{
	if (!dq.empty())
	{
		task temp = dq.front(); // queue�� ���� ù ��° ���� pop�ϱ� ���� �ӽ� ����
		dq.pop(); // queue�� ���� ù ��° �� pop, queue�� FIFO ����.
		dma->SaveTask(temp); // DMAController���� �˷��� memory buffer���ٰ� pop�� task�� ����
		printf("[Disk Processing] ID: %d\n", temp.tid); // processing�� �Ϸ�Ǿ����� �˸�
	}
}

void IDECon::MouseProcessing(DMACon* dma) // Mouse Controller�� task ó��, Disk Controller�� ����� �Ȱ���
{
	if (!mq.empty())
	{
		task temp = mq.front();
		mq.pop();
		dma->SaveTask(temp);
		printf("[Mouse Processing] ID: %d\n", temp.tid);
	}
}

void IDECon::PrinterProcessing(DMACon* dma) // Printer Controller�� task ó��
{
	if (!pq.empty())
	{
		task temp = pq.front(); // queue�� ���� ù ��° ���� pop�ϱ� ���� �ӽ� ����
		pq.pop();
		dma->SaveTask(temp);
		printf("[Printer Processing] ID: %d\n", temp.tid);
	}
}