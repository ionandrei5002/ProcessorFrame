#ifndef ROW_H
#define ROW_H

#include <memory>
#include <memory.h>
#include <iostream>

#include <experimental/string_view>

#include "allocator/free_list_allocator.h"

class Row
{
private:
    char* _row = nullptr;
    uint64_t _size = 0;
    uint64_t _capacity = 0;
public:
    Row();
    ~Row() noexcept;
    Row(const Row& ot);
    Row& operator=(const Row& ot);
    Row(Row&& ot) noexcept;
    Row& operator=(Row&& ot) noexcept;
    void append(const char* data, uint64_t size);
    const char* buffer() const;
    uint64_t size() const;
    uint64_t capacity() const;
    bool operator()(const Row& lv, const Row& rv);
    bool operator<(const Row& ot) const;
    friend std::ostream& operator<<(std::ostream& out, const Row& ot);
private:
    void resize();
};

#endif // ROW_H
