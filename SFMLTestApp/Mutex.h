#pragma once
///	Critical section managing for multi-threading
class Mutex
{
public:
	///	Constructor
	Mutex();
	///	Destructor
	~Mutex();

	///	Enter the critical section
	void enter();
	///	Leave the critical section
	void leave();

private:
	void* m_data;

	// Prevent copying
	Mutex(const Mutex&);
	Mutex& operator=(const Mutex&);
};

/**
*	A lock is a clever object that you construct on the stack
*	and for the duration of its lifetime your object is protected
*	from any other threads.
*	You have to put Locks inside all the methods of your object that
*	access data shared with the captive thread.
*/
class Lock
{
public:
	///	Acquire the state of the semaphore
	Lock( Mutex & p_mutex ) : m_mutex(p_mutex)
	{
		m_mutex.enter();
	}

	///	Release the state of the semaphore
	~Lock()
	{
		m_mutex.leave();
	}

private:
	Mutex & m_mutex;

	// Prevent copying
	Lock(const Lock&);
	Lock& operator=(const Lock&);
};