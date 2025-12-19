#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

typedef struct Node {
  char name[100];
  char service_type[100];
  int ticket_number;
  struct Node *next;
} Node;

Node *head = NULL;
Node *tail = NULL; // Added tail pointer for FIFO
int ticket_counter = 1;

EXPORT int enqueue(const char *name, const char *service_type) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  if (!new_node)
    return -1;

  strncpy(new_node->name, name, 99);
  new_node->name[99] = '\0'; // Ensure null usage
  strncpy(new_node->service_type, service_type, 99);
  new_node->service_type[99] = '\0';

  int t_num = ticket_counter++;
  new_node->ticket_number = t_num;
  new_node->next = NULL; // New node is always at the end

  // FIFO logic: Insert at tail
  if (tail == NULL) {
    // Queue was empty
    head = new_node;
    tail = new_node;
  } else {
    tail->next = new_node;
    tail = new_node;
  }

  return t_num;
}

// Helper struct to return data to Python
typedef struct {
  char name[100];
  char service_type[100];
  int ticket_number;
  int success; // 1 if found, 0 if empty
} CustomerData;

EXPORT CustomerData dequeue() {
  CustomerData data;
  data.success = 0;

  if (head == NULL) {
    return data;
  }

  Node *temp = head;
  head = head->next;

  if (head == NULL) {
    tail = NULL; // If queue becomes empty, update tail
  }

  strncpy(data.name, temp->name, 100);
  strncpy(data.service_type, temp->service_type, 100);
  data.ticket_number = temp->ticket_number;
  data.success = 1;

  free(temp);
  return data;
}

EXPORT int get_count() {
  int count = 0;
  Node *current = head;
  while (current != NULL) {
    count++;
    current = current->next;
  }
  return count;
}

// Optional: Peek at the next person without removing
EXPORT CustomerData peek() {
  CustomerData data;
  data.success = 0;

  if (head == NULL) {
    return data;
  }

  strncpy(data.name, head->name, 100);
  strncpy(data.service_type, head->service_type, 100);
  data.ticket_number = head->ticket_number;
  data.success = 1;

  return data;
}
