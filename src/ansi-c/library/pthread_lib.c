/* FUNCTION: pthread_mutex_init */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_mutex_init(
        pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
{
    __CPROVER_HIDE:;
    *((signed char *)mutex)=0;
    if(mutexattr!=0) (void)*mutexattr;
    return 0;
}

/* FUNCTION: pthread_mutex_lock */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    __CPROVER_HIDE:;
    __CPROVER_atomic_begin();
    __CPROVER_assert(*((signed char *)mutex)!=-1,
                     "mutex not initialised or destroyed");
    __CPROVER_assume(!*((signed char *)mutex));
    *((signed char *)mutex)=1;
    __CPROVER_atomic_end();

    __CPROVER_fence("WWfence", "RRfence", "RWfence", "WRfence",
                    "WWcumul", "RRcumul", "RWcumul", "WRcumul");

    return 0; // we never fail
}

/* FUNCTION: pthread_mutex_trylock */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    __CPROVER_HIDE:;
    int return_value;
    __CPROVER_atomic_begin();

    __CPROVER_assert(*((signed char *)mutex)!=-1,
                     "mutex not initialised or destroyed");

    if(*((signed char *)mutex)==1)
    {
        // failed
        return_value=1;
    }
    else
    {
        // ok
        return_value=0;
        *((signed char *)mutex)=1;
    }

    __CPROVER_atomic_end();

    __CPROVER_fence("WWfence", "RRfence", "RWfence", "WRfence",
                    "WWcumul", "RRcumul", "RWcumul", "WRcumul");

    return return_value;
}

/* FUNCTION: pthread_mutex_unlock */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    __CPROVER_HIDE:;
    // the fence must be before the unlock
    __CPROVER_fence("WWfence", "RRfence", "RWfence", "WRfence",
                    "WWcumul", "RRcumul", "RWcumul", "WRcumul");
    __CPROVER_atomic_begin();
    __CPROVER_assert(*((signed char *)mutex)==1,
                     "must hold lock upon unlock");
    *((signed char *)mutex)=0;
    __CPROVER_atomic_end();

    return 0; // we never fail
}

/* FUNCTION: pthread_mutex_destroy */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    __CPROVER_HIDE:;
    __CPROVER_assert(*((signed char *)mutex)==0,
                     "lock held upon destroy");
    *((signed char *)mutex)=-1;
    return 0;
}

/* FUNCTION: pthread_exit */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

extern __CPROVER_bool __CPROVER_threads_exited[];
extern __CPROVER_thread_local unsigned long __CPROVER_thread_id;

extern __CPROVER_thread_local unsigned long __CPROVER_next_thread_key;

inline void pthread_exit(void *value_ptr)
{
    __CPROVER_HIDE:;
    if(value_ptr!=0) (void)*(char*)value_ptr;

    __CPROVER_threads_exited[__CPROVER_thread_id]=1;
    __CPROVER_assume(0);
}

/* FUNCTION: pthread_join */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

#ifndef __CPROVER_ERRNO_H_INCLUDED
#include <errno.h>
#define __CPROVER_ERRNO_H_INCLUDED
#endif

extern __CPROVER_bool __CPROVER_threads_exited[];
extern __CPROVER_thread_local unsigned long __CPROVER_thread_id;
extern unsigned long __CPROVER_next_thread_id;

inline int pthread_join(pthread_t thread, void **value_ptr)
{
    __CPROVER_HIDE:;
    if((unsigned long)thread>__CPROVER_next_thread_id) return ESRCH;
    if((unsigned long)thread==__CPROVER_thread_id) return EDEADLK;
    if(value_ptr!=0) (void)**(char**)value_ptr;
    __CPROVER_assume(__CPROVER_threads_exited[(unsigned long)thread]);
    return 0;
}

/* FUNCTION: pthread_rwlock_destroy */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_destroy(pthread_rwlock_t *lock)
{
    __CPROVER_HIDE:;
    __CPROVER_assert(*((signed char *)lock)==0,
                     "lock held upon destroy");
    *((signed char *)lock)=-1;
    return 0;
}

/* FUNCTION: pthread_rwlock_init */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_init(pthread_rwlock_t *lock,
                               const pthread_rwlockattr_t *attr)
{
    __CPROVER_HIDE:;
    (*(signed char *)lock)=0;
    if(attr!=0) (void)*attr;
    return 0;
}

/* FUNCTION: pthread_rwlock_rdlock */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_rdlock(pthread_rwlock_t *lock)
{
    __CPROVER_HIDE:;
    __CPROVER_atomic_begin();
    __CPROVER_assert(*((signed char *)lock)!=-1,
                     "lock not initialised or destroyed");
    __CPROVER_assume(!*((signed char *)lock));
    *((signed char *)lock)=1;
    __CPROVER_atomic_end();
    return 0; // we never fail
}

/* FUNCTION: pthread_rwlock_tryrdlock */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_tryrdlock(pthread_rwlock_t *lock)
{
    __CPROVER_HIDE:;
    __CPROVER_atomic_begin();
    if((*(signed char *)lock & 2)!=0) { __CPROVER_atomic_end(); return 1; }
    (*(signed char *)lock)|=1;
    __CPROVER_atomic_end();
    return 0;
}

/* FUNCTION: pthread_rwlock_trywrlock */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_trywrlock(pthread_rwlock_t *lock)
{
    __CPROVER_HIDE:;
    __CPROVER_atomic_begin();
    if(*(signed char *)lock) { __CPROVER_atomic_end(); return 1; }
    (*(signed char *)lock)=2;
    __CPROVER_atomic_end();
    return 0;
}

/* FUNCTION: pthread_rwlock_unlock */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_unlock(pthread_rwlock_t *lock)
{
    __CPROVER_HIDE:;
    __CPROVER_assert(*((signed char *)lock)==1,
                     "must hold lock upon unlock");
    // TODO: unlocks all held locks at once
    *((signed char *)lock)=0;
    return 0; // we never fail
}

/* FUNCTION: pthread_rwlock_wrlock */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_wrlock(pthread_rwlock_t *lock)
{
    __CPROVER_HIDE:;
    __CPROVER_atomic_begin();
    __CPROVER_assert(*((signed char *)lock)!=-1,
                     "lock not initialised or destroyed");
    __CPROVER_assume(!*((signed char *)lock));
    *((signed char *)lock)=2;
    __CPROVER_atomic_end();
    return 0; // we never fail
}

/* FUNCTION: pthread_create */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

extern __CPROVER_bool __CPROVER_threads_exited[];
extern __CPROVER_thread_local unsigned long __CPROVER_thread_id;
extern unsigned long __CPROVER_next_thread_id;
extern __CPROVER_thread_local unsigned int __CPROVER_thread_keys[];

// using separate function avoid unnecessary copies of local variables
// from functions invoking pthread_create, don't inline!
void __actual_thread_spawn(
        void * (*start_routine)(void *),
        void *arg,
        unsigned long id)
{
    __CPROVER_HIDE:;
    __CPROVER_ASYNC_1: __CPROVER_thread_id=id,
        start_routine(arg),
        __CPROVER_fence("WWfence", "RRfence", "RWfence", "WRfence",
                        "WWcumul", "RRcumul", "RWcumul", "WRcumul"),
        __CPROVER_threads_exited[id]=1;
}

int pthread_create(
        pthread_t *thread,
        const pthread_attr_t *attr,
        void * (*start_routine)(void *),
        void *arg)
{
    __CPROVER_HIDE:;
    unsigned long this_thread_id;
    __CPROVER_atomic_begin();
    this_thread_id=++__CPROVER_next_thread_id;
    __CPROVER_atomic_end();

    if(thread)
    {
#ifdef __APPLE__
        // pthread_t is a pointer type on the Mac
    *thread=(pthread_t)this_thread_id;
#else
        *thread=this_thread_id;
#endif
    }

    if(attr) (void)*attr;
    __actual_thread_spawn(start_routine, arg, this_thread_id);

    return 0;
}

/* FUNCTION: pthread_cond_init */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_cond_init(
        pthread_cond_t *cond,
        const pthread_condattr_t *attr)
{ __CPROVER_HIDE:
    *((unsigned *)cond)=0;
    if(attr) (void)*attr;
    return 0;
}

/* FUNCTION: pthread_cond_signal */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_cond_signal(
        pthread_cond_t *cond)
{ __CPROVER_HIDE:
    __CPROVER_atomic_begin();
    (*((unsigned *)cond))++;
    __CPROVER_atomic_end();
    return 0;
}

/* FUNCTION: pthread_cond_broadcast */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_cond_broadcast(
        pthread_cond_t *cond)
{ __CPROVER_HIDE:
    __CPROVER_atomic_begin();
    *((unsigned *)cond)=(unsigned)-1;
    __CPROVER_atomic_end();
    return 0;
}

/* FUNCTION: pthread_cond_wait */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

inline int pthread_cond_wait(
        pthread_cond_t *cond,
        pthread_mutex_t *mutex)
{
    __CPROVER_HIDE:
    pthread_mutex_unlock(mutex);
    __CPROVER_atomic_begin();
    __CPROVER_assume(*((unsigned *)cond));
    (*((unsigned *)cond))--;
    __CPROVER_atomic_end();
    pthread_mutex_lock(mutex);
    return 0; // we never fail
}
//inline int pthread_cond_wait(
//        pthread_cond_t *cond,
//        pthread_mutex_t *mutex)
//{ __CPROVER_HIDE:
//
//    (void)*mutex;
//
//#ifdef __CPROVER_CUSTOM_BITVECTOR_ANALYSIS
//    __CPROVER_assert(__CPROVER_get_must(mutex, "mutex-init"),
//                   "mutex must be initialized");
//
//  __CPROVER_assert(__CPROVER_get_must(mutex, "mutex-locked"),
//                   "mutex must be locked");
//
//  __CPROVER_assert(!__CPROVER_get_may(mutex, "mutex-destroyed"),
//                   "mutex must not be destroyed");
//
//  __CPROVER_clear_may(mutex, "mutex-locked");
//#endif
//
//    __CPROVER_atomic_begin();
//    __CPROVER_assume(*((unsigned *)cond));
//    (*((unsigned *)cond))--;
//    __CPROVER_atomic_end();
//
//    return 0; // we never fail
//}

/* FUNCTION: pthread_spin_lock */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

// no pthread_spinlock_t on the Mac
#ifndef __APPLE__
int pthread_spin_lock(pthread_spinlock_t *lock)
{
    __CPROVER_HIDE:;
    __CPROVER_atomic_begin();
    __CPROVER_assume(!*((unsigned *)lock));
    (*((unsigned *)lock))=1;
    __CPROVER_atomic_end();

    __CPROVER_fence("WWfence", "RRfence", "RWfence", "WRfence",
                    "WWcumul", "RRcumul", "RWcumul", "WRcumul");
    return 0;
}
#endif

/* FUNCTION: pthread_spin_unlock */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

// no pthread_spinlock_t on the Mac
#ifndef __APPLE__
int pthread_spin_unlock(pthread_spinlock_t *lock)
{
    __CPROVER_HIDE:;
    // This is atomic_full_barrier() in glibc.
    // The fence must be before the unlock.
    __CPROVER_fence("WWfence", "RRfence", "RWfence", "WRfence",
                    "WWcumul", "RRcumul", "RWcumul", "WRcumul");
    *((unsigned *)lock) = 0;
    return 0;
}
#endif

/* FUNCTION: pthread_spin_trylock */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

#ifndef __CPROVER_ERRNO_H_INCLUDED
#include <errno.h>
#define __CPROVER_ERRNO_H_INCLUDED
#endif

// no pthread_spinlock_t on the Mac
#ifndef __APPLE__
int pthread_spin_trylock(pthread_spinlock_t *lock)
{
    __CPROVER_HIDE:;
    int result;
    __CPROVER_atomic_begin();
    if(*((unsigned *)lock))
        result=EBUSY;
    else
    {
        result=0;
        (*((unsigned *)lock))=1;
    }
    __CPROVER_atomic_end();

    __CPROVER_fence("WWfence", "RRfence", "RWfence", "WRfence",
                    "WWcumul", "RRcumul", "RWcumul", "WRcumul");
    return result;
}
#endif


//////// __FHY_ADD_BEGIN__
/* FUNCTION: pthread_barrier_init */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

int __VERIFIER_nondet_int();

// no pthread_barrier_t on the Mac
#ifndef __APPLE__
inline int pthread_barrier_init(
        pthread_barrier_t *restrict barrier,
        const pthread_barrierattr_t *restrict attr, unsigned count)
{
    __CPROVER_HIDE:;
    (void)barrier;
    (void)attr;
    (void)count;

#ifdef __CPROVER_CUSTOM_BITVECTOR_ANALYSIS
    __CPROVER_set_must(barrier, "barrier-init");
  __CPROVER_clear_may(barrier, "barrier-destroyed");
#endif

    int result=__VERIFIER_nondet_int();
    return result;
}
#endif

/* FUNCTION: pthread_barrier_destroy */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

int __VERIFIER_nondet_int();

// no pthread_barrier_t on the Mac
#ifndef __APPLE__
inline int pthread_barrier_destroy(pthread_barrier_t *barrier)
{
    __CPROVER_HIDE:;

    (void)barrier;

#ifdef __CPROVER_CUSTOM_BITVECTOR_ANALYSIS
    __CPROVER_assert(__CPROVER_get_must(barrier, "barrier-init"),
                   "pthread barrier must be initialized");
  __CPROVER_assert(!__CPROVER_get_may(barrier, "barrier-destroyed"),
                   "pthread barrier must not be destroyed");
  __CPROVER_set_may(barrier, "barrier-destroyed");
#endif

    int result=__VERIFIER_nondet_int();
    return result;
}
#endif

/* FUNCTION: pthread_barrier_wait */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

int __VERIFIER_nondet_int();

// no pthread_barrier_t on the Mac
#ifndef __APPLE__
inline int pthread_barrier_wait(pthread_barrier_t *barrier)
{
    __CPROVER_HIDE:;

    (void)barrier;

#ifdef __CPROVER_CUSTOM_BITVECTOR_ANALYSIS
    __CPROVER_assert(__CPROVER_get_must(barrier, "barrier-init"),
                   "pthread barrier must be initialized");
  __CPROVER_assert(!__CPROVER_get_may(barrier, "barrier-destroyed"),
                   "pthread barrier must not be destroyed");
#endif

    int result=__VERIFIER_nondet_int();
    return result;
}
#endif

/* FUNCTION: pthread_key_create */
#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

extern __CPROVER_thread_local unsigned int __CPROVER_thread_keys[];
extern __CPROVER_thread_local unsigned long __CPROVER_next_thread_key;

inline int pthread_key_create(pthread_key_t *key, void (*destructor)(void *))
{
    __CPROVER_HIDE:;
    __CPROVER_thread_keys[__CPROVER_next_thread_key] = 0;
    __CPROVER_assert(destructor == 0, "Consider no destructor now!");

    *key = __CPROVER_next_thread_key++;
    return 0;
}

/* FUNCTION: pthread_key_delete */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

extern __CPROVER_thread_local unsigned int  __CPROVER_thread_keys[];

inline int pthread_key_delete(pthread_key_t key)
{
    __CPROVER_HIDE:;
    __CPROVER_thread_keys[key] = 0;
    return 0;
}

/* FUNCTION: pthread_getspecific */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

extern __CPROVER_thread_local unsigned int __CPROVER_thread_keys[];

inline void *pthread_getspecific(pthread_key_t key)
{
    __CPROVER_HIDE:;
    return (void *)__CPROVER_thread_keys[key];
}

/* FUNCTION: pthread_setspecific */

#ifndef __CPROVER_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __CPROVER_PTHREAD_H_INCLUDED
#endif

extern __CPROVER_thread_local unsigned int __CPROVER_thread_keys[];

inline int pthread_setspecific(pthread_key_t key, const void *value)
{
    __CPROVER_HIDE:;
    __CPROVER_thread_keys[key] = (unsigned int)value;
    return 0;
}
//////// __FHY_ADD_END__