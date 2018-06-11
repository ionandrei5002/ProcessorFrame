#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <string>
#include <unordered_set>

#include "types.h"
#include "bytebuffer.h"
#include "row.h"
#include "visitor.h"
#include "value.h"

class Aggregator
{
protected:
    std::string _column;
    Type::type _inputType;
    Type::type _outputType;
public:
    virtual ~Aggregator() {}
    std::string getColumn();
    Type::type getInputType();
    Type::type getOutputType();
    virtual void inputValue(std::unique_ptr<Visitor>& input) = 0;
    // virtual std::unique_ptr<Visitor> outputValue() = 0;
    virtual void writeValue(RawRow& row) = 0;
    virtual void reset() = 0;
    virtual void print(std::ostream& out) = 0;
};

template<typename T, typename U = T>
class None: public Aggregator
{
private:
    ViewByteBuffer _value;
    typedef typename T::c_type _type;
public:
    None(std::string column)
    {
        _column = column;
        _inputType = T::type_num;
        _outputType = U::type_num;
    }
    void inputValue(std::unique_ptr<Visitor>& input) override
    {
        _value = input->get();
    }
    // std::unique_ptr<Visitor> outputValue() override
    // {
    //     return _value;
    // }
    void writeValue(RawRow& row) override
    {
        row.append(_value._data, _value._size);
    }
    void reset() override
    {
        _value = ViewByteBuffer();
    }
    void print(std::ostream& out) override
    {
        _type* typed_value = nullptr;
        typed_value = reinterpret_cast<_type*>(_value._data);

        out << *typed_value;
    }
};

template<>
class None<StringType,StringType>: public Aggregator
{
private:
    ViewByteBuffer _value;
    typedef typename StringType::c_type _type;
public:
    None(std::string column)
    {
        _column = column;
        _inputType = StringType::type_num;
        _outputType = StringType::type_num;
    }
    void inputValue(std::unique_ptr<Visitor>& input) override
    {
        _value = input->get();
    }
    // std::unique_ptr<Visitor> outputValue() override
    // {
    //     return _value;
    // }
    void writeValue(RawRow& row) override
    {
        uint64_t size = _value._size;
        const char* data = _value._data;
        row.append(reinterpret_cast<char*>(&size), sizeof(size));
        row.append(data, size);
    }
    void reset() override
    {
        _value = ViewByteBuffer();
    }
    void print(std::ostream& out) override
    {
        out << _value;
    }
};

template<typename T, typename U = T>
class Sum: public Aggregator
{
private:
    typedef typename T::c_type _type;
    _type _value = 0;
public:
    Sum(std::string column)
    {
        _column = column;
        _inputType = T::type_num;
        _outputType = U::type_num;
    }
    void inputValue(std::unique_ptr<Visitor>& input) override
    {
        _type* this_value = nullptr;
        this_value = reinterpret_cast<_type*>(input->get()._data);
        _value += *this_value;
    }
    // std::unique_ptr<Visitor> outputValue() override
    // {
    //     ViewByteBuffer _tmp;
    //     _tmp._size = sizeof(_type);
    //     _tmp._data = reinterpret_cast<char*>(&_value);
    //     return _tmp;
    // }
    void writeValue(RawRow& row) override
    {
        ViewByteBuffer _tmp;
        _tmp._size = sizeof(_type);
        _tmp._data = reinterpret_cast<char*>(&_value);
        row.append(_tmp._data, _tmp._size);
    }
    void reset() override
    {
        _value = 0;
    }
    void print(std::ostream& out) override
    {
        out << _value;
    }
};

template<typename T, typename U = UInt64Type>
class CountDistinct: public Aggregator
{
private:
    typedef typename T::c_type _type;
    uint64_t _value = 0;
    std::unordered_set<_type> distinct;
public:
    CountDistinct(std::string column)
    {
        _column = column;
        _inputType = T::type_num;
        _outputType = U::type_num;
    }
    void inputValue(std::unique_ptr<Visitor>& input) override
    {
        _type* this_value = nullptr;
        this_value = reinterpret_cast<_type*>(input->get()._data);
        distinct.emplace(*this_value);
        _value = distinct.size();
    }
    // std::unique_ptr<Visitor> outputValue() override
    // {
    //     ViewByteBuffer _tmp;
    //     _tmp._size = sizeof(uint64_t);
    //     _tmp._data = reinterpret_cast<char*>(&_value);
    //     return _tmp;
    // }
    void writeValue(RawRow& row) override
    {
        ViewByteBuffer _tmp;
        _tmp._size = sizeof(uint64_t);
        _tmp._data = reinterpret_cast<char*>(&_value);
        row.append(_tmp._data, _tmp._size);
    }
    void reset() override
    {
        distinct.clear();
        _value = 0;
    }
    void print(std::ostream& out) override
    {
        out << _value;
    }
};

template<typename T, typename U = UInt64Type>
class Count: public Aggregator
{
private:
    typedef typename U::c_type _type;
    _type _value = 0;
public:
    Count(std::string column)
    {
        _column = column;
        _inputType = T::type_num;
        _outputType = U::type_num;
    }
    void inputValue(std::unique_ptr<Visitor>& input) override
    {
        _value ++;
    }
    // std::unique_ptr<Visitor> outputValue() override
    // {
    //     ViewByteBuffer _tmp;
    //     _tmp._size = sizeof(_type);
    //     _tmp._data = reinterpret_cast<char*>(&_value);
    //     return _tmp;
    // }
    void writeValue(RawRow& row) override
    {
        ViewByteBuffer _tmp;
        _tmp._size = sizeof(_type);
        _tmp._data = reinterpret_cast<char*>(&_value);
        row.append(_tmp._data, _tmp._size);
    }
    void reset() override
    {
        _value = 0;
    }
    void print(std::ostream& out) override
    {
        out << _value;
    }
};

template<typename T, typename U = T>
class Const: public Aggregator
{
private:
    ByteBuffer _value;
    typedef typename T::c_type _type;
public:
    Const(std::string column, std::unique_ptr<Value> value)
    {
        _column = column;
        _value = value->getBuffer();
        _inputType = T::type_num;
        _outputType = U::type_num;
    }
    void inputValue(std::unique_ptr<Visitor>& input) override {}
    // std::unique_ptr<Visitor> outputValue() override
    // {
    //     return _value;
    // }
    void writeValue(RawRow& row) override
    {
        row.append(_value._data, _value._size);
    }
    void reset() override {}
    void print(std::ostream& out) override
    {
        _type* typed_value = nullptr;
        typed_value = reinterpret_cast<_type*>(_value._data);

        out << *typed_value;
    }
};

template<>
class Const<StringType,StringType>: public Aggregator
{
private:
    ByteBuffer _value;
    typedef typename StringType::c_type _type;
public:
    Const(std::string column, std::unique_ptr<Value> value)
    {
        _column = column;
        _value = value->getBuffer();
        _inputType = StringType::type_num;
        _outputType = StringType::type_num;
    }
    void inputValue(std::unique_ptr<Visitor>& input) override {}
    // std::unique_ptr<Visitor> outputValue() override
    // {
    //     return _value;
    // }
    void writeValue(RawRow& row) override
    {
        uint64_t size = _value._size;
        const char* data = _value._data;
        row.append(reinterpret_cast<char*>(&size), sizeof(size));
        row.append(data, size);
    }
    void reset() override {}
    void print(std::ostream& out) override
    {
        out << _value;
    }
};

#endif // AGGREGATOR_H
