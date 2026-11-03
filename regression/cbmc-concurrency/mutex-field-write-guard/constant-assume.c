#include <pthread.h>

int main(void)
{
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&mutex);
#ifdef SELF_DEADLOCK
  // The assume is false and no subsequent lock write is generated.
  pthread_mutex_lock(&mutex);
#endif
  pthread_mutex_unlock(&mutex);
  return 0;
}
