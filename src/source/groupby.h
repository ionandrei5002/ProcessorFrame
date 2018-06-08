#ifndef GROUPBY_H
#define GROUPBY_H

#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <unordered_map>

#include "schema.h"
#include "table.h"
#include "aggregator.h"
#include "visitor.h"
#include "sorter.h"

class GroupBy
{
private:
    Table& _inputTable;
    Table _outputTable;
    std::vector<std::pair<uint64_t, std::shared_ptr<Aggregator>>> _inputColumns;
    std::vector<uint64_t> _outputColumns;
public:
    GroupBy(Table& table):_inputTable(table) {}
    Table& getResult();
    GroupBy& Group(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = _inputTable.getSchema().position(column->getColumn());
        _inputColumns.push_back(std::make_pair(pos, column));
        return *this;
    }
    GroupBy& Sum(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = _inputTable.getSchema().position(column->getColumn());
        _inputColumns.push_back(std::make_pair(pos, column));
        return *this;
    }
    GroupBy& Count(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = _inputTable.getSchema().position(column->getColumn());
        _inputColumns.push_back(std::make_pair(pos, column));
        return *this;
    }
    GroupBy& CountDistinct(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = _inputTable.getSchema().position(column->getColumn());
        _inputColumns.push_back(std::make_pair(pos, column));
        return *this;
    }
    GroupBy& Const(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = 9999;
        _inputColumns.push_back(std::make_pair(pos, column));
        return *this;
    }
public:
    Schema buildOutputSchema();
    void buildOutputTable();
};

#endif // GROUPBY_H
