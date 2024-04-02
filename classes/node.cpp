#include "node.h"


node::node() {
    this->content = 0;
    this->priority = 0;
    this->arity = 0;
    this->function_id = 0;
    this->operand = false;
    this->function = false;
    this->last = false;
    this->requires_additional_parenthesis = false;
    this->next = nullptr;
}

void node::set_content(int value) {
    this->content = value;
}

void node::set_priority(int value) {
    this->priority = value;
}

void node::set_arity(int value) {
    this->arity = value;
}

void node::set_function_id(int value) {
    this->function_id = value;
}

void node::set_operand(bool value) {
    this->operand = value;
}

void node::set_function(bool value) {
    this->function = value;
}

void node::set_last(bool value) {
    this->last = value;
}

void node::set_requires_additional_parenthesis(bool value) {
    this->requires_additional_parenthesis = value;
}

void node::set_next(node* value) {
    this->next = value;
}

[[nodiscard]] int node::get_content() const {
    return this->content;
}

[[nodiscard]] int node::get_priority() const {
    return this->priority;
}

[[nodiscard]] int node::get_arity() const {
    return this->arity;
}

[[nodiscard]] int node::get_function_id() const {
    return this->function_id;
}

[[nodiscard]] bool node::is_operand() const {
    return this->operand;
}

[[nodiscard]] bool node::is_function() const {
    return this->function;
}

[[nodiscard]] bool node::is_last() const {
    return this->last;
}

[[nodiscard]] bool node::get_requires_additional_parenthesis() const {
    return this->requires_additional_parenthesis;
}

node* node::get_next() {
    return this->next;
}
