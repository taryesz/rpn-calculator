#include <iostream>

class node {
private:
    int content;
    int priority;
    bool operand;
    node* next;

public:

    node() {
        this->content = 0;
        this->priority = 0;
        this->operand = false;
        this->next = nullptr;
    }

    void set_content(int value) {
        this->content = value;
    }

    void set_priority(int value) {
        this->priority = value;
    }

    void set_operand(int value) {
        this->operand = value;
    }

    void set_next(node* value) {
        this->next = value;
    }

    int get_content() const {
        return this->content;
    }

    int get_priority() const {
        return this->priority;
    }

    bool is_operand() const {
        return this->operand;
    }

    node* get_next() {
        return this->next;
    }

};


class stack {
private:
    node* head;
    node* tail;

    void set_node(int content, int priority, bool operand, bool method) {

        // create a new node
        auto* symbol = new node();

        // give the node some properties
        symbol->set_content(content);
        symbol->set_priority(priority);
        symbol->set_operand(operand);

        // if the stack is empty ...
        if (head == nullptr) {

            // set both head & tail as the same node
            head = symbol;
            tail = symbol;

        }

        // if the stack is not empty ...
        else {

            // if the program puts the node ...
            if (method) {

                // connect a previous node to the new node
                this->tail->set_next(symbol);

                // update the tail
                this->tail = symbol;

            }

            // if the program pushes the node ...
            else {

                // connect a new node to the first node in the stack
                symbol->set_next(this->head);

                // update the head
                this->head = symbol;

            }

        }

    }

public:

    stack() {
        this->head = nullptr;
        this->tail = nullptr;
    }

    void set_head(node* value) {
        this->head = value;
    }

    void set_tail(node* value) {
        this->tail = value;
    }

    node* get_head() {
        return this->head;
    }

    node* get_tail() {
        return this->tail;
    }

    void put(int content, int priority, int operand) {

        // create a variable that will tell a setter to *put* the element
        const bool method = true;

        // set the node
        set_node(content, priority, operand, method);

    }

    void push(int content, int priority, int operand) {

        // create a variable that will tell a setter to *push* the element
        const bool method = false;

        // set the node
        set_node(content, priority, operand, method);

    }

    node* pop() {

        if (this->head == nullptr) return nullptr;

        node* popped = this->head;

        if (this->head == this->tail) {
            this->head = nullptr;
            this->tail = nullptr;
        }
        else this->head = this->head->get_next();

        popped->set_next(nullptr);
        return popped;

    }

    void print() {

        node* iterator = new node();
        if (this->head != nullptr) iterator = this->head;
        else {
            delete iterator;
            return;
        }

        while (iterator != nullptr) {
            if (iterator->is_operand()) printf("%d ", iterator->get_content());
            else printf("%c ", iterator->get_content());

            iterator = iterator->get_next();
        }

        printf("\n");
        delete iterator;

    }

    void clear() {
        node* current = this->head;
        while (current != nullptr) {
            node* next = current->get_next();
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
    }

//    ~stack() {
//        this->clear();
//        delete this;
//    }

};


#include "define.h"
#include "input.h"
#include "parse.h"


int main() {

    // create needed stacks
    auto* output = new stack();
    auto* operators = new stack();
    auto* arguments = new stack();

    // get number of formulae that the user is going to provide
    const int number_of_formulae = get_number_of_formulae();

    for (int counter = 0; counter < number_of_formulae; counter++) {

        output->clear();
        operators->clear();
        arguments->clear();

        parse_formula(operators, output);

        output->print();

        // get input
        // calculate
        // print results

    }

    // clear memory
    output->clear();
    operators->clear();
    arguments->clear();

    // delete stacks
    delete output;
    delete operators;
    delete arguments;

    return 0;

}
