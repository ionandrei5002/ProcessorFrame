#ifndef VISITOR_H
#define VISITOR_H

#include <iostream>
#include <vector>

#include "types.h"
#include "bytebuffer.h"
#include "schema.h"

class Visitor
{
protected:
    ViewByteBuffer _view;
    Type::type _type;
public:
    Visitor() {}
    virtual ~Visitor() {}
    virtual uint64_t set(const char* row, uint64_t pos) = 0;
    ViewByteBuffer& get();
    static std::unique_ptr<Visitor> builder(Node node);
    virtual void print(std::ostream& out) = 0;
    virtual bool operator !=(const std::shared_ptr<Visitor>& value) = 0;
    virtual bool operator <(const std::shared_ptr<Visitor>& value) = 0;
};

template<typename T>
class TypedVisitor: public Visitor
{
private:
    typedef typename T::c_type _val;
public:
    uint64_t set(const char* row, uint64_t pos) override
    {
        uint64_t size = sizeof(_val);
        _view = ViewByteBuffer(size, &row[pos]);

        return (pos + size);
    }
    void print(std::ostream& out) override
    {
        _val* typed_value = nullptr;
        typed_value = reinterpret_cast<_val*>(_view._data);

        out << *typed_value;
    }
    bool operator !=(const std::shared_ptr<Visitor>& value) override
    {
        _val _lv, _rv;

        _lv = *reinterpret_cast<_val*>(_view._data);
        _rv = *reinterpret_cast<_val*>(value->get()._data);

        return (_lv < _rv) || (_lv > _rv);
    }
    bool operator <(const std::shared_ptr<Visitor>& value) override
    {
        _val _lv, _rv;

        _lv = *reinterpret_cast<_val*>(_view._data);
        _rv = *reinterpret_cast<_val*>(value->get()._data);

        return (_lv < _rv);
    }
};

template<>
class TypedVisitor<StringType>: public Visitor
{
private:
    typedef typename StringType::c_type _val;
public:
    uint64_t set(const char* row, uint64_t pos) override
    {
        uint64_t size = *(reinterpret_cast<const uint64_t*>(&row[pos]));
        _view = ViewByteBuffer(size, &row[pos + sizeof(size)]);

        return (pos + sizeof(size) + size);
    }
    void print(std::ostream& out) override
    {
        out << _view;
    }
    bool operator !=(const std::shared_ptr<Visitor>& value) override
    {
        return (_view != value->get());
    }
    bool operator <(const std::shared_ptr<Visitor>& value) override
    {
        return (_view < value->get());
    }
};

typedef TypedVisitor<UInt8Type> UInt8Visitor;
typedef TypedVisitor<Int8Type> Int8Visitor;
typedef TypedVisitor<UInt16Type> UInt16Visitor;
typedef TypedVisitor<Int16Type> Int16Visitor;
typedef TypedVisitor<UInt32Type> UInt32Visitor;
typedef TypedVisitor<Int32Type> Int32Visitor;
typedef TypedVisitor<UInt64Type> UInt64Visitor;
typedef TypedVisitor<Int64Type> Int64Visitor;
typedef TypedVisitor<FloatType> FloatVisitor;
typedef TypedVisitor<DoubleType> DoubleVisitor;
typedef TypedVisitor<StringType> StringVisitor;

#endif // VISITOR_H
