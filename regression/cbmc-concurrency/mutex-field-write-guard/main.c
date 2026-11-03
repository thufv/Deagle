#include <pthread.h>

extern int nondet_int(void);

typedef struct
{
  pthread_mutex_t mu;
  int payload;
} Node;

typedef struct
{
  Node *first, *second;
  int choose_second;
} Args;

static void *worker(void *raw)
{
  Args *args = raw;
  Node *node = args->choose_second ? args->second : args->first;
  pthread_mutex_lock(&node->mu);
  // Updating payload must not create a shared write to mu.
  node->payload++;
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
  struct { Node first, second; } nodes = {0};
  Args args = {&nodes.first, &nodes.second, 0};
#endif
#ifdef NONDET
  args.choose_second = nondet_int();
#endif
  pthread_mutex_init(&args.first->mu, 0);
  pthread_mutex_init(&args.second->mu, 0);
  pthread_t first, second;
  pthread_create(&first, 0, worker, &args);
  pthread_create(&second, 0, worker, &args);
  pthread_join(first, 0);
  pthread_join(second, 0);
  return 0;
}
