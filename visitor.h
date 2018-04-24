#ifndef VISITOR_H
#define VISITOR_H

#include <vector>

#include "types.h"
#include "bytebuffer.h"

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
