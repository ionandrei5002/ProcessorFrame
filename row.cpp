#include "row.h"

Row::Row()
{
    _row = new char[64];
    _capacity = 64;
    _size = 0;
}

Row::~Row()
{
    delete[] _row;
    _row = nullptr;
    _capacity = 0;
    _size = 0;
}

uint64_t Row::capacity() {
    return _capacity;
}

uint64_t Row::size() {
    return _size;
}

void Row::resize()
{
    uint64_t newCapacity = _capacity * 2;
    char* newData = new char[newCapacity];
    memcpy(newData, _row, _size);

    delete[] _row;
    _row = nullptr;

    _row = newData;
    _capacity = newCapacity;
}

void Row::append(const char* data, uint64_t size)
{
    uint64_t bytesLeft = _capacity - _size;

    if (bytesLeft < size)
    {
        resize();
    }

    memcpy(&_row[_size], data, size);
    _size += size;
}

const char* Row::buffer() const {
    return _row;
}
std::ostream& operator<<(std::ostream& out, const Row& ot)
{
    for(uint64_t i = 0; i < ot._size; ++i)
    {
        out << ot._row[i];
    }

    return out;
}
