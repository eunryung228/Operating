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

	MakingTask(task_num, taskList); // �Է��� ������ŭ Task ����
	Allocation(task_num, taskList, &ide_con); // Task �Ҵ�


	int dq_size = ide_con.GetDQ()->size();
	int mq_size = ide_con.GetMQ()->size();
	int pq_size = ide_con.GetPQ()->size();
	int max_size = GetMax(dq_size, mq_size, pq_size); // �� Queue �� maxsize ���ϱ�

	thread* th_disk = new thread[dq_size];
	thread* th_mouse = new thread[mq_size];
	thread* th_printer = new thread[pq_size]; // �� IDEController���� thread ����

	for (int c = 0; c < max_size; c++)
	{
		if (c < dq_size) // ���� c�� dq�� size���� ������ DiskController���� task ó�� ����
			th_disk[c] = thread([&] {ide_con.DiskProcessing(&dma_controller); });
		if (c < mq_size)// ���� c�� mq�� size���� ������ MouseController���� task ó�� ����
			th_mouse[c] = thread([&] {ide_con.MouseProcessing(&dma_controller); });
		if (c < pq_size)// ���� c�� pq�� size���� ������ PrinterController���� task ó�� ����
			th_printer[c] = thread([&] {ide_con.PrinterProcessing(&dma_controller); });
		else if (c == dq_size)
			cout << "Disk Processing is Finished.\n";
		else if (c == mq_size)
			cout << "Mouse Processing is Finished.\n";
		else if (c == pq_size)
			cout << "Printer Processing is Finished.\n";
	}

	// thread�� ������ ���� main �Լ��� ����Ǵ� ���� �����ֱ� ���� �ڵ�
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
		// Memory Buffer�� �ִ� task���� ���������� ���
		// Memory Buffer�� queue�� ������

		system("pause");
		return 0;
	}
}


void MakingTask(int task_num, task* tList)
{
	srand(time(NULL));

	string con_list[3] = { "Disk", "Mouse", "Printer" }; // controller list�� ����
	int randnum;

	for (int i = 0; i < task_num; i++)
	{
		randnum = rand() % 3;
		tList[i].tid = i;
		tList[i].type = con_list[randnum]; // �������� �Ҵ�
	}
}

void Allocation(int task_num, task* tList, IDECon* idecon)
{
	for (int i = 0; i < task_num; i++)
	{
		if (tList[i].type == "Disk") // task�� type�� Disk
		{
			printf("[Allocation] ID: %d,   Type: Disk\n", tList[i].tid);
			idecon->GetDQ()->push(tList[i]); // idecon�� DQ�� push
		}
		else if (tList[i].type == "Mouse") // task�� type�� Mouse
		{
			printf("[Allocation] ID: %d,   Type: Mouse\n", tList[i].tid);
			idecon->GetMQ()->push(tList[i]);// idecon�� MQ�� push
		}
		else if (tList[i].type == "Printer") // task�� type�� Printer
		{
			printf("[Allocation] ID: %d,   Type: Printer\n", tList[i].tid);
			idecon->GetPQ()->push(tList[i]);// idecon�� PQ�� push
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