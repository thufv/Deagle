#include <pthread.h>

extern int nondet_int(void);
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t unused = PTHREAD_MUTEX_INITIALIZER;

int pthread_mutex_unlock(pthread_mutex_t *p)
{
  __CPROVER_atomic_begin();
  *((signed char *)p) = 0;
  pthread_mutex_t *q = nondet_int() ? p : &unused;
  // This conditional write must not hide the preceding release of p.
  *((signed char *)q) = *((signed char *)q);
  __CPROVER_atomic_end();
  return 0;
}

static void *worker(void *arg)
{
  pthread_mutex_lock(&mutex);
  pthread_mutex_unlock(&mutex);
  return 0;
}

int main(void)
{
  pthread_t t, u;
  pthread_create(&t, 0, worker, 0);
  pthread_create(&u, 0, worker, 0);
  pthread_join(t, 0);
  pthread_join(u, 0);
  return 0;
}
