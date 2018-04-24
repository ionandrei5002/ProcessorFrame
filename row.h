#ifndef ROW_H
#define ROW_H

#include <memory>
#include <memory.h>
#include <iostream>

class Row
{
private:
    char* _row = nullptr;
    uint64_t _size = 0;
    uint64_t _capacity = 0;
public:
    Row();
    ~Row();
    Row(const Row& ot)
    {
        _size = ot._size;
        _row = new char[ot._size];
        memcpy(_row, ot._row, ot._size);
    }
    Row& operator=(const Row& ot)
    {
        _size = ot._size;
        _row = new char[ot._size];
        memcpy(_row, ot._row, ot._size);
        return *this;
    }
    Row(Row&& ot)
    {
        _size = ot._size;
        _row = ot._row;
        ot._row = nullptr;
    }
    Row& operator=(Row&& ot)
    {
        _size = ot._size;
        _row = ot._row;
        ot._row = nullptr;
        return *this;
    }
    void append(const char* data, uint64_t size);
    const char* buffer() const;
    uint64_t size();
    uint64_t capacity();
    friend std::ostream& operator<<(std::ostream& out, const Row& ot);
private:
    void resize();
};

#endif // ROW_H
