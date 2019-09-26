/*
Banker's Algorithm 구현
작성자: 권은령

사용자가 입력한 request에 대해, safe sequence의 여부를 판단하여,
있다면 그 순서를 출력하고, 없다면 safe sequence가 없음을 사용자에게 알려준다.

자원의 type은 A, B, C, D 총 4개이며,
각 instance는 A(12), B(7), C(9), D(5)로 설정하였다.
allocation과 max 또한 main 함수에서 작성자 임의로 설정하였다.

프로세스는 p0부터 p4까지 총 5개이다.

safe sequence가 있는 request의 예는 다음과 같다.
p1을 p1의 need보다 작거나 같은 값으로 request
p3을 p3의 need보다 작거나 같은 값으로 request
p4를 p4의 need보다 작거나 같은 값으로 request

각 프로세스의 need(max-allocation)보다 더 큰 값을 request 할 시에는 오류를 출력한다.
또한 0 0 0 0 0을 입력할 경우에는 프로그램을 종료한다.
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
	vector<int> available = { 2,3,3,2 }; // 할당하고 난 후 가용한 자원 수
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
		if (!(temp <= need[pNum])) // 만약 request한 값이 need보다 크다면(max보다 더 많이 allocate를 하려고 한다면)
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
				i = 0; // i 초기화
			}
			else
				i++;

			if (i == 5) // 마지막까지 옴
			{
				for (int j = 0; j < 5; j++)
				{
					if (finish[j] == "F") // 만약 F가 하나라도 있다면 조건 불만족
					{
						cout << "There is no safe sequence." << endl;
						inAlgorithm = false;
						break;
					}
				}
				if (inAlgorithm) // 모든 finish 값이 T라면
				{
					cout << "This is the Safe Sequence!" << endl; // 현재까지 출력한 경로가 safe sequence임을 말해줌
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

void DisplayCurrentState(vector<vector<int>>& al, vector<vector<int>>& mx, vector<vector<int>>& nd, vector<int> aval) // 현재 상태를 출력
{
	// allocation, max, need, aval vector 값들을 차례대로 출력
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