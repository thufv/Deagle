#include <pthread.h>

int shared;
extern int nondet_int(void);

void write_shared(void)
{
  shared = 1; // HELPER_WRITE
}

void *writer(void *arg)
{
#if CASE == 13
  write_shared();
#else
  shared = 1; // DIRECT_WRITE
#endif
  return 0;
}

void *idle(void *arg)
{
  return 0;
}

int main(void)
{
  pthread_t first, second;
#if CASE == 12
  pthread_create(&first, 0, writer, 0); pthread_create(&second, 0, writer, 0); // SAME_LINE
#elif CASE == 14
  void *(*entry)(void *) = nondet_int() ? writer : idle;
  __CPROVER_assume(entry == writer);
  pthread_create(&first, 0, entry, 0); // POINTER_CREATE
  pthread_create(&second, 0, writer, 0); // SECOND_CREATE
#else
  pthread_create(&first, 0, writer, 0); // FIRST_CREATE
  pthread_create(&second, 0, writer, 0); // LAST_CREATE
#endif
  pthread_join(first, 0);
  pthread_join(second, 0);
  return 0;
}
