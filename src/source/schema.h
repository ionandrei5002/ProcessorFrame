#ifndef SCHEMA_H
#define SCHEMA_H

#include <iostream>
#include <string>
#include <vector>

#include "types.h"

class Node {
private:
    std::string _name;
    Type::type _type;
    bool _isNullable = false;
public:
    Node(std::string name, Type::type type)
        :_name(name),_type(type),_isNullable(false) {}
    Node(std::string name, Type::type type, bool isNullable)
        :_name(name),_type(type),_isNullable(isNullable) {}
    Type::type getType();
    bool isNullable();
    bool operator ==(const Node& node);
    bool operator ==(const std::string name);
    friend std::ostream& operator <<(std::ostream& out, const Node& node);
};

class Schema {
private:
    std::vector<Node> _nodes;
public:
    Schema():_nodes(std::vector<Node>()) {}
    uint64_t size();
    Schema& push(Node node);
    Node peek(uint64_t pos);
    uint64_t position(std::string name);
    friend std::ostream& operator <<(std::ostream& out, const Schema& schema);
};

#endif // SCHEMA_H
