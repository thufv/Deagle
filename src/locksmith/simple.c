#include <pthread.h>

pthread_mutex_t lock;
int shared;

void* run(void*);

int main() {
  pthread_t t1, t2;

  pthread_mutex_init(&lock, NULL);
  pthread_create(&t1, NULL, run, NULL);
  pthread_create(&t2, NULL, run, NULL);
  return 1;
}

void *run(void *arg) {
  pthread_mutex_lock(&lock);
  shared++;
  pthread_mutex_unlock(&lock);
  return NULL;
}
