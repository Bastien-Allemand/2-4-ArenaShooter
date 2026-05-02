#include "Thread.h"


gce::Thread::Thread(void* arg, LPTHREAD_START_ROUTINE func)
{
	m_hThread = CreateThread(nullptr, 0, func, arg, 0, &m_threadID);
}

bool gce::Thread::LaunchThread(void* arg, LPTHREAD_START_ROUTINE func)
{
	DWORD exitCode;
	GetExitCodeThread(m_hThread, &exitCode);

	if (exitCode == STILL_ACTIVE)
	{
		return false;
	}

	OpenThread(THREAD_ALL_ACCESS, false, m_threadID);

	m_hThread = CreateThread(nullptr, 0, func, arg, 0, &m_threadID);

	if (m_hThread == nullptr)
	{
		return false;
	}

	return true;
}

bool gce::Thread::CloseThread()
{
	TerminateThread(m_hThread, m_threadID);

	return CloseHandle(m_hThread);
}

gce::Thread::~Thread()
{
	TerminateThread(m_hThread, 0);

	CloseHandle(m_hThread);
}