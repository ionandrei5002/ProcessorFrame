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
    std::vector<std::pair<std::shared_ptr<Aggregator>, std::reference_wrapper<std::unique_ptr<Visitor>>>> _inputColumns;
    std::vector<std::unique_ptr<Visitor>> _comparationColumns;
    std::vector<uint64_t> _group;
    void buildComparationColumns();
public:
    GroupBy(Table& table):_inputTable(table) {}
    Table& getResult();
    GroupBy& Group(std::shared_ptr<Aggregator> column);
    GroupBy& Aggregation(std::shared_ptr<Aggregator> column);
    GroupBy& Const(std::shared_ptr<Aggregator> column);
public:
    Schema buildOutputSchema();
    void buildOutputTable();
};

#endif // GROUPBY_H
