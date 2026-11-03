#include <pthread.h>

extern int nondet_int(void);
static pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t b = PTHREAD_MUTEX_INITIALIZER;

static void *worker(void *unused)
{
  pthread_mutex_lock(&a);
  pthread_mutex_lock(&b);
  pthread_mutex_unlock(&b);
  pthread_mutex_unlock(&a);
  return 0;
}

int main(void)
{
  pthread_mutex_lock(&a);
  pthread_mutex_lock(&b);
  pthread_t thread;
  pthread_create(&thread, 0, worker, 0);

  // Both acquisitions preceded thread creation, so the path guard is true.
  int choice = nondet_int();
  pthread_mutex_t *first = choice ? &a : &b;
  pthread_mutex_t *second = choice ? &b : &a;
  pthread_mutex_unlock(first);
  pthread_mutex_unlock(second);
  pthread_join(thread, 0);
  return 0;
}
