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
    std::vector<std::unique_ptr<Visitor>> _visitors;
    std::vector<std::pair<std::shared_ptr<Aggregator>, std::reference_wrapper<std::unique_ptr<Visitor>>>> _inputColumns;
    void buildVisitors();
public:
    GroupBy(Table& table):_inputTable(table) { buildVisitors(); }
    Table& getResult();
    GroupBy& Group(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = _inputTable.getSchema().position(column->getColumn());
        _inputColumns.push_back(std::make_pair(column, std::ref(_visitors[pos])));
        return *this;
    }
    GroupBy& Sum(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = _inputTable.getSchema().position(column->getColumn());
        _inputColumns.push_back(std::make_pair(column, std::ref(_visitors[pos])));
        return *this;
    }
    GroupBy& Count(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = _inputTable.getSchema().position(column->getColumn());
        _inputColumns.push_back(std::make_pair(column, std::ref(_visitors[pos])));
        return *this;
    }
    GroupBy& CountDistinct(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = _inputTable.getSchema().position(column->getColumn());
        _inputColumns.push_back(std::make_pair(column, std::ref(_visitors[pos])));
        return *this;
    }
    GroupBy& Const(std::shared_ptr<Aggregator> column)
    {
        auto nullVisitor = std::unique_ptr<Visitor>(nullptr);
        _inputColumns.push_back(std::make_pair(column, std::ref(nullVisitor)));
        return *this;
    }
public:
    Schema buildOutputSchema();
    void buildOutputTable();
};

#endif // GROUPBY_H
