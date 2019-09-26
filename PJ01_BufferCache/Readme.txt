OS Project 01 Buffer Cache ����
  �޸� ���� ��� �� �ϳ��� Buffer Cache�� �����غ����ν�, kernel�� ��� �޸𸮸� �����ϰ� �����ϴ��� �����ϰ� �����Ѵ�.


���α׷��� �����ϸ� buffer�� �� �� ���� �������� ����ڿ��� ���´�. ����ڴ� ���� buffer�� ������ �Է��Ѵ�.
�׸��� hash queue�� �����ϱ� ���ؼ� mod number�� �Է��ϰ� �Ǵµ�,
�� header���� ���� �� �ִ� buffer�� �ִ� ������ 10���� ���� �����Ͽ� �Է��Ѵ�. (buffer�� num�� �ߺ����� �ʰ� �����)
(ex: buffer�� 40�� ������µ�, ���� mod number�� 3�� �Է��Ѵٸ� header�� ������ 3���̴�.
�� �� ���α׷��� ��ġ�� �ʴ� buffer�� 40�� ��������ؼ�(�Ұ���) ���α׷��� ������ �ʰ� ����ؼ� ���ư���.
�̷��� ������ ���� �ʰԲ� ���� buffer�� ������ modnumber*10 �̸��̾�� �Ѵ�.)

�׸��� ���α׷��� �������� ���� number�� ����, �� hashqueue�� header���� ����Ʈ�� �̷��.
hashqueue�� ���������, �ܼ�â�� print �ȴ�.
�ܼ�â�� Ȯ���ϰ� ����ڴ� ã����� ���ڸ� �Է��Ѵ�.


<Scenario 1>
���� �ش� buffer�� hashqueue�� �����ϰ� busy ���¶��, �ó����� 1�� ��찡 ����ȴ�.
�ش� buffer�� free���� ������ ���α׷��� sleep�ϰ�, (time library�� �̿��Ͽ� 3�ʸ� sleep ���־���.)
continue�� ���� while������ ���ư���.

<Scenario 5>
���� �ش� buffer�� hashqueue�� �����ϰ� busy ���°� �ƴ϶��, �ó����� 5�� ��찡 ����ȴ�.
buffer�� busy ���¶�� ǥ���ϰ�, (���¸� LOCKED�� ����) freelist���� buffer�� �����.
���������� buffer�� return �Ѵ�.

<Scenario 4>
���� �ش� buffer�� hashqueue�� �������� �ʰ� freelist�� ���̰� 0�̶��, �ó����� 4�� ��찡 ����ȴ�.
���� ���α׷������� ��� buffer�� free ���°� �� ������ sleep�ϴµ�,
�� ���α׷������� ������ header�� number �߿� random number�� ������ �ش� header �� LOCKED ����,
�� busy�� ������ buffer�� ã�� UNLOCKED�� �ٲ��־���. ���� busy ������ buffer�� ���ٸ� �ٽ� random number�� �����־���. �׸��� free ���°� �Ǿ��� ������, freelist�� �߰����ش�.
���� ��� buffer�� free ���°� �Ǿ��� ������, continue�� ���� while������ ���ư���.

<Scenario 3>
���� �ش� buffer�� hashqueue�� �������� �ʰ� freelist�� ù ��° buffer�� ���°� DELAYED���, �ó����� 3�� ��찡 ����ȴ�.
�켱 freelist�� ù ��° buffer�� �����Ѵ�. �׸��� hashqueue������ �ش� buffer�� ���¸� WRITE�� �����Ѵ�.
�׸��� continue�� ���� while������ ���ư���.

<Scenario 2>
���� �ش� buffer�� hashqueue�� �������� �ʰ� freelist�� ù ��° buffer�� ���°� DELAYED�� �ƴ϶��, �ó����� 2�� ��찡 ����ȴ�.
�켱 freelist�� ù ��° buffer�� �����Ѵ�. �׸��� �� buffer�� hashqueue������ �����Ѵ�.
�׸��� �Է��� number�� buffer�� ���Ӱ� hashqueue�� �������ش�.
���������� buffer�� return �Ѵ�.