// list.c

#include <stdlib.h>
#include <assert.h>

#include "list.h"

struct node {
  int val;
  struct node* tail; // NULL if last element;
};

struct list {
  struct node* head; // NULL if empty.
};

struct list* list_new(void) {
  struct list *l =
    malloc(sizeof(struct list));
  l->head = NULL;
  return l;
}

void list_free(struct list* l) {
  while (l->head != NULL) {
    list_remove(l);
  }
  free(l);
}

int list_length(struct list* l) {
  struct node *cur = l->head;
  int n = 0;
  while (cur != NULL) {
    n++;
    cur = cur->tail;
  }
  return n;
}

void list_insert(struct list* l, int x) {
  struct node *new =
    malloc(sizeof(struct node));
  new->val = x;
  new->tail = l->head;
  l->head = new;
}

int list_remove(struct list* l) {
  int x = l->head->val;
  struct node* new_head = l->head->tail;
  free(l->head);
  l->head = new_head;
  return x;
}
