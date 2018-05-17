#include "bytebuffer.h"

ByteBuffer::ByteBuffer(uint64_t size, const char* data):_size(size)
{
    _data = new char[size];
    memcpy(_data, data, size);
}

ByteBuffer::ByteBuffer(const ByteBuffer& ot)
{
    _size = ot._size;
    _data = new char[ot._size];
    memcpy(_data, ot._data, ot._size);
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& ot)
{
    _size = ot._size;
    _data = new char[ot._size];
    memcpy(_data, ot._data, ot._size);
    return *this;
}

ByteBuffer::ByteBuffer(ByteBuffer&& ot)
{
    _size = ot._size;
    _data = ot._data;
    ot._data = nullptr;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& ot)
{
    _size = ot._size;
    _data = ot._data;
    ot._data = nullptr;
    return *this;
}

ByteBuffer::~ByteBuffer()
{
    if (_data != nullptr)
    {
        delete[] _data;
        _data = nullptr;
    }
    _size = 0;
}

bool ByteBuffer::operator ==(const ByteBuffer& value)
{
    return std::experimental::string_view(this->_data, this->_size) == std::experimental::string_view(value._data, value._size);
}

bool ByteBuffer::operator !=(const ByteBuffer& value)
{
    return std::experimental::string_view(this->_data, this->_size) != std::experimental::string_view(value._data, value._size);
}

bool ByteBuffer::operator <(const ByteBuffer& value)
{
    return std::experimental::string_view(this->_data, this->_size) < std::experimental::string_view(value._data, value._size);
}

bool ByteBuffer::operator >(const ByteBuffer& value)
{
    return std::experimental::string_view(this->_data, this->_size) > std::experimental::string_view(value._data, value._size);
}

std::ostream& operator<<(std::ostream& out, const ByteBuffer& ot)
{
    for(uint64_t i = 0; i < ot._size; ++i)
    {
        out << ot._data[i];
    }

    return out;
}

ViewByteBuffer::ViewByteBuffer(uint64_t size, const char* data):_size(size),_data(const_cast<char*>(data)){}

ViewByteBuffer::ViewByteBuffer(const ViewByteBuffer& ot)
{
    _size = ot._size;
    _data = ot._data;
}

ViewByteBuffer& ViewByteBuffer::operator=(const ViewByteBuffer& ot)
{
    _size = ot._size;
    _data = ot._data;
    return *this;
}

ViewByteBuffer::ViewByteBuffer(ViewByteBuffer&& ot)
{
    _size = ot._size;
    _data = ot._data;
    ot._data = nullptr;
}

ViewByteBuffer& ViewByteBuffer::operator=(ViewByteBuffer&& ot)
{
    _size = ot._size;
    _data = ot._data;
    ot._data = nullptr;
    return *this;
}

ViewByteBuffer::~ViewByteBuffer()
{
    _data = nullptr;
    _size = 0;
}

bool operator<(const ByteBuffer& lv, const ByteBuffer& rv)
{
    return std::experimental::string_view(lv._data, lv._size) < std::experimental::string_view(rv._data, rv._size);
}

bool ViewByteBuffer::operator ==(const ViewByteBuffer& value)
{
    return std::experimental::string_view(this->_data, this->_size) == std::experimental::string_view(value._data, value._size);
}

bool ViewByteBuffer::operator !=(const ViewByteBuffer& value)
{
    return std::experimental::string_view(this->_data, this->_size) != std::experimental::string_view(value._data, value._size);
}

bool ViewByteBuffer::operator <(const ViewByteBuffer& value)
{
    return std::experimental::string_view(this->_data, this->_size) < std::experimental::string_view(value._data, value._size);
}

bool ViewByteBuffer::operator >(const ViewByteBuffer& value)
{
    return std::experimental::string_view(this->_data, this->_size) > std::experimental::string_view(value._data, value._size);
}

std::ostream& operator<<(std::ostream& out, const ViewByteBuffer& ot)
{
    for(uint64_t i = 0; i < ot._size; ++i)
    {
        out << ot._data[i];
    }

    return out;
}
bool operator<(const ViewByteBuffer& lv, const ViewByteBuffer& rv)
{
    return std::experimental::string_view(lv._data, lv._size) < std::experimental::string_view(rv._data, rv._size);
}

ByteBuffer::ByteBuffer(ViewByteBuffer& ot)
{
    _size = ot._size;
    _data = new char[ot._size];
    memcpy(_data, ot._data, ot._size);
}
ViewByteBuffer::ViewByteBuffer(ByteBuffer& ot)
{
    _size = ot._size;
    _data = ot._data;
}
