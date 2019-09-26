/*
Banker's Algorithm ����
�ۼ���: ������

����ڰ� �Է��� request�� ����, safe sequence�� ���θ� �Ǵ��Ͽ�,
�ִٸ� �� ������ ����ϰ�, ���ٸ� safe sequence�� ������ ����ڿ��� �˷��ش�.

�ڿ��� type�� A, B, C, D �� 4���̸�,
�� instance�� A(12), B(7), C(9), D(5)�� �����Ͽ���.
allocation�� max ���� main �Լ����� �ۼ��� ���Ƿ� �����Ͽ���.

���μ����� p0���� p4���� �� 5���̴�.

safe sequence�� �ִ� request�� ���� ������ ����.
p1�� p1�� need���� �۰ų� ���� ������ request
p3�� p3�� need���� �۰ų� ���� ������ request
p4�� p4�� need���� �۰ų� ���� ������ request

�� ���μ����� need(max-allocation)���� �� ū ���� request �� �ÿ��� ������ ����Ѵ�.
���� 0 0 0 0 0�� �Է��� ��쿡�� ���α׷��� �����Ѵ�.
*/

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// vector overloading
vector<vector<int>> operator-(vector<vector<int>>& va, vector<vector<int>>& vb);
bool operator<=(vector<vector<int>>& va, vector<int>& vb);
vector<int> operator+=(vector<int>& va, vector<int>& vb);

void DisplayCurrentState(vector<vector<int>>& al, vector<vector<int>>& mx, vector<vector<int>>& nd, vector<int> aval);

int main()
{
	vector<vector<int>> allocation = { {2,0,1,0},{4,1,0,0},{3,2,2,1},{0,1,2,2},{1,0,1,0} };
	vector<vector<int>> max = { {7,2,3,1},{6,2,2,1},{9,2,3,1},{2,3,5,3},{2,2,1,0} };
	vector<vector<int>> need = max - allocation;
	vector<int> available = { 2,3,3,2 }; // �Ҵ��ϰ� �� �� ������ �ڿ� ��
	vector<int> work;
	vector<string> finish;

	int pNum = 0, ia = 0, ib = 0, ic = 0, id = 0;
	while (true)
	{
		DisplayCurrentState(allocation, max, need, available);
		cout << "Please enter your request(process num, A, B, C, D)." << endl;
		cout << "ex) 1 7 4 3 2 -> that means process1, A->7, B->4, C->3, D->2" << endl;
		cout << "(if you want to exit the program, please enter 0 0 0 0 0) >> ";
		cin >> pNum >> ia >> ib >> ic >> id;

		if (pNum == 0 && ia == 0 && ib == 0 && ic == 0 && id == 0)
			break;
		
		bool inAlgorithm = true;
		work = available;
		finish = { "F", "F" ,"F" ,"F" ,"F" };
		
		vector<int> temp = { ia, ib, ic, id };
		if (!(temp <= need[pNum])) // ���� request�� ���� need���� ũ�ٸ�(max���� �� ���� allocate�� �Ϸ��� �Ѵٸ�)
		{
			cout << "You can't allocate instances more than max..." << endl;
			inAlgorithm = false;
		}
		else if (finish[pNum] == "F" && (need[pNum] <= work))
		{
			work += allocation[pNum];
			finish[pNum] = "T";
			cout << "p" << pNum << "(" << work[0] << ", " << work[1] << ", " << work[2] << ", " << work[3] << ")" << endl;
		}
		else
		{
			cout << "There is no safe sequence." << endl;
			inAlgorithm = false;
		}

		int i = 0;
		while (inAlgorithm)
		{
			if (finish[i] == "F" && (need[i] <= work))
			{
				work += allocation[i];
				finish[i] = "T";
				cout << "p" << i << "(" << work[0] << ", " << work[1] << ", " << work[2] << ", " << work[3] << ")" << endl;
				i = 0; // i �ʱ�ȭ
			}
			else
				i++;

			if (i == 5) // ���������� ��
			{
				for (int j = 0; j < 5; j++)
				{
					if (finish[j] == "F") // ���� F�� �ϳ��� �ִٸ� ���� �Ҹ���
					{
						cout << "There is no safe sequence." << endl;
						inAlgorithm = false;
						break;
					}
				}
				if (inAlgorithm) // ��� finish ���� T���
				{
					cout << "This is the Safe Sequence!" << endl; // ������� ����� ��ΰ� safe sequence���� ������
					inAlgorithm = false;
				}
			}
		}
	}
	cout << "Exit the program..." << endl;

	system("pause");
	return 0;
}

vector<vector<int>> operator-(vector<vector<int>>& va, vector<vector<int>>& vb)
{
	vector<vector<int>> result = va;

	for (int i = 0; i < vb.size(); i++)
	{
		for (int j = 0; j < vb[0].size(); j++)
		{
			result[i][j] -= vb[i][j];
		}
	}
	return result;
}

bool operator<=(vector<vector<int>>& va, vector<int>& vb)
{
	for (int i = 0; i < va.size(); i++)
	{
		for (int j = 0; j < va[0].size(); j++)
		{
			if (va[i][j] > vb[j])
				return false;
		}
	}
	return true;
}

vector<int> operator+=(vector<int>& va, vector<int>& vb)
{
	for (int i = 0; i < va.size(); i++)
	{
		va[i] += vb[i];
	}
	return va;
}

void DisplayCurrentState(vector<vector<int>>& al, vector<vector<int>>& mx, vector<vector<int>>& nd, vector<int> aval) // ���� ���¸� ���
{
	// allocation, max, need, aval vector ������ ���ʴ�� ���
	cout << "\n   [Allocation]\t [Max]\t   [Need]     [aval]" << endl;
	for (int i = 0; i < al.size(); i++)
	{
		cout << "p" << i << ")  ";
		for (int j = 0; j < al[0].size(); j++)
		{
			cout << al[i][j] << " ";
		}
		cout << "   ";
		for (int k = 0; k < mx[0].size(); k++)
		{
			cout << mx[i][k] << " ";
		}
		cout << "   ";
		for (int l = 0; l < nd[0].size(); l++)
		{
			cout << nd[i][l] << " ";
		}
		if (i == 0)
		{
			cout << "   ";
			for (int j = 0; j < aval.size(); j++)
			{
				cout << aval[j] << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
}