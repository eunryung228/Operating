#include "DMACon.h"

class IDECon
{
public:
	IDECon() {}

	void DiskProcessing(DMACon* dma);
	void MouseProcessing(DMACon* dma);
	void PrinterProcessing(DMACon* dma);

	queue<task>* GetDQ() { return &dq; }
	queue<task>* GetMQ() { return &mq; }
	queue<task>* GetPQ() { return &pq; }

private:
	queue<task> dq; // disk controller¿« queue
	queue<task> mq; // mouse controller¿« queue
	queue<task> pq; // printer controller¿« queue
};