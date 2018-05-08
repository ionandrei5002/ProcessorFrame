#include "visitor.h"

ViewByteBuffer& Visitor::get()
{
    return _view;
}

std::unique_ptr<Visitor> Visitor::builder(Node node)
{
    Type::type type = node.getType();
    switch (type) {
        case Type::UINT8:
        case Type::INT8:
        case Type::UINT16:
        case Type::INT16:
        case Type::UINT32:
        case Type::INT32:
            return nullptr;
        case Type::FLOAT:
        case Type::DOUBLE:
            return std::make_unique<FloatVisitor>();
        case Type::UINT64:
        case Type::INT64:
            return std::make_unique<Int64Visitor>();
        case Type::STRING:
            return std::make_unique<StringVisitor>();
        default:
            return nullptr;
    }
}
