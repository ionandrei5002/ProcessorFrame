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
public:
    Row(const char* row, uint64_t size);
    ~Row() noexcept;
    Row(const Row& ot);
    Row& operator=(const Row& ot);
    Row(Row&& ot) noexcept;
    Row& operator=(Row&& ot) noexcept;
    const char* buffer() const;
    uint64_t size() const;
    bool operator()(const Row& lv, const Row& rv);
    bool operator<(const Row& ot) const;
    friend std::ostream& operator<<(std::ostream& out, const Row& ot);
};

class RawRow
{
private:
    char* _row = nullptr;
    uint64_t _size = 0;
    uint64_t _capacity = 0;
public:
    RawRow();
    ~RawRow() noexcept;
    RawRow(const RawRow& ot);
    RawRow& operator=(const RawRow& ot);
    RawRow(RawRow&& ot) noexcept;
    RawRow& operator=(RawRow&& ot) noexcept;
    void append(const char* data, uint64_t size);
    const char* buffer() const;
    uint64_t size() const;
    uint64_t capacity() const;
    void reset();
    bool operator()(const RawRow& lv, const RawRow& rv);
    bool operator<(const RawRow& ot) const;
    friend std::ostream& operator<<(std::ostream& out, const RawRow& ot);
private:
    void resize();
};

#endif // ROW_H
