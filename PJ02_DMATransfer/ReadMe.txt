OS Project 02 DMA Transfer ����
 DMA Transfer�� ���� �����غ����ν� Multi-Thread�� �˾ƺ���, DMA Transfer�� ��� �̷������ �� �˾ƺ���.

���α׷��� ����Ǹ� �켱 ����ڿ��� �� ���� task�� ������ �������� �Է¹޴´�.
�Է¹��� ���ڸ�ŭ MakingTask(int task_num, task* tList) �� ���� task�� �����Ѵ�. task_num�� ������ task�� ��, tList�� ������ task�� �ӽ� ������ list�̴�.
task�� int type�� tid, string type�� type ������ ������ �ִ� ����ü�̴�.

�׷��� conList��� string type�� ���̰� 3�� list�� �����ϴµ�, �̴� { "Disk", "Mouse", "Printer" }�� �����ȴ�.
�׸��� Allocation(int task_num, task* tList, IDECon* idecon)�� ���� ���� task�� type�� ���� ���� IDEController(Disk Controller, Mouse Controller, Printer Controller)�� �Ҵ��Ѵ�.

�Ҵ��� �����ٸ� ���� Multi-Thread �������� task ó���� �����Ѵ�.
�� �̻� ó���� task�� ���ٸ� Processing�� �����ٰ� ������ش�.

�� task ó�� ������ ������ ����.
�켱 �� queue�� task type�̴�.
�ش� queue�� ù ��° task�� pop�ϰ�(ó���� ����), DMA Controller���� ó���� �������� �˸��鼭 pop�� task�� memory buffer�� �߰��Ѵ�.
�� ��, c++ <queue>�� pop �Լ��� ���ϰ��� �����Ƿ�, pop�ϱ� ���� �ӽ÷� queue�� ù ��° ���� �����ؾ��Ѵ�.

thread ������ ������ ���� main �Լ��� ����� �� �ֱ� ������, �� thread�� join()�� ���� ���� ������ ��ٸ���.
��� thread�� ������ �����ٸ�, dma_controller�� PrintBuffer() �Լ��� ȣ���� Memory Buffer�� ����� task���� ���������� ����ϰ�, ���α׷��� �����Ѵ�.