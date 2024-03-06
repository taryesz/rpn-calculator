//
// Created by Tarazjusz Szulakiewicz on 06/03/2024.
//

#ifndef UNTITLED1_LIST_H
#define UNTITLED1_LIST_H

#endif //UNTITLED1_LIST_H

#include "node.h"

#define INDEX_INCREMENTATION 1

typedef struct List {
    struct Node* head;
} *List;

void add_node_at_end(struct List* self, struct Node* insert_node) {

    Node list_node = self->head;

    while (list_node->next != NULL) {
        list_node = list_node->next;
    }

    list_node->next = insert_node;
    insert_node->previous = list_node;

    int index = list_node->index + INDEX_INCREMENTATION;
    insert_node->index = index;

}

