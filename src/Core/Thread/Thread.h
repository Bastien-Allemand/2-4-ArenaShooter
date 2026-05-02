#ifndef CORE_THREAD_H_INCLUDED
#define CORE_THREAD_H_INCLUDED

#include <Windows.h>

namespace gce
{

	struct Thread
	{
		Thread() = default;
		Thread(void* arg, LPTHREAD_START_ROUTINE func);

		Thread(const Thread&) = delete;
		Thread& operator=(const Thread&) = delete;

		bool LaunchThread(void* arg, LPTHREAD_START_ROUTINE func);

		bool CloseThread();

		~Thread();
	private:
		HANDLE m_hThread = nullptr;
		DWORD m_threadID;
	};

}

#endif