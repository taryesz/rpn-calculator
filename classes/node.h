class node {
private:

    int content;
    int priority;
    int arity;
    int function_id;
    bool operand;
    bool function;
    bool last;
    bool requires_additional_parenthesis;
    node* next;

public:

    node();

    void set_content(int value);

    void set_priority(int value);

    void set_arity(int value);

    void set_function_id(int value);

    void set_operand(bool value);

    void set_function(bool value);

    void set_last(bool value);

    void set_requires_additional_parenthesis(bool value);

    void set_next(node* value);

    [[nodiscard]] int get_content() const;

    [[nodiscard]] int get_priority() const;

    [[nodiscard]] int get_arity() const;

    [[nodiscard]] int get_function_id() const;

    [[nodiscard]] bool is_operand() const;

    [[nodiscard]] bool is_function() const;

    [[nodiscard]] bool is_last() const;

    [[nodiscard]] bool get_requires_additional_parenthesis() const;

    node* get_next();

};
