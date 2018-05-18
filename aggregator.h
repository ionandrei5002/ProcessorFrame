#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include "types.h"
#include "bytebuffer.h"

class Aggregator
{
protected:
    Type::type _inputType;
    Type::type _outputType;
public:
    virtual ~Aggregator() {}
    Type::type getInputType();
    Type::type getOutputType();
    virtual void inputValue(ViewByteBuffer input) = 0;
    virtual ViewByteBuffer outputValue() = 0;
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
    None()
    {
        _inputType = T::type_num;
        _outputType = U::type_num;
    }
    void inputValue(ViewByteBuffer input) override
    {
        _value = input;
    }
    ViewByteBuffer outputValue() override
    {
        return _value;
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
    None()
    {
        _inputType = StringType::type_num;
        _outputType = StringType::type_num;
    }
    void inputValue(ViewByteBuffer input) override
    {
        _value = input;
    }
    ViewByteBuffer outputValue() override
    {
        return _value;
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
    Sum()
    {
        _inputType = T::type_num;
        _outputType = U::type_num;
    }
    void inputValue(ViewByteBuffer input) override
    {
        _type* this_value = nullptr;
        this_value = reinterpret_cast<_type*>(input._data);
        _value += *this_value;
    }
    ViewByteBuffer outputValue() override
    {
        ViewByteBuffer _tmp;
        _tmp._size = sizeof(_type);
        _tmp._data = reinterpret_cast<char*>(&_value);
        return _tmp;
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
class Count: public Aggregator
{
private:
    typedef typename U::c_type _type;
    _type _value = 0;
public:
    Count()
    {
        _inputType = T::type_num;
        _outputType = U::type_num;
    }
    void inputValue(ViewByteBuffer input) override
    {
        _value ++;
    }
    ViewByteBuffer outputValue() override
    {
        ViewByteBuffer _tmp;
        _tmp._size = sizeof(_type);
        _tmp._data = reinterpret_cast<char*>(&_value);
        return _tmp;
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

#endif // AGGREGATOR_H
