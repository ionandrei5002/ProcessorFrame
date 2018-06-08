#ifndef SORTER_H
#define SORTER_H

#include <vector>
#include <string>
#include <iostream>
#include <chrono>

#include "schema.h"
#include "table.h"
#include "comparator.h"

class Sorter
{
private:
    std::vector<std::string> _columns;
    std::vector<std::string> _positions;
public:
    Sorter(std::vector<std::string> columns):_columns(columns) {}
    void sort(Table& table);
};

#endif // SORTER_H
