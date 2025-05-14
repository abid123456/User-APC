// User-APC.cpp

#include <windows.h>
#include <iostream>

int counter = 0;

void Papcfunc(
	ULONG_PTR Parameter
)
{
	puts("<> APC called");
	counter++;
	//wprintf(L"[%u] APC callback has fired with param=%Id\n", GetCurrentThreadId(), Parameter);
} 

DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
)
{
	DWORD curr_tid = GetCurrentThreadId();
	wprintf(L"[%u] Thread has started\n", curr_tid);

	for (int i = 0; i < 100; i++) {
		DWORD dwR = SleepEx(1, TRUE);
		//wprintf(L"[%u] SleepEx returned %d\n", curr_tid, dwR);
		puts(dwR ? "[thread] SleepEx returned 192" : "[thread] SleepEx returned 0");
		wprintf(L"[thread] counter: %d\n", counter);
	}
	wprintf(L"[%u] Loop done\n", curr_tid);

	Sleep(1000);

	wprintf(L"[%u] Thread exiting\n", curr_tid);
	return 0;
}

int main()
{
	HANDLE hThread = CreateThread(NULL, 0, ThreadProc, 0, 0, NULL);
	DWORD dr;
	if (hThread)
	{
		puts("Sleeping");
		Sleep(100);
		puts("Sleep done");
		for (int q = 0; q < 100; q++)
		{
			puts("APC scheduling");
			dr = QueueUserAPC(Papcfunc, hThread, q);
			if (!dr)
			{
				wprintf(L"ERROR: (%d) QueueUserAPC with value q=%d\n", GetLastError(), q);
				break;
			}
			else {
				puts("APC scheduled");
			}
		}


		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
	else
		wprintf(L"ERROR: (%d) CreateThread\n", GetLastError());
}
