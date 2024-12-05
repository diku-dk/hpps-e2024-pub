#include "list.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
  struct list* l = list_new();
  assert(l != NULL);
  assert(list_length(l) == 0);
  list_insert(l, 20);
  assert(list_length(l) == 1);
  list_insert(l, 5);
  assert(list_length(l) == 2);
  assert(list_remove(l) == 5);
  list_free(l);
}
