#include "value.h"

Type::type Value::getType()
{
    return _type;
}

ByteBuffer& Value::getBuffer()
{
    return _value;
}

std::unique_ptr<Value> MakeUInt32Value(std::string value)
{
    uint32_t cast_value = 0;
    boost::spirit::qi::parse(value.begin(), value.end(), boost::spirit::qi::uint_, cast_value);
    ByteBuffer buffer(sizeof(uint32_t), reinterpret_cast<char*>(&cast_value));
    return std::make_unique<UInt32Value>(buffer);
}
std::unique_ptr<Value> MakeUInt32Value(uint32_t value)
{
    ByteBuffer buffer(sizeof(uint32_t), reinterpret_cast<char*>(&value));
    return std::make_unique<UInt32Value>(buffer);
}
std::unique_ptr<Value> MakeInt32Value(std::string value)
{
    int32_t cast_value = 0;
    boost::spirit::qi::parse(value.begin(), value.end(), boost::spirit::qi::int_, cast_value);
    ByteBuffer buffer(sizeof(int32_t), reinterpret_cast<char*>(&cast_value));
    return std::make_unique<Int32Value>(buffer);
}
std::unique_ptr<Value> MakeInt32Value(int32_t value)
{
    ByteBuffer buffer(sizeof(int32_t), reinterpret_cast<char*>(&value));
    return std::make_unique<Int32Value>(buffer);
}

std::unique_ptr<Value> MakeUInt64Value(std::string value)
{
    uint64_t cast_value = 0;
    boost::spirit::qi::parse(value.begin(), value.end(), boost::spirit::qi::ulong_, cast_value);
    ByteBuffer buffer(sizeof(uint64_t), reinterpret_cast<char*>(&cast_value));
    return std::make_unique<UInt64Value>(buffer);
}
std::unique_ptr<Value> MakeUInt64Value(uint64_t value)
{
    ByteBuffer buffer(sizeof(uint64_t), reinterpret_cast<char*>(&value));
    return std::make_unique<UInt64Value>(buffer);
}
std::unique_ptr<Value> MakeInt64Value(std::string value)
{
    int64_t cast_value = 0;
    boost::spirit::qi::parse(value.begin(), value.end(), boost::spirit::qi::long_, cast_value);
    ByteBuffer buffer(sizeof(int64_t), reinterpret_cast<char*>(&cast_value));
    return std::make_unique<Int64Value>(buffer);
}
std::unique_ptr<Value> MakeInt64Value(int64_t value)
{
    ByteBuffer buffer(sizeof(int64_t), reinterpret_cast<char*>(&value));
    return std::make_unique<Int64Value>(buffer);
}

std::unique_ptr<Value> MakeStringValue(std::string value)
{
    ByteBuffer buffer(value.size(), value.data());
    return std::make_unique<StringValue>(buffer);
}
