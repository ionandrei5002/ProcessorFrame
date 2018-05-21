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

class GroupBy
{
private:
    Table& _table;
    Table _result;
    std::vector<uint64_t> _group;
    std::unordered_map<uint64_t, std::shared_ptr<Aggregator>> _operations;
    std::vector<uint64_t> _positions;
public:
    GroupBy(Table& table):_table(table) {}
    Table& getResult();
    GroupBy& Group(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = _table.getSchema().position(column->getColumn());
        _group.push_back(pos);
        _operations.emplace(pos, column);
        return *this;
    }
    GroupBy& Sum(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = _table.getSchema().position(column->getColumn());
        _operations.emplace(pos, column);
        return *this;
    }
    GroupBy& Count(std::shared_ptr<Aggregator> column)
    {
        uint64_t pos = _table.getSchema().position(column->getColumn());
        _operations.emplace(pos, column);
        return *this;
    }
private:
    void operationsPosition();
    Schema buildResultSchema();
    void buildResultTable();
};

#endif // GROUPBY_H
