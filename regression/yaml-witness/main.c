#include <pthread.h>

int shared;
int active[3];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
extern int nondet_int(void);

void *writer(void *arg)
{
#if CASE == 6
  pthread_mutex_lock(&mutex);
#endif
#if CASE == 7
  if(active[(unsigned long)arg])
#endif
    shared = 1; // WRITE
#if CASE == 6
  pthread_mutex_unlock(&mutex);
#endif
  return 0;
}

void *reader(void *arg)
{
  int value = shared; // READ
  return (void *)(unsigned long)value;
}

void *launcher(void *arg)
{
  pthread_t child;
  pthread_create(&child, 0, writer, 0); // NESTED
#if CASE != 11
  pthread_join(child, 0);
#endif
  return 0;
}

void reach_error(void)
{
  __CPROVER_assert(0, "reach_error");
}

int main(void)
{
  pthread_t threads[3];
#if CASE == 9
  __CPROVER_assert(0, "ordinary assertion");
#elif CASE == 10
  reach_error(); // REACH
#elif CASE == 11
  pthread_create(&threads[0], 0, launcher, 0); // ORDERED_PARENT
  pthread_join(threads[0], 0);
  pthread_create(&threads[1], 0, writer, 0); // ORDERED_WRITER
  pthread_join(threads[1], 0);
#elif CASE == 5
  for(int i = 0; i < 2; ++i)
    pthread_create(&threads[i], 0, writer, 0); // LOOP
  for(int i = 0; i < 2; ++i)
    pthread_join(threads[i], 0);
#else
#if CASE == 7
  active[0] = nondet_int();
  __CPROVER_assume(active[0] == 0);
  active[1] = active[2] = 1;
#elif CASE == 8
  int create = nondet_int();
  __CPROVER_assume(!create);
  if(create)
    pthread_create(&threads[2], 0, writer, 0); // SKIPPED
#endif
  pthread_create(&threads[0], 0, writer, 0); // CREATE1
#if CASE == 2
  pthread_create(&threads[1], 0, reader, 0); // CREATE_READER
#elif CASE == 3
  pthread_create(&threads[1], 0, launcher, 0); // CREATE_LAUNCHER
#elif CASE == 4
  shared = 2; // MAIN_WRITE
#else
  pthread_create(&threads[1], 0, writer, (void *)1); // CREATE2
#endif
#if CASE == 7
  pthread_create(&threads[2], 0, writer, (void *)2); // CREATE3
  pthread_join(threads[2], 0);
#endif
  pthread_join(threads[0], 0);
#if CASE != 4
  pthread_join(threads[1], 0);
#endif
#endif
  return 0;
}
