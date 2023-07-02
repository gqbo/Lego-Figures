// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(const char* debugName, int initialValue)
{
    name = (char *)debugName;
    value = initialValue;
    queue = new List<Thread*>;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append(currentThread);		// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    interrupt->SetLevel(oldLevel);		// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    interrupt->SetLevel(oldLevel);
}

#ifdef USER_PROGRAM
//----------------------------------------------------------------------
// Semaphore::Destroy
// 	Destroy the semaphore, freeing the waiting threads
//	This is used to destroy a user semaphore
//----------------------------------------------------------------------

void
Semaphore::Destroy()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    while ( (thread = queue->Remove() ) != NULL )	// make thread ready
	scheduler->ReadyToRun(thread);

    interrupt->SetLevel(oldLevel);
}

#endif


// Lock class
Lock::Lock(const char* debugName) {
    name = new char[strlen(debugName) + 1];
    strcpy(name, debugName); 
}

Lock::~Lock() {
    delete[] name;
}


void Lock::Acquire() {
    // disable interrupts to guarantee atomicity of the lock acquire operation
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    // if the lock is already held, put the thread to sleep
    if (value == BUSY) {
        waitQueue->Append(currentThread);
        currentThread->Sleep();
    }
    // mark the lock as BUSY and set the lock holder
    value = BUSY;
    holder = currentThread;
    // restore the previous interrupt level
    (void) interrupt->SetLevel(oldLevel);
}


void Lock::Release() {
        // disable interrupts to guarantee atomicity of the lock release operation
        IntStatus oldLevel = interrupt->SetLevel(IntOff);
        ASSERT(isHeldByCurrentThread());

        // mark the lock as FREE, clear the lock holder, and wake up a thread waiting on the lock
        value = FREE;
        holder = NULL;
        if (!waitQueue->IsEmpty()) {
            Thread* nextThread = (Thread*)waitQueue->Remove();
            scheduler->ReadyToRun(nextThread);
        }

        // restore the previous interrupt level
        (void) interrupt->SetLevel(oldLevel);
}


bool Lock::isHeldByCurrentThread() {
    return (currentThread == holder);
}


Condition::Condition(const char* debugName) {
    name = new char[strlen(debugName) + 1];
    strcpy(name, debugName);
    waitQueue = new List<Thread*>;
    
}


Condition::~Condition() {
    delete waitQueue;
    delete[] name;
}


void Condition::Wait( Lock * conditionLock ) {
    // disable interrupts to guarantee atomicity of the wait operation
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    ASSERT(conditionLock->isHeldByCurrentThread());
    // release the lock and put the thread to sleep
    conditionLock->Release();
    waitQueue->Append(currentThread);
    currentThread->Sleep();
    // re-acquire the lock
    conditionLock->Acquire();
    // restore the previous interrupt level
    (void) interrupt->SetLevel(oldLevel);
}


void Condition::Signal( Lock * conditionLock ) {
    // disable interrupts to guarantee atomicity of the signal operation
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    ASSERT(conditionLock->isHeldByCurrentThread());
    // wake up a thread waiting on the condition variable
    if (!waitQueue->IsEmpty()) {
        Thread* nextThread = (Thread*)waitQueue->Remove();
        scheduler->ReadyToRun(nextThread);
    }
    // restore the previous interrupt level
    (void) interrupt->SetLevel(oldLevel);
}


void Condition::Broadcast( Lock * conditionLock ) {
    // disable interrupts to guarantee atomicity of the broadcast operation
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    ASSERT(conditionLock->isHeldByCurrentThread());
    // wake up all threads waiting on the condition variable
    while (!waitQueue->IsEmpty()) {
        Thread* nextThread = (Thread*)waitQueue->Remove();
        scheduler->ReadyToRun(nextThread);
    }
    // restore the previous interrupt level
    (void) interrupt->SetLevel(oldLevel);
}


// Mutex class
Mutex::Mutex( const char * debugName ) {

}

Mutex::~Mutex() {

}

void Mutex::Lock() {

}

void Mutex::Unlock() {

}


// Barrier class
Barrier::Barrier( const char * debugName, int count ) {
}

Barrier::~Barrier() {
}

void Barrier::Wait() {
}

