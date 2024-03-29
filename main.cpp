#include <iostream>
#include "define.h"


class node {
private:
    int content;
    int priority;
    int arity;
    int function_id;
    bool operand;
    bool function;
    bool last;
    node* next;

public:

    node() {
        this->content = 0;
        this->priority = 0;
        this->arity = 0;
        this->function_id = 0;
        this->operand = false;
        this->function = false;
        this->last = false;
        this->next = nullptr;
    }

    void set_content(int value) {
        this->content = value;
    }

    void set_priority(int value) {
        this->priority = value;
    }

    void set_arity(int value) {
        this->arity = value;
    }

    void set_function_id(int value) {
        this->function_id = value;
    }

    void set_operand(bool value) {
        this->operand = value;
    }

    void set_function(bool value) {
        this->function = value;
    }

    void set_last(bool value) {
        this->last = value;
    }

    void set_next(node* value) {
        this->next = value;
    }

    [[nodiscard]] int get_content() const {
        return this->content;
    }

    [[nodiscard]] int get_priority() const {
        return this->priority;
    }

    [[nodiscard]] int get_arity() const {
        return this->arity;
    }

    [[nodiscard]] int get_function_id() const {
        return this->function_id;
    }

    [[nodiscard]] bool is_operand() const {
        return this->operand;
    }

    [[nodiscard]] bool is_function() const {
        return this->function;
    }

    [[nodiscard]] bool is_last() const {
        return this->last;
    }

    node* get_next() {
        return this->next;
    }

};


class stack {
private:
    node* head;
    node* tail;

    void set_node(int content, int priority, int arity, int function_id, bool operand, bool function, bool last, bool method) {

        // create a new node
        auto* symbol = new node();

        // give the node some properties
        symbol->set_content(content);
        symbol->set_priority(priority);
        symbol->set_arity(arity);
        symbol->set_function_id(function_id);
        symbol->set_operand(operand);
        symbol->set_function(function);
        symbol->set_last(last);

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

    node* get_head() {
        return this->head;
    }

    void put(int content, int priority, int arity, int function_id, bool operand, bool function, bool last) {

        // create a variable that will tell a setter to *put* the element
        const bool method = true;

        // set the node
        set_node(content, priority, arity, function_id, operand, function, last, method);

    }

    void push(int content, int priority, int arity, int function_id, bool operand, bool function, bool last) {

        // create a variable that will tell a setter to *push* the element
        const bool method = false;

        // set the node
        set_node(content, priority, arity, function_id, operand, function, last, method);

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
            else if (iterator->is_function()) {
                if ((iterator->get_function_id() == maximum || iterator->get_function_id() == minimum) && iterator->is_last()) {
                    printf("%c%d ", iterator->get_content(), iterator->get_arity());
                }
                else if (iterator->is_last()) printf("%c ", iterator->get_content());
                else printf("%c", iterator->get_content());
            }
            else printf("%c ", iterator->get_content());

//            printf(" --> (%d) ", iterator->get_arity());
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

};


#include "input.h"
#include "functions.h"
#include "parse.h"
#include "calculate.h"


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

        parse_formula(operators, output, arguments);
        output->print();

        arguments->clear();
        operators->clear();

        calculate(output);
        printf("\n");

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
