#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <memory>
#include <memory.h>
#include <iostream>
#include <experimental/string_view>

class ViewByteBuffer;

class ByteBuffer
{
public:
    uint64_t _size = 0;
    char* _data = nullptr;
public:
    ByteBuffer():_size(0),_data(nullptr) {}
    ByteBuffer(uint64_t size, const char* data);
    ByteBuffer(const ByteBuffer& ot);
    ByteBuffer& operator=(const ByteBuffer& ot);
    ByteBuffer(ByteBuffer&& ot);
    ByteBuffer& operator=(ByteBuffer&& ot);
    ByteBuffer(ViewByteBuffer& ot);
    ~ByteBuffer();
    bool operator ==(const ByteBuffer& value);
    bool operator !=(const ByteBuffer& value);
    bool operator <(const ByteBuffer& value);
    bool operator >(const ByteBuffer& value);
    friend std::ostream& operator<<(std::ostream& out, const ByteBuffer& ot);
    friend bool operator<(const ByteBuffer& lv, const ByteBuffer& rv);
};

class ViewByteBuffer
{
public:
    uint64_t _size = 0;
    char* _data = nullptr;
public:
    ViewByteBuffer():_size(0),_data(nullptr) {}
    ViewByteBuffer(uint64_t size, const char* data);
    ViewByteBuffer(const ViewByteBuffer& ot);
    ViewByteBuffer& operator=(const ViewByteBuffer& ot);
    ViewByteBuffer(ViewByteBuffer&& ot);
    ViewByteBuffer& operator=(ViewByteBuffer&& ot);
    ViewByteBuffer(ByteBuffer& ot);
    ~ViewByteBuffer();
    bool operator ==(const ViewByteBuffer& value);
    bool operator !=(const ViewByteBuffer& value);
    bool operator <(const ViewByteBuffer& value);
    bool operator >(const ViewByteBuffer& value);
    friend std::ostream& operator<<(std::ostream& out, const ViewByteBuffer& ot);
    friend bool operator<(const ViewByteBuffer& lv, const ViewByteBuffer& rv);
};

#endif // BYTEBUFFER_H
