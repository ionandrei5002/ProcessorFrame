#include "row.h"

Row::Row()
{
    _row = reinterpret_cast<char*>(free_list_alloca.allocate(64));
    _capacity = 64;
    _size = 0;
}

Row::Row(const Row& ot)
{
    _size = ot._size;
    _row = reinterpret_cast<char*>(free_list_alloca.allocate(ot._size));
    memcpy(_row, ot._row, ot._size);
}

Row& Row::operator=(const Row& ot)
{
    _size = ot._size;
    _row = reinterpret_cast<char*>(free_list_alloca.allocate(ot._size));
    memcpy(_row, ot._row, ot._size);
    return *this;
}

Row::Row(Row&& ot) noexcept
{
    _size = ot._size;
    _row = ot._row;
    ot._row = nullptr;
}

Row& Row::operator=(Row&& ot) noexcept
{
    _size = ot._size;
    _row = ot._row;
    ot._row = nullptr;
    return *this;
}

Row::~Row() noexcept
{
    free_list_alloca.deallocate(_row, _capacity);
    _row = nullptr;
    _capacity = 0;
    _size = 0;
}

uint64_t Row::capacity() const {
    return _capacity;
}

uint64_t Row::size() const {
    return _size;
}

void Row::resize()
{
    uint64_t newCapacity = _capacity * 2;
    char* newData = reinterpret_cast<char*>(free_list_alloca.allocate(newCapacity));
    memcpy(newData, _row, _size);

    free_list_alloca.deallocate(_row, _capacity);
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

bool Row::operator()(const Row& lv, const Row& rv)
{
    return std::experimental::string_view(lv.buffer(), lv._size) < std::experimental::string_view(rv.buffer(), rv._size);
}

bool Row::operator<(const Row& ot) const
{
    return std::experimental::string_view(this->buffer(), this->_size) < std::experimental::string_view(ot.buffer(), ot._size);
}

std::ostream& operator<<(std::ostream& out, const Row& ot)
{
    for(uint64_t i = 0; i < ot._size; ++i)
    {
        out << ot._row[i];
    }

    return out;
}
