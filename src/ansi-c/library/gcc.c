/* FUNCTION: __builtin_ia32_sfence */

#if defined(__i386__) || defined(__x86_64__)

void __builtin_ia32_sfence(void)
{
  __asm("sfence");
}

#endif

/* FUNCTION: __builtin_ia32_lfence */

#if defined(__i386__) || defined(__x86_64__)

void __builtin_ia32_lfence(void)
{
  __asm("lfence");
}

#endif

/* FUNCTION: __builtin_ia32_mfence */

#if defined(__i386__) || defined(__x86_64__)

void __builtin_ia32_mfence(void)
{
  __asm("mfence");
}

#endif

/* FUNCTION: __sync_synchronize */

void __sync_synchronize(void)
{
  // WARNING: this was a NOP before gcc 4.3.1,
  // but is now believed to be the strongest possible barrier.

  #if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) >= 40301
  __CPROVER_HIDE:;
  __CPROVER_fence("WWfence", "RRfence", "RWfence", "WRfence",
                  "WWcumul", "RRcumul", "RWcumul", "WRcumul");
  #endif
}

/* FUNCTION: __atomic_test_and_set */

void __atomic_thread_fence(int memorder);

_Bool __atomic_test_and_set(void *ptr, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_begin();
  _Bool result = *(char *)ptr == 1;
  *(char *)ptr = 1;
  __atomic_thread_fence(memorder);
  __CPROVER_atomic_end();
  return result;
}

/* FUNCTION: __atomic_clear */

void __atomic_thread_fence(int memorder);

void __atomic_clear(_Bool *ptr, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_begin();
  *(char *)ptr = 0;
  __atomic_thread_fence(memorder);
  __CPROVER_atomic_end();
}

/* FUNCTION: __atomic_thread_fence */

#if __STDC_VERSION__ >= 201112L
// GCC 4.8 did claim to support C++11, but failed to ship stdatomic.h
#  if !defined(__GNUC__) || (__GNUC__ * 100 + __GNUC_MINOR__) >= 409
#    include <stdatomic.h>
#  endif
#endif

#ifndef __ATOMIC_RELAXED
#  define __ATOMIC_RELAXED 0
#endif

#ifndef __ATOMIC_CONSUME
#  define __ATOMIC_CONSUME 1
#endif

#ifndef __ATOMIC_ACQUIRE
#  define __ATOMIC_ACQUIRE 2
#endif

#ifndef __ATOMIC_RELEASE
#  define __ATOMIC_RELEASE 3
#endif

#ifndef __ATOMIC_ACQ_REL
#  define __ATOMIC_ACQ_REL 4
#endif

#ifndef __ATOMIC_SEQ_CST
#  define __ATOMIC_SEQ_CST 5
#endif

void __atomic_thread_fence(int memorder)
{
__CPROVER_HIDE:;
  if(memorder == __ATOMIC_CONSUME || memorder == __ATOMIC_ACQUIRE)
    __CPROVER_fence("RRfence", "RWfence", "RRcumul", "RWcumul");
  else if(memorder == __ATOMIC_RELEASE)
    __CPROVER_fence("WRfence", "WWfence", "WRcumul", "WWcumul");
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_SEQ_CST)
    __CPROVER_fence(
      "WWfence",
      "RRfence",
      "RWfence",
      "WRfence",
      "WWcumul",
      "RRcumul",
      "RWcumul",
      "WRcumul");
}

/* FUNCTION: __atomic_signal_fence */

void __atomic_thread_fence(int memorder);

void __atomic_signal_fence(int memorder)
{
__CPROVER_HIDE:;
  __atomic_thread_fence(memorder);
}

// __SZH_ADD_BEGIN__

/* FUNCTION: __atomic_store */

inline void __atomic_store(void* loc, void* val, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_store(loc, val, memorder);
}

/* FUNCTION: __atomic_store_n */

void __atomic_store_n(void* loc, void* val, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_store_n(loc, val, memorder);
}

/* FUNCTION: __atomic_load */

void __atomic_load(void* loc, void* ret, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_load(loc, ret, memorder);
}

/* FUNCTION: __atomic_load_n */

void __atomic_load_n(void* loc, void* ret, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_load_n(loc, ret, memorder);
}

/* FUNCTION: __atomic_compare_exchange */

#ifndef __ATOMIC_RELAXED
#  define __ATOMIC_RELAXED 0
#endif

#ifndef __ATOMIC_CONSUME
#  define __ATOMIC_CONSUME 1
#endif

#ifndef __ATOMIC_ACQUIRE
#  define __ATOMIC_ACQUIRE 2
#endif

#ifndef __ATOMIC_RELEASE
#  define __ATOMIC_RELEASE 3
#endif

#ifndef __ATOMIC_ACQ_REL
#  define __ATOMIC_ACQ_REL 4
#endif

#ifndef __ATOMIC_SEQ_CST
#  define __ATOMIC_SEQ_CST 5
#endif

_Bool __atomic_compare_exchange(int* left, int* right, int val, int is_weak, int memorder1, int memorder2)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_begin();

  int __atomic_compare_exchange_right_load;
  __atomic_compare_exchange_right_load = *right;
  int __atomic_compare_exchange_left_load;
  if(memorder1 == __ATOMIC_SEQ_CST)
    __atomic_load(left, &__atomic_compare_exchange_left_load, __ATOMIC_SEQ_CST);
  else if(memorder1 == __ATOMIC_ACQ_REL || memorder1 == __ATOMIC_ACQUIRE)
    __atomic_load(left, &__atomic_compare_exchange_left_load, __ATOMIC_ACQUIRE);
  else
    __atomic_load(left, &__atomic_compare_exchange_left_load, __ATOMIC_RELAXED);

  _Bool equal = (__atomic_compare_exchange_right_load == __atomic_compare_exchange_left_load);
  if(equal)
  {
    if(memorder1 == __ATOMIC_SEQ_CST)
      __atomic_store(right, &val, __ATOMIC_SEQ_CST);
    else if(memorder1 == __ATOMIC_ACQ_REL || memorder1 == __ATOMIC_RELEASE)
      __atomic_store(right, &val, __ATOMIC_RELEASE);
    else
      __atomic_store(right, &val, __ATOMIC_RELAXED);
    
  }
  else
  {
    *right = __atomic_compare_exchange_left_load;
  }
  __CPROVER_atomic_end();

  return equal;
}

/* FUNCTION: __atomic_exchange */

#ifndef __ATOMIC_RELAXED
#  define __ATOMIC_RELAXED 0
#endif

#ifndef __ATOMIC_CONSUME
#  define __ATOMIC_CONSUME 1
#endif

#ifndef __ATOMIC_ACQUIRE
#  define __ATOMIC_ACQUIRE 2
#endif

#ifndef __ATOMIC_RELEASE
#  define __ATOMIC_RELEASE 3
#endif

#ifndef __ATOMIC_ACQ_REL
#  define __ATOMIC_ACQ_REL 4
#endif

#ifndef __ATOMIC_SEQ_CST
#  define __ATOMIC_SEQ_CST 5
#endif

void __atomic_exchange(int* loc, int* val, int* ret, int memorder)
{
  __CPROVER_atomic_begin();

  int __atomic_exchange_old_val;
  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_load(loc, &__atomic_exchange_old_val, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_ACQUIRE)
    __atomic_load(loc, &__atomic_exchange_old_val, __ATOMIC_ACQUIRE);
  else
    __atomic_load(loc, &__atomic_exchange_old_val, __ATOMIC_RELAXED);

  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_store(loc, val, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_RELEASE)
    __atomic_store(loc, val, __ATOMIC_RELEASE);
  else
    __atomic_store(loc, val, __ATOMIC_RELAXED);

  *ret = __atomic_exchange_old_val;

  __CPROVER_atomic_end();
}

/* FUNCTION: __atomic_fetch_add */

#ifndef __ATOMIC_RELAXED
#  define __ATOMIC_RELAXED 0
#endif

#ifndef __ATOMIC_CONSUME
#  define __ATOMIC_CONSUME 1
#endif

#ifndef __ATOMIC_ACQUIRE
#  define __ATOMIC_ACQUIRE 2
#endif

#ifndef __ATOMIC_RELEASE
#  define __ATOMIC_RELEASE 3
#endif

#ifndef __ATOMIC_ACQ_REL
#  define __ATOMIC_ACQ_REL 4
#endif

#ifndef __ATOMIC_SEQ_CST
#  define __ATOMIC_SEQ_CST 5
#endif

int __atomic_fetch_add(int* loc, int val, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_begin();

  // load
  int __atomic_fetch_load_ret;
  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_ACQUIRE)
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_ACQUIRE);
  else
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_RELAXED);

  // calc new value
  int __atomic_fetch_store_val;
  __atomic_fetch_store_val = __atomic_fetch_load_ret + val;

  // and then store
  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_RELEASE)
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_RELEASE);
  else
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_RELAXED);

  __CPROVER_atomic_end();

  return __atomic_fetch_load_ret;
}

/* FUNCTION: __atomic_fetch_sub */

#ifndef __ATOMIC_RELAXED
#  define __ATOMIC_RELAXED 0
#endif

#ifndef __ATOMIC_CONSUME
#  define __ATOMIC_CONSUME 1
#endif

#ifndef __ATOMIC_ACQUIRE
#  define __ATOMIC_ACQUIRE 2
#endif

#ifndef __ATOMIC_RELEASE
#  define __ATOMIC_RELEASE 3
#endif

#ifndef __ATOMIC_ACQ_REL
#  define __ATOMIC_ACQ_REL 4
#endif

#ifndef __ATOMIC_SEQ_CST
#  define __ATOMIC_SEQ_CST 5
#endif

int __atomic_fetch_sub(int* loc, int val, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_begin();

  // load
  int __atomic_fetch_load_ret;
  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_ACQUIRE)
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_ACQUIRE);
  else
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_RELAXED);

  // calc new value
  int __atomic_fetch_store_val;
  __atomic_fetch_store_val = __atomic_fetch_load_ret - val;

  // and then store
  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_RELEASE)
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_RELEASE);
  else
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_RELAXED);

  __CPROVER_atomic_end();

  return __atomic_fetch_load_ret;
}

/* FUNCTION: __atomic_fetch_and */

#ifndef __ATOMIC_RELAXED
#  define __ATOMIC_RELAXED 0
#endif

#ifndef __ATOMIC_CONSUME
#  define __ATOMIC_CONSUME 1
#endif

#ifndef __ATOMIC_ACQUIRE
#  define __ATOMIC_ACQUIRE 2
#endif

#ifndef __ATOMIC_RELEASE
#  define __ATOMIC_RELEASE 3
#endif

#ifndef __ATOMIC_ACQ_REL
#  define __ATOMIC_ACQ_REL 4
#endif

#ifndef __ATOMIC_SEQ_CST
#  define __ATOMIC_SEQ_CST 5
#endif

int __atomic_fetch_and(int* loc, int val, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_begin();

  // load
  int __atomic_fetch_load_ret;
  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_ACQUIRE)
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_ACQUIRE);
  else
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_RELAXED);

  // calc new value
  int __atomic_fetch_store_val;
  __atomic_fetch_store_val = __atomic_fetch_load_ret & val;

  // and then store
  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_RELEASE)
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_RELEASE);
  else
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_RELAXED);

  __CPROVER_atomic_end();

  return __atomic_fetch_load_ret;
}

/* FUNCTION: __atomic_fetch_or */

#ifndef __ATOMIC_RELAXED
#  define __ATOMIC_RELAXED 0
#endif

#ifndef __ATOMIC_CONSUME
#  define __ATOMIC_CONSUME 1
#endif

#ifndef __ATOMIC_ACQUIRE
#  define __ATOMIC_ACQUIRE 2
#endif

#ifndef __ATOMIC_RELEASE
#  define __ATOMIC_RELEASE 3
#endif

#ifndef __ATOMIC_ACQ_REL
#  define __ATOMIC_ACQ_REL 4
#endif

#ifndef __ATOMIC_SEQ_CST
#  define __ATOMIC_SEQ_CST 5
#endif

int __atomic_fetch_or(int* loc, int val, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_begin();

  // load
  int __atomic_fetch_load_ret;
  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_ACQUIRE)
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_ACQUIRE);
  else
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_RELAXED);

  // calc new value
  int __atomic_fetch_store_val;
  __atomic_fetch_store_val = __atomic_fetch_load_ret | val;

  // and then store
  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_RELEASE)
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_RELEASE);
  else
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_RELAXED);

  __CPROVER_atomic_end();

  return __atomic_fetch_load_ret;
}

/* FUNCTION: __atomic_fetch_xor */

#ifndef __ATOMIC_RELAXED
#  define __ATOMIC_RELAXED 0
#endif

#ifndef __ATOMIC_CONSUME
#  define __ATOMIC_CONSUME 1
#endif

#ifndef __ATOMIC_ACQUIRE
#  define __ATOMIC_ACQUIRE 2
#endif

#ifndef __ATOMIC_RELEASE
#  define __ATOMIC_RELEASE 3
#endif

#ifndef __ATOMIC_ACQ_REL
#  define __ATOMIC_ACQ_REL 4
#endif

#ifndef __ATOMIC_SEQ_CST
#  define __ATOMIC_SEQ_CST 5
#endif

int __atomic_fetch_xor(int* loc, int val, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_begin();

  // load
  int __atomic_fetch_load_ret;
  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_ACQUIRE)
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_ACQUIRE);
  else
    __atomic_load(loc, &__atomic_fetch_load_ret, __ATOMIC_RELAXED);

  // calc new value
  int __atomic_fetch_store_val;
  __atomic_fetch_store_val = __atomic_fetch_load_ret ^ val;

  // and then store
  if(memorder == __ATOMIC_SEQ_CST)
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_SEQ_CST);
  else if(memorder == __ATOMIC_ACQ_REL || memorder == __ATOMIC_RELEASE)
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_RELEASE);
  else
    __atomic_store(loc, &__atomic_fetch_store_val, __ATOMIC_RELAXED);

  __CPROVER_atomic_end();

  return __atomic_fetch_load_ret;
}

/* FUNCTION: __LKMM_LOAD */

int __LKMM_LOAD(int* loc, int memorder)
{
__CPROVER_HIDE:;
  return __CPROVER_LKMM_LOAD(loc, memorder);
}

/* FUNCTION: __LKMM_STORE */

void __LKMM_STORE(int* loc, int val, int memorder)
{
__CPROVER_HIDE:;
  __CPROVER_LKMM_STORE(loc, val, memorder);
}

/* FUNCTION: __LKMM_FENCE */

void __LKMM_FENCE(int memorder)
{
__CPROVER_HIDE:;
    __CPROVER_fence(
      "WWfence",
      "RRfence",
      "RWfence",
      "WRfence",
      "WWcumul",
      "RRcumul",
      "RWcumul",
      "WRcumul");
}

/* FUNCTION: __LKMM_XCHG */

#ifndef __LKMM_RELAXED
#define __LKMM_RELAXED 0
#endif

#ifndef __LKMM_ONCE
#define __LKMM_ONCE 1
#endif

#ifndef __LKMM_ACQUIRE
#define __LKMM_ACQUIRE 2
#endif

#ifndef __LKMM_RELEASE
#define __LKMM_RELEASE 3
#endif

#ifndef __LKMM_MB
#define __LKMM_MB 4
#endif

int __LKMM_XCHG(int* loc, int val, int memorder)
{
__CPROVER_HIDE:;
  if(memorder == __LKMM_MB)
    __LKMM_FENCE(__LKMM_MB);

  __CPROVER_atomic_begin();
  int xchg_tmp;
  if(memorder == __LKMM_ACQUIRE)
    xchg_tmp = __LKMM_LOAD(loc, __LKMM_ACQUIRE);
  else
    xchg_tmp = __LKMM_LOAD(loc, __LKMM_ONCE);

  if(memorder == __LKMM_RELEASE)
    __LKMM_STORE(loc, val, __LKMM_RELEASE);
  else
    __LKMM_STORE(loc, val, __LKMM_ONCE);
  __CPROVER_atomic_end();

  if(memorder == __LKMM_MB)
    __LKMM_FENCE(__LKMM_MB);
  
  return xchg_tmp;
}

/* FUNCTION: __LKMM_CMPXCHG */

#ifndef __LKMM_RELAXED
#define __LKMM_RELAXED 0
#endif

#ifndef __LKMM_ONCE
#define __LKMM_ONCE 1
#endif

#ifndef __LKMM_ACQUIRE
#define __LKMM_ACQUIRE 2
#endif

#ifndef __LKMM_RELEASE
#define __LKMM_RELEASE 3
#endif

#ifndef __LKMM_MB
#define __LKMM_MB 4
#endif

int __LKMM_CMPXCHG(int* loc, int old, int new, int memorder1, int memorder2)
{
__CPROVER_HIDE:;
  if(memorder1 == __LKMM_MB)
    __LKMM_FENCE(__LKMM_MB);

  __CPROVER_atomic_begin();
  int xchg_tmp;
  if(memorder1 == __LKMM_ACQUIRE)
    xchg_tmp = __LKMM_LOAD(loc, __LKMM_ACQUIRE);
  else
    xchg_tmp = __LKMM_LOAD(loc, __LKMM_ONCE);

  if(xchg_tmp == old)
  {
    if(memorder2 == __LKMM_RELEASE)
      __LKMM_STORE(loc, new, __LKMM_RELEASE);
    else
      __LKMM_STORE(loc, new, __LKMM_ONCE);
  }
  __CPROVER_atomic_end();

  if(memorder1 == __LKMM_MB)
    __LKMM_FENCE(__LKMM_MB);
  
  return xchg_tmp;
}

/* FUNCTION: __LKMM_ATOMIC_OP */

#ifndef __LKMM_ADD
#define __LKMM_ADD 0
#endif

#ifndef __LKMM_SUB
#define __LKMM_SUB 1
#endif

#ifndef __LKMM_AND
#define __LKMM_AND 2
#endif

#ifndef __LKMM_OR
#define __LKMM_OR 3
#endif

#ifndef __LKMM_ONCE
#define __LKMM_ONCE 1
#endif

void __LKMM_ATOMIC_OP(int* loc, int val, int op)
{
__CPROVER_HIDE:;
  __CPROVER_atomic_begin();
  if(op == __LKMM_ADD)
  {
    int fetch_tmp = __LKMM_LOAD(loc, __LKMM_ONCE);
    __LKMM_STORE(loc, fetch_tmp + val, __LKMM_ONCE);
  }
  if(op == __LKMM_SUB)
  {
    int fetch_tmp = __LKMM_LOAD(loc, __LKMM_ONCE);
    __LKMM_STORE(loc, fetch_tmp - val, __LKMM_ONCE);
  }
  if(op == __LKMM_AND)
  {
    int fetch_tmp = __LKMM_LOAD(loc, __LKMM_ONCE);
    __LKMM_STORE(loc, fetch_tmp & val, __LKMM_ONCE);
  }
  if(op == __LKMM_OR)
  {
    int fetch_tmp = __LKMM_LOAD(loc, __LKMM_ONCE);
    __LKMM_STORE(loc, fetch_tmp | val, __LKMM_ONCE);
  }
  __CPROVER_atomic_end();
}

/* FUNCTION: __LKMM_ATOMIC_FETCH_OP */

#ifndef __LKMM_ADD
#define __LKMM_ADD 0
#endif

#ifndef __LKMM_SUB
#define __LKMM_SUB 1
#endif

#ifndef __LKMM_AND
#define __LKMM_AND 2
#endif

#ifndef __LKMM_OR
#define __LKMM_OR 3
#endif

#ifndef __LKMM_ONCE
#define __LKMM_ONCE 1
#endif

#ifndef __LKMM_ACQUIRE
#define __LKMM_ACQUIRE 2
#endif

#ifndef __LKMM_RELEASE
#define __LKMM_RELEASE 3
#endif

#ifndef __LKMM_MB
#define __LKMM_MB 4
#endif

int __LKMM_ATOMIC_FETCH_OP(int* loc, int val, int memorder, int op)
{
__CPROVER_HIDE:;
  if(memorder == __LKMM_MB)
    __LKMM_FENCE(__LKMM_MB);

  __CPROVER_atomic_begin();
  int fetch_tmp;
  if(memorder == __LKMM_ACQUIRE)
    fetch_tmp = __LKMM_LOAD(loc, __LKMM_ACQUIRE);
  else
    fetch_tmp = __LKMM_LOAD(loc, __LKMM_ONCE);

  if(op == __LKMM_ADD)
  {
    if(memorder == __LKMM_RELEASE)
      __LKMM_STORE(loc, fetch_tmp + val, __LKMM_RELEASE);
    else
      __LKMM_STORE(loc, fetch_tmp + val, __LKMM_ONCE);
  }
  if(op == __LKMM_SUB)
  {
    if(memorder == __LKMM_RELEASE)
      __LKMM_STORE(loc, fetch_tmp - val, __LKMM_RELEASE);
    else
      __LKMM_STORE(loc, fetch_tmp - val, __LKMM_ONCE);
  }
  if(op == __LKMM_AND)
  {
    if(memorder == __LKMM_RELEASE)
      __LKMM_STORE(loc, fetch_tmp & val, __LKMM_RELEASE);
    else
      __LKMM_STORE(loc, fetch_tmp & val, __LKMM_ONCE);
  }
  if(op == __LKMM_OR)
  {
    if(memorder == __LKMM_RELEASE)
      __LKMM_STORE(loc, fetch_tmp | val, __LKMM_RELEASE);
    else
      __LKMM_STORE(loc, fetch_tmp | val, __LKMM_ONCE);
  }
  __CPROVER_atomic_end();

  if(memorder == __LKMM_MB)
    __LKMM_FENCE(__LKMM_MB);

  return fetch_tmp;
}

/* FUNCTION: __LKMM_ATOMIC_OP_RETURN */

#ifndef __LKMM_ADD
#define __LKMM_ADD 0
#endif

#ifndef __LKMM_SUB
#define __LKMM_SUB 1
#endif

#ifndef __LKMM_AND
#define __LKMM_AND 2
#endif

#ifndef __LKMM_OR
#define __LKMM_OR 3
#endif

#ifndef __LKMM_ONCE
#define __LKMM_ONCE 1
#endif

#ifndef __LKMM_ACQUIRE
#define __LKMM_ACQUIRE 2
#endif

#ifndef __LKMM_RELEASE
#define __LKMM_RELEASE 3
#endif

#ifndef __LKMM_MB
#define __LKMM_MB 4
#endif

int __LKMM_ATOMIC_OP_RETURN(int* loc, int val, int memorder, int op)
{
__CPROVER_HIDE:;
  if(memorder == __LKMM_MB)
    __LKMM_FENCE(__LKMM_MB);

  __CPROVER_atomic_begin();
  int fetch_tmp;
  if(memorder == __LKMM_ACQUIRE)
    fetch_tmp = __LKMM_LOAD(loc, __LKMM_ACQUIRE);
  else
    fetch_tmp = __LKMM_LOAD(loc, __LKMM_ONCE);

  if(op == __LKMM_ADD)
    fetch_tmp += val;
  if(op == __LKMM_SUB)
    fetch_tmp -= val;
  if(op == __LKMM_AND)
    fetch_tmp &= val;
  if(op == __LKMM_OR)
    fetch_tmp |= val;

  if(memorder == __LKMM_RELEASE)
    __LKMM_STORE(loc, fetch_tmp, __LKMM_RELEASE);
  else
    __LKMM_STORE(loc, fetch_tmp, __LKMM_ONCE);
  __CPROVER_atomic_end();

  if(memorder == __LKMM_MB)
    __LKMM_FENCE(__LKMM_MB);

  return fetch_tmp;
}

/* FUNCTION: __LKMM_SPIN_LOCK */

#ifndef __LKMM_ONCE
#define __LKMM_ONCE 1
#endif

#ifndef __LKMM_ACQUIRE
#define __LKMM_ACQUIRE 2
#endif

#ifndef __LKMM_STRUCT_SPINLOCK
#define __LKMM_STRUCT_SPINLOCK
typedef struct spinlock {
    int unused;
} spinlock_t;
#endif

int __LKMM_SPIN_LOCK(spinlock_t *lock)
{
  __CPROVER_HIDE:;
  __CPROVER_atomic_begin();
  int lock_status = __LKMM_LOAD((unsigned *)lock, __LKMM_ACQUIRE);
  __CPROVER_assume(!lock_status);
  __LKMM_STORE((unsigned *)lock, 1, __LKMM_ONCE);
  __CPROVER_atomic_end();
  return 0;
}

/* FUNCTION: __LKMM_SPIN_UNLOCK */

#ifndef __LKMM_STRUCT_SPINLOCK
#define __LKMM_STRUCT_SPINLOCK
typedef struct spinlock {
    int unused;
} spinlock_t;
#endif

#ifndef __LKMM_RELEASE
#define __LKMM_RELEASE 3
#endif

int __LKMM_SPIN_UNLOCK(spinlock_t *lock)
{
  __CPROVER_HIDE:;
  __LKMM_STORE((unsigned *)lock, 0, __LKMM_RELEASE);
  return 0;
}

// __SZH_ADD_END__

/* FUNCTION: __atomic_always_lock_free */

_Bool __atomic_always_lock_free(__CPROVER_size_t size, void *ptr)
{
__CPROVER_HIDE:;
  (void)ptr;
  return size <= sizeof(__CPROVER_size_t);
}

/* FUNCTION: __atomic_is_lock_free */

_Bool __atomic_is_lock_free(__CPROVER_size_t size, void *ptr)
{
__CPROVER_HIDE:;
  (void)ptr;
  return size <= sizeof(__CPROVER_size_t);
}

/* FUNCTION: __builtin_ia32_vec_ext_v4hi */

typedef short __gcc_v4hi __attribute__((__vector_size__(8)));

short __builtin_ia32_vec_ext_v4hi(__gcc_v4hi vec, int offset)
{
  return *((short *)&vec + offset);
}

/* FUNCTION: __builtin_ia32_vec_ext_v8hi */

typedef short __gcc_v8hi __attribute__((__vector_size__(16)));

short __builtin_ia32_vec_ext_v8hi(__gcc_v8hi vec, int offset)
{
  return *((short *)&vec + offset);
}

/* FUNCTION: __builtin_ia32_vec_ext_v4si */

typedef int __gcc_v4si __attribute__((__vector_size__(16)));

int __builtin_ia32_vec_ext_v4si(__gcc_v4si vec, int offset)
{
  return *((int *)&vec + offset);
}

/* FUNCTION: __builtin_ia32_vec_ext_v2di */

typedef long long __gcc_v2di __attribute__((__vector_size__(16)));

long long __builtin_ia32_vec_ext_v2di(__gcc_v2di vec, int offset)
{
  return *((long long *)&vec + offset);
}

/* FUNCTION: __builtin_ia32_vec_ext_v16qi */

typedef char __gcc_v16qi __attribute__((__vector_size__(16)));

int __builtin_ia32_vec_ext_v16qi(__gcc_v16qi vec, int offset)
{
  return *((char *)&vec + offset);
}

/* FUNCTION: __builtin_ia32_vec_ext_v4sf */

typedef float __gcc_v4sf __attribute__((__vector_size__(16)));

float __builtin_ia32_vec_ext_v4sf(__gcc_v4sf vec, int offset)
{
  return *((float *)&vec + offset);
}

/* FUNCTION: __builtin_ia32_psubsw128 */

#ifndef LIBRARY_CHECK
typedef short __gcc_v8hi __attribute__((__vector_size__(16)));
#else
__gcc_v8hi __CPROVER_saturating_minus();
#endif

__gcc_v8hi __builtin_ia32_psubsw128(__gcc_v8hi a, __gcc_v8hi b)
{
  return __CPROVER_saturating_minus(a, b);
}

/* FUNCTION: __builtin_ia32_psubusw128 */

#ifndef LIBRARY_CHECK
typedef short __gcc_v8hi __attribute__((__vector_size__(16)));
#endif

__gcc_v8hi __builtin_ia32_psubusw128(__gcc_v8hi a, __gcc_v8hi b)
{
  typedef unsigned short v8hi_u __attribute__((__vector_size__(16)));
  return (__gcc_v8hi)__CPROVER_saturating_minus((v8hi_u)a, (v8hi_u)b);
}

/* FUNCTION: __builtin_ia32_paddsw */

#ifndef LIBRARY_CHECK
typedef short __gcc_v4hi __attribute__((__vector_size__(8)));
#else
__gcc_v4hi __CPROVER_saturating_plus();
#endif

__gcc_v4hi __builtin_ia32_paddsw(__gcc_v4hi a, __gcc_v4hi b)
{
  return __CPROVER_saturating_plus(a, b);
}

/* FUNCTION: __builtin_ia32_psubsw */

#ifndef LIBRARY_CHECK
typedef short __gcc_v4hi __attribute__((__vector_size__(8)));
#else
__gcc_v4hi __CPROVER_saturating_minus_v4hi(__gcc_v4hi, __gcc_v4hi);
#  define __CPROVER_saturating_minus __CPROVER_saturating_minus_v4hi
#endif

__gcc_v4hi __builtin_ia32_psubsw(__gcc_v4hi a, __gcc_v4hi b)
{
  return __CPROVER_saturating_minus(a, b);
}

#ifdef LIBRARY_CHECK
#  undef __CPROVER_saturating_minus
#endif

/* FUNCTION: __builtin_ia32_vec_init_v4hi */

#ifndef LIBRARY_CHECK
typedef short __gcc_v4hi __attribute__((__vector_size__(8)));
#endif

__gcc_v4hi __builtin_ia32_vec_init_v4hi(short e0, short e1, short e2, short e3)
{
  return (__gcc_v4hi){e0, e1, e2, e3};
}
