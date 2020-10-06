/*
 * 文件名: Thread.cpp 
 *
 * 日期/时间:
 *		开始: 2020年09月25日 15:32:41 周五
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#include "Thread.hpp"

extern "C" {

BOOLEAN WINAPI TryAcquireSRWLockExclusive (PSRWLOCK);
BOOLEAN WINAPI TryAcquireSRWLockShared (PSRWLOCK);
void WINAPI ReleaseSRWLockExclusive (PSRWLOCK);
void WINAPI ReleaseSRWLockShared (PSRWLOCK);

void WINAPI InitializeConditionVariable (PCONDITION_VARIABLE);
BOOL WINAPI SleepConditionVariableCS (PCONDITION_VARIABLE, PCRITICAL_SECTION, DWORD);
BOOL WINAPI SleepConditionVariableSRW (PCONDITION_VARIABLE, PSRWLOCK, DWORD, ULONG);
void WINAPI WakeAllConditionVariable (PCONDITION_VARIABLE);
void WINAPI WakeConditionVariable (PCONDITION_VARIABLE);

}

namespace Easy {

e_int Thread::_id = 0;
CONDITION_VARIABLE Thread::_cond = CONDITION_VARIABLE_INIT;


e_void Thread::sleep(e_ulong mills) {
	Sleep(mills);
}
	
Thread::Thread() {
	_isLocked  = false;
	_isStarted = false;
	_name      = String{L"Thread"} + Integer{_id++};
	handle     = (HANDLE)_beginthreadex(nullptr, 0, _run, this, CREATE_SUSPENDED, 0);
	if (handle == nullptr) {
		throw ThreadCreateException(__func__, __LINE__);
	}
}

Thread::Thread(const String& name) {
	_isLocked  = false;
	_isStarted = false;
	_name      = name;
	handle     = (HANDLE)_beginthreadex(nullptr, 0, _run, this, CREATE_SUSPENDED, 0);
	if (handle == nullptr) {
		throw ThreadCreateException(__func__, __LINE__);
	}
}

Thread::~Thread() {
	CloseHandle(handle);
}

e_void Thread::start() {
	if (ResumeThread(handle) == (DWORD)-1) {
		throw ThreadStartException(__func__, __LINE__);
	}
	_isStarted = true;
}

e_void Thread::join() {
	if (_isStarted) {
		if (WaitForSingleObject(handle, INFINITE) == WAIT_FAILED) {
			throw ThreadStatusException(__func__, __LINE__);
		}
	}
}

e_void Thread::wait() {
	if (!SleepConditionVariableCS(&_cond, &_lock->sc, INFINITE)) {
		throw ThreadStatusException(__func__, __LINE__);
	}
}

e_void Thread::wakeup() {
	WakeConditionVariable(&_cond);
}

e_void Thread::wakeupAll() {
	WakeAllConditionVariable(&_cond);
}

e_void Thread::setLock(const SharedPointer<Lock>& lk) {
	_lock = lk;
}

e_void Thread::lock() {
	_isLocked = true;
	EnterCriticalSection(&_lock->sc);
}

e_void Thread::unlock() {
	LeaveCriticalSection(&_lock->sc);
	_isLocked = false;
}

e_bool Thread::isLocked() const {
	return _isLocked;
}

e_int Thread::getPriority() const {
	e_int result = GetThreadPriority(handle);
	if (result == THREAD_PRIORITY_ERROR_RETURN) 
		throw ThreadStatusException(__func__, __LINE__);
	return result;
}

e_void Thread::setPriority(e_int priority) {
	if (SetThreadPriority(handle, priority) == 0) {
		throw ThreadStatusException(__func__, __LINE__);
	}
}

e_void Thread::yield() {
	SwitchToThread();
}

String Thread::toString() const {
	return System::move(String{_name});
}

} // namespace Easy