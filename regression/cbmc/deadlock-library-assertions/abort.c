#include <stdlib.h>

int main(void)
{
  abort();
  __CPROVER_assert(0, "abort must not return");
}
