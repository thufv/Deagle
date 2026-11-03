#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int initialized = 1;

int pthread_mutex_lock(pthread_mutex_t *p)
{
  __CPROVER_atomic_begin();
  // Acquisition success is this entire condition, not its last conjunct.
  __CPROVER_assume(!*((signed char *)p) && initialized == 1);
  *((signed char *)p) = 1;
  __CPROVER_atomic_end();
  return 0;
}

static void *worker(void *unused)
{
  pthread_mutex_lock(&mutex);
#ifdef SELF_DEADLOCK
  pthread_mutex_lock(&mutex);
#endif
  pthread_mutex_unlock(&mutex);
  return 0;
}

int main(void)
{
  pthread_t first, second;
  pthread_create(&first, 0, worker, 0);
  pthread_create(&second, 0, worker, 0);
  pthread_join(first, 0);
  pthread_join(second, 0);
  return 0;
}
