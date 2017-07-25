#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    void* value;
    struct node* next;
    unsigned index;
} node;

typedef struct list
{
    node* root;
    unsigned count;

    void (*ctor)(struct list* self);
    void (*add)(struct list* self, void* value);
    void (*dtor)(const struct list* self);
    void* (*get)(const struct list* self, unsigned index);
} list;

void initialize(list* list)
{
    list->root = NULL;
    list->count = 0;
}

void add(list* list, void* value){
    node* new_node = malloc(sizeof(node));
    new_node->next = list->root;
    new_node->value = value;
    new_node->index = list->count;
    list->root = new_node;
    list->count += 1;
}

void destroy(const list* list)
{
    node* temp = list->root;
    while(temp)
    {
        node* current = temp;
        temp = temp->next;
        free(current->value);
        free(current);
    }
}

void* get(const list* list, unsigned index)
{
    void* result = NULL;
    for(node* n = list->root; n != NULL; n = n->next)
    {
        if(n->index == index)
        {
            result = n->value;
            break;
        }
    }
    return result;
}

typedef struct person
{
    char name[50];
    char surname[50];
} person;

void insert_persons(list* persons)
{
    unsigned count = 0;
    printf("How many persons You want to insert? "); scanf("%d", &count);

    for(unsigned i = 0; i < count; ++i)
    {
        person *p = malloc(sizeof(person));

        printf("\nPerson no: %d\n", i + 1);
        printf("Insert name: "); scanf("%s", p->name);
        printf("Insert surname: "); scanf("%s", p->surname);

        persons->add(persons, p);
    }
}

void print_selected(list *persons)
{
    person* selected = NULL;
    do
    {
        unsigned index = 0;
        printf("\nTotal number of persons: %d\n", persons->count);
        printf("Give me an index: "); scanf("%d", &index);

        selected = (person*)persons->get(persons, index - 1);
        if(selected)
            printf("Name: %s\nSurname: %s\n\n", selected->name, selected->surname);
        else
            printf("No person with such index!\n");
    }
    while(!selected);
}

int main()
{
    list persons;
    persons.ctor = initialize;
    persons.dtor = destroy;
    persons.add = add;
    persons.get = get;

    persons.ctor(&persons);
    insert_persons(&persons);
    print_selected(&persons);

    persons.dtor(&persons);

    return 0;
}

