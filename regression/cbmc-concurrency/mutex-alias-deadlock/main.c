#define _XOPEN_SOURCE 700
#include <pthread.h>

typedef struct
{
  pthread_mutex_t mutex;
  int rank;
} node;

typedef struct
{
  node *a, *b;
  int swap;
} args;

static pthread_barrier_t gate;

static void *worker(void *raw)
{
  args *x = raw;
  node *first = x->swap ? x->b : x->a;
  node *second = x->swap ? x->a : x->b;
#ifdef ORDERED
  if(first->rank > second->rank)
  {
    node *tmp = first;
    first = second;
    second = tmp;
  }
  pthread_barrier_wait(&gate);
#endif
  pthread_mutex_lock(&first->mutex);
#ifndef ORDERED
  pthread_barrier_wait(&gate);
#endif
  pthread_mutex_lock(&second->mutex);
  pthread_mutex_unlock(&second->mutex);
  pthread_mutex_unlock(&first->mutex);
  return 0;
}

int main(void)
{
  node a = {0}, b = {0};
  pthread_mutex_init(&a.mutex, 0);
  pthread_mutex_init(&b.mutex, 0);
  a.rank = 0;
  b.rank = 1;
  pthread_barrier_init(&gate, 0, 2);
  args x = {&a, &b, 0}, y = {&a, &b, 1};
  pthread_t t0, t1;
  pthread_create(&t0, 0, worker, &x);
  pthread_create(&t1, 0, worker, &y);
  pthread_join(t0, 0);
  pthread_join(t1, 0);
  return 0;
}
