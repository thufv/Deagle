#include <pthread.h>

typedef struct
{
  pthread_mutex_t mu;
  int payload;
} Node;

typedef struct
{
  Node *a, *b;
  int choice;
} Args;

static void *worker(void *raw)
{
  Args *args = raw;
  Node *node = args->choice ? args->a : args->b;
  pthread_mutex_lock(&node->mu);
#ifdef SELF_DEADLOCK
  pthread_mutex_lock(&node->mu);
#endif
  pthread_mutex_unlock(&node->mu);
  return 0;
}

int main(void)
{
#ifdef ARRAY
  Node nodes[2] = {0};
  Args args = {&nodes[0], &nodes[1], 0};
#else
  struct
  {
    Node a, b;
  } nodes = {0};
  Args args = {&nodes.a, &nodes.b, 0};
#endif
  pthread_mutex_init(&args.a->mu, 0);
  pthread_mutex_init(&args.b->mu, 0);
  pthread_t thread;
  pthread_create(&thread, 0, worker, &args);
  pthread_join(thread, 0);
  return 0;
}
