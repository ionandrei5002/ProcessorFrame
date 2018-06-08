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
            return std::make_unique<UInt8Visitor>();
        case Type::INT8:
            return std::make_unique<Int8Visitor>();
        case Type::UINT16:
            return std::make_unique<UInt16Visitor>();
        case Type::INT16:
            return std::make_unique<Int16Visitor>();
        case Type::UINT32:
            return std::make_unique<UInt32Visitor>();
        case Type::INT32:
            return std::make_unique<Int32Visitor>();
        case Type::FLOAT:
            return std::make_unique<FloatVisitor>();
        case Type::DOUBLE:
            return std::make_unique<DoubleVisitor>();
        case Type::UINT64:
            return std::make_unique<UInt64Visitor>();
        case Type::INT64:
            return std::make_unique<Int64Visitor>();
        case Type::STRING:
            return std::make_unique<StringVisitor>();
        default:
            return nullptr;
    }
}
