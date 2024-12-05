// list.h

struct list;

// Creates new list.
struct list* list_new(void);

// Free list.
//
// Precondition: list must be empty.
void list_free(struct list*);

// Returns length of list.
int list_length(struct list*);

// Inserts element at head of list.
void list_insert(struct list*, int);

// Removes head of list.
//
// Precondition: list must not be empty.
int list_remove(struct list*);
