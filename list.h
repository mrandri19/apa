#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct list {
  int head;
  struct list *next;
} * List;

List LISTInit() { return NULL; }
List LISTInsertHead(List l, int item) {
  List new_list = (List)malloc(sizeof(struct list));
  new_list->head = item;
  new_list->next = l;
  return new_list;
}
#endif
