#include <pthread.h>

int main(void)
{
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_unlock(&mutex);
}
