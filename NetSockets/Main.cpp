#include <thread>
#include <atomic>
#include <Windows.h>
#include <mutex>
import NetSocket;
import <vector>;
import <string>;

using namespace std;

bool b;
mutex m;

void try_set() {

	scoped_lock<mutex> lock(m);
	if (!b) 
	{
		Sleep(1000);
		b = true;
		printf("Initialized\n");
	}
}

int main()
{
	vector<thread> thrs;
	for (int i = 0; i < 100; i++)
	{
		thrs.push_back(thread(try_set)); // this will already execute try_set upon construction
	}

	for (thread& t : thrs)
		t.join(); // wait for all threads to finish.

	return 0;
}