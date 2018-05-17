#ifndef VALUE_H
#define VALUE_H

#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_numeric.hpp>

#include "types.h"
#include "bytebuffer.h"

class Value
{
protected:
    ByteBuffer _value;
    Type::type _type;
public:
    virtual ~Value() {}
    Type::type getType();
    ByteBuffer& getBuffer();
    virtual void print(std::ostream& out) const = 0;
    virtual bool operator<(std::unique_ptr<Value>& ot) = 0;
    virtual bool operator>(std::unique_ptr<Value>& ot) = 0;
    virtual bool operator==(std::unique_ptr<Value>& ot) = 0;
    virtual bool operator!=(std::unique_ptr<Value>& ot) = 0;
};

template<typename T>
class TypedValue: public Value
{
private:
    typedef typename T::c_type _val;
public:
    TypedValue(ByteBuffer value)
    {
        _type = T::type_num;
        _value = std::move(value);
    }
    TypedValue(ViewByteBuffer value)
    {
        _type = T::type_num;
        _value = std::move(ByteBuffer(value));
    }
    void print(std::ostream& out) const override
    {
        _val* typed_value = nullptr;
        typed_value = reinterpret_cast<_val*>(_value._data);

        out << *typed_value;
    }
    bool operator<(std::unique_ptr<Value>& ot) override
    {
        _val* this_value = nullptr;
        this_value = reinterpret_cast<_val*>(_value._data);

        _val* ot_value = nullptr;
        ot_value = reinterpret_cast<_val*>(ot->getBuffer()._data);

        return *this_value < *ot_value;
    }
    bool operator>(std::unique_ptr<Value>& ot) override
    {
        _val* this_value = nullptr;
        this_value = reinterpret_cast<_val*>(_value._data);

        _val* ot_value = nullptr;
        ot_value = reinterpret_cast<_val*>(ot->getBuffer()._data);

        return *this_value > *ot_value;
    }
    bool operator==(std::unique_ptr<Value>& ot) override
    {
        _val* this_value = nullptr;
        this_value = reinterpret_cast<_val*>(_value._data);

        _val* ot_value = nullptr;
        ot_value = reinterpret_cast<_val*>(ot->getBuffer()._data);

        return *this_value == *ot_value;
    }
    bool operator!=(std::unique_ptr<Value>& ot) override
    {
        _val* this_value = nullptr;
        this_value = reinterpret_cast<_val*>(_value._data);

        _val* ot_value = nullptr;
        ot_value = reinterpret_cast<_val*>(ot->getBuffer()._data);

        return *this_value != *ot_value;
    }
};

template<>
class TypedValue<StringType>: public Value
{
private:
    typedef typename StringType::c_type _val;
public:
    TypedValue(ByteBuffer value)
    {
        _type = StringType::type_num;
        _value = std::move(value);
    }
    TypedValue(ViewByteBuffer value)
    {
        _type = StringType::type_num;
        _value = value;
    }
    void print(std::ostream& out) const override
    {
        out << _value;
    }
    bool operator<(std::unique_ptr<Value>& ot) override
    {
        return _value < ot->getBuffer();
    }
    bool operator>(std::unique_ptr<Value>& ot) override
    {
        return _value > ot->getBuffer();
    }
    bool operator==(std::unique_ptr<Value>& ot) override
    {
        return _value == ot->getBuffer();
    }
    bool operator!=(std::unique_ptr<Value>& ot) override
    {
        return _value != ot->getBuffer();
    }
};

typedef TypedValue<UInt8Type> UInt8Value;
typedef TypedValue<Int8Type> Int8Value;
typedef TypedValue<UInt16Type> UInt16Value;
typedef TypedValue<Int16Type> Int16Value;
typedef TypedValue<UInt32Type> UInt32Value;
typedef TypedValue<Int32Type> Int32Value;
typedef TypedValue<UInt64Type> UInt64Value;
typedef TypedValue<Int64Type> Int64Value;
typedef TypedValue<FloatType> FloatValue;
typedef TypedValue<DoubleType> DoubleValue;
typedef TypedValue<StringType> StringValue;

std::unique_ptr<Value> MakeUInt8Value(std::string& value);
std::unique_ptr<Value> MakeUInt8Value(uint8_t value);
std::unique_ptr<Value> MakeUInt16Value(std::string& value);
std::unique_ptr<Value> MakeUInt16Value(uint16_t value);
std::unique_ptr<Value> MakeUInt32Value(std::string& value);
std::unique_ptr<Value> MakeUInt32Value(uint32_t value);
std::unique_ptr<Value> MakeUInt64Value(std::string& value);
std::unique_ptr<Value> MakeUInt64Value(uint64_t value);

std::unique_ptr<Value> MakeInt8Value(std::string& value);
std::unique_ptr<Value> MakeInt8Value(int8_t value);
std::unique_ptr<Value> MakeInt16Value(std::string& value);
std::unique_ptr<Value> MakeInt16Value(int16_t value);
std::unique_ptr<Value> MakeInt32Value(std::string& value);
std::unique_ptr<Value> MakeInt32Value(int32_t value);
std::unique_ptr<Value> MakeInt64Value(std::string& value);
std::unique_ptr<Value> MakeInt64Value(int64_t value);

std::unique_ptr<Value> MakeStringValue(std::string& value);

#endif // VALUE_H
