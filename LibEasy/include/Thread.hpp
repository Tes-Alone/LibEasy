/*
 * 文件名: Thread.hpp 
 *
 * 日期/时间:
 *		开始: 2020年09月24日 20:44:39 周四
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#ifndef _EASY_THREAD_H_
#define _EASY_THREAD_H_

#include <windows.h>

#include "BaseTypes.hpp"
#include "String.hpp"
#include "Exception.hpp"
#include "SharedPointer.hpp"
#include "Console.hpp"

namespace Easy {

class Lock : public EnableSharedFromThis<Lock> {

friend class Thread;

private:
	CRITICAL_SECTION sc;
public:
	Lock() {
		InitializeCriticalSection(&sc);
	}
	
	~Lock() {
		DeleteCriticalSection(&sc);
	}

private:
	Lock(const Lock& copy) = delete;
	Lock(Lock&& move) = delete;
	Lock& operator=(const Lock& copy) = delete;
	Lock& operator=(Lock&& move) = delete;
}; // Lock

class ThreadCreateException : public Exception {
public:
	ThreadCreateException(const String& func, e_int line) :
		Exception(String{L"Thread create failed."}, func, line){}
		
	String what() const override {
		return L"ThreadCreateException";
	}
}; // ThreadCreateException


class ThreadStartException : public Exception {
public:
	ThreadStartException(const String& func, e_int line) :
		Exception(String{L"Thread start failed."}, func, line){}
		
	String what() const override {
		return L"ThreadStartException";
	}
}; // ThreadStartException

class ThreadStatusException : public Exception {
public:
	ThreadStatusException(const String& func, e_int line) :
		Exception(String{L"Thread status invalid."}, func, line){}
		
	String what() const override {
		return L"ThreadStatusException";
	}
}; // ThreadStatusException

class Thread : public EnableSharedFromThis<Thread> {

private:
	HANDLE handle;
	SharedPointer<Lock> _lock;
	e_bool _isLocked;
	e_bool _isStarted;
	String _name;
	
	static CONDITION_VARIABLE _cond;
	static e_int _id;
public:
	static const int PRIORITY_HIGHEST      = THREAD_PRIORITY_HIGHEST;
	static const int PRIORITY_ABOVE_NORMAL = THREAD_PRIORITY_ABOVE_NORMAL;
	static const int PRIORITY_NORMAL       = THREAD_PRIORITY_NORMAL;
	static const int PRIORITY_BELOW_NORMAL = THREAD_PRIORITY_BELOW_NORMAL;
	static const int PRIORITY_LOWEST       = THREAD_PRIORITY_LOWEST;
	
	static e_void sleep(e_ulong mills);
	
	Thread();
	
	Thread(const String& name);
	
	virtual ~Thread();

	e_void start();
	
	e_void join();
	
	e_void wait();
	
	e_void wakeup();
	
	e_void wakeupAll();
	
	e_void setLock(const SharedPointer<Lock>& lk);
	
	e_void lock();
	
	e_void unlock();
	
	e_bool isLocked() const;
	
	e_int getPriority() const;
	
	e_void setPriority(e_int priority);
	
	e_void yield();
	
	String toString() const override;
	
	virtual e_void run() =0;

private:	
	static unsigned _run(void* t) {
		Thread* thread = (Thread*)t;
		try {
			thread->run();
		} catch (ExitException& e) {
		} catch (Exception& e) {
			Console::outln(String{L"Easy exception arise, in thread '"} + thread->toString() + L"' what() is: " + e.what());
			Console::outln(String{L"Function is: "} + e.getFunction());
			Console::outln(String{L"At line: "} + Integer{e.getLine()});
			Console::outln(String{L"Message is: "} + e.getMessage());
		} catch (std::exception& stde) {
			Console::out(String{"C++ build-in exception arise, in thread '"} + thread->toString() + L"' what() is: ");
			std::cout << stde.what() << std::endl;
		}
		return 0;
	}
	
	Thread(const Thread& copy) = delete;
	Thread(Thread&& move) = delete;
	Thread& operator=(const Thread& copy) = delete;
	Thread& operator=(Thread&& copy) = delete;
}; // Thread

} // namespace Easy


#endif // _EASY_THREAD_H_
