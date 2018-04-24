#include "bytebuffer.h"

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
