#include <iostream>
#include <string>
#include <thread>
#include <queue>
#include <ctime>
using namespace std;

struct task // task ±¸Á¶Ã¼
{
	int tid;
	string type;
};

class DMACon
{
public:
	DMACon() {};
	void SaveTask(task& t);
	void PrintBuffer();

private:
	queue<task> mBuffer; // memory buffer
};