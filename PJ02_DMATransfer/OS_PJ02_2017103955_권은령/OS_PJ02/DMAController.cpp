#include "IDECon.h"

void MakingTask(int task_num, task* tList);
void Allocation(int task_num, task* tList, IDECon* idecon);
int GetMax(int s_dq, int s_mq, int s_pq);

int main()
{
	srand(time(NULL));

	int task_num;
	cout << "Please enter the number of tasks. : ";
	cin >> task_num;

	task* taskList = new task[task_num];
	string conList[3] = { "Disk", "Mouse", "Printer" };

	IDECon ide_con;
	DMACon dma_controller;

	MakingTask(task_num, taskList); // 입력한 개수만큼 Task 생성
	Allocation(task_num, taskList, &ide_con); // Task 할당


	int dq_size = ide_con.GetDQ()->size();
	int mq_size = ide_con.GetMQ()->size();
	int pq_size = ide_con.GetPQ()->size();
	int max_size = GetMax(dq_size, mq_size, pq_size); // 각 Queue 중 maxsize 구하기

	thread* th_disk = new thread[dq_size];
	thread* th_mouse = new thread[mq_size];
	thread* th_printer = new thread[pq_size]; // 각 IDEController마다 thread 선언

	for (int c = 0; c < max_size; c++)
	{
		if (c < dq_size) // 현재 c가 dq의 size보다 작으면 DiskController에서 task 처리 실행
			th_disk[c] = thread([&] {ide_con.DiskProcessing(&dma_controller); });
		if (c < mq_size)// 현재 c가 mq의 size보다 작으면 MouseController에서 task 처리 실행
			th_mouse[c] = thread([&] {ide_con.MouseProcessing(&dma_controller); });
		if (c < pq_size)// 현재 c가 pq의 size보다 작으면 PrinterController에서 task 처리 실행
			th_printer[c] = thread([&] {ide_con.PrinterProcessing(&dma_controller); });
		else if (c == dq_size)
			cout << "Disk Processing is Finished.\n";
		else if (c == mq_size)
			cout << "Mouse Processing is Finished.\n";
		else if (c == pq_size)
			cout << "Printer Processing is Finished.\n";
	}

	// thread가 끝나기 전에 main 함수가 종료되는 것을 막아주기 위한 코드
	for (int d = 0; d < dq_size; d++)
	{
		th_disk[d].join();
	}
	for (int m = 0; m < mq_size; m++)
	{
		th_mouse[m].join();
		for (int p = 0; p < pq_size; p++)
		{
			th_printer[p].join();
		}

		dma_controller.PrintBuffer();
		// Memory Buffer에 있는 task들을 순차적으로 출력
		// Memory Buffer는 queue의 형태임

		system("pause");
		return 0;
	}
}


void MakingTask(int task_num, task* tList)
{
	srand(time(NULL));

	string con_list[3] = { "Disk", "Mouse", "Printer" }; // controller list를 생성
	int randnum;

	for (int i = 0; i < task_num; i++)
	{
		randnum = rand() % 3;
		tList[i].tid = i;
		tList[i].type = con_list[randnum]; // 랜덤으로 할당
	}
}

void Allocation(int task_num, task* tList, IDECon* idecon)
{
	for (int i = 0; i < task_num; i++)
	{
		if (tList[i].type == "Disk") // task의 type이 Disk
		{
			printf("[Allocation] ID: %d,   Type: Disk\n", tList[i].tid);
			idecon->GetDQ()->push(tList[i]); // idecon의 DQ에 push
		}
		else if (tList[i].type == "Mouse") // task의 type이 Mouse
		{
			printf("[Allocation] ID: %d,   Type: Mouse\n", tList[i].tid);
			idecon->GetMQ()->push(tList[i]);// idecon의 MQ에 push
		}
		else if (tList[i].type == "Printer") // task의 type이 Printer
		{
			printf("[Allocation] ID: %d,   Type: Printer\n", tList[i].tid);
			idecon->GetPQ()->push(tList[i]);// idecon의 PQ에 push
		}
		else
		{
			printf("No Allocation!");
		}
	}
	cout << "Allocation is finished." << endl << endl;
}

int GetMax(int s_dq, int s_mq, int s_pq)
{
	if (s_dq >= s_mq)
	{
		if (s_dq >= s_pq)
			return s_dq;
		else
			return s_pq;
	}
	else
	{
		if (s_mq >= s_pq)
			return s_mq;
		else
			return s_pq;
	}
}