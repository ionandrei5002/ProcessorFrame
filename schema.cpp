#include "schema.h"

Type::type Node::getType()
{
    return this->_type;
}

bool Node::isNullable()
{
    return this->_isNullable;
}

bool Node::operator ==(const Node& node)
{
    return this->_name == node._name;
}

bool Node::operator ==(const std::string name)
{
    return this->_name == name;
}

std::ostream& operator <<(std::ostream& out, const Node& node)
{
    out << node._name << " " << node._type << " " << " " << node._isNullable;

    return out;
}

uint64_t Schema::size()
{
    return this->_nodes.size();
}

Schema& Schema::push(Node node)
{
    for(uint64_t i = 0; i < _nodes.size(); i++)
    {
        Node value = _nodes.at(i);
        if (value == node) {
            throw;
        }
    }

    _nodes.push_back(node);

    return *this;
}

Node Schema::peek(uint64_t pos)
{
    return _nodes.at(pos);
}

uint64_t Schema::position(std::string name)
{
    for(uint64_t i = 0; i < _nodes.size(); i++)
    {
        Node node = _nodes.at(i);
        if (node == name) {
            return i;
        }
    }

    throw;
}

std::ostream& operator <<(std::ostream& out, const Schema& schema)
{
    for(uint64_t i = 0; i < schema._nodes.size(); i++) {
        Node node = schema._nodes.at(i);
        out << node << std::endl;
    }

    return out;
}
