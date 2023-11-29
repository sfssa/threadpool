/*
 * @Author: atpdxy
 * @Date: 2023-11-29 09:28:13
 * @Version: 1.0
 * @LastEditors: sfssa 1664549131@qq.com
 * @LastEditTime: 2023-11-29 09:36:30
 * @Description: Windows平台下的自旋锁
 */
#pragma once
#include <atomic>
#include <windows.h>

namespace atpdxy{

class SpinLock{
public:
    static constexpr DWORD SPINLOCK_COUNT=4000;

    SpinLock(){
        InitializeCriticalSectionAndSpinCount(&m_cs,SPINLOCK_COUNT);
    }

    ~SpinLock(){
        DeleteCriticalSection(&m_cs);
    }

    void lock(){
        EnterCriticalSection(&m_cs);
    }

    void unlock(){
        LeaveCriticalSection(&m_cs);
    }
    
private:
    CRITICAL_SECTION m_cs;
};

class SpinLockGuard{
public:
    SpinLockGuard(SpinLock& lock)
        :spin_lock(lock){
        spin_lock.lock();
    }
    
    ~SpinLockGuard(){
        spin_lock.unlock();
    }
private:
    SpinLock& spin_lock;
};
}