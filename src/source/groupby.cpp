#include "groupby.h"

void GroupBy::buildVisitors()
{
    Schema schema = _inputTable.getSchema();

    for(uint64_t pos = 0; pos < schema.size(); pos++)
    {
        _visitors.push_back(Visitor::builder(schema.peek(pos)));
    }
}

Schema GroupBy::buildOutputSchema()
{
    Schema schema;

    for(auto it = _inputColumns.begin(); it != _inputColumns.end(); ++it)
    {
        auto pair = (*it);
        std::string column = pair.first->getColumn();
        Type::type type = pair.first->getOutputType();
        schema.push(Node(column, type));
    }

    std::cout << schema << std::endl;

    return schema;
}

void GroupBy::buildOutputTable()
{
    Schema schema = buildOutputSchema();
    _outputTable = Table(schema);
}

Table& GroupBy::getResult()
{
    RawRow rawRow;

    for(auto jt = _inputTable.getRows().begin(); jt != _inputTable.getRows().end(); ++jt)
    {
        Row& row = (*jt);
        uint64_t pos = 0;
        for(uint64_t i = 0; i < _visitors.size(); i++)
        {
            pos = _visitors[i]->set(row.buffer(), pos);
        }
        
        for(auto it = _inputColumns.begin(); it != _inputColumns.end(); ++it)
        {
            auto aggregate = (*it).first;
            auto& visitor = (*it).second;
            aggregate->inputValue(visitor);
            aggregate->writeValue(rawRow);
            aggregate->reset();
        }

        _outputTable.getRows().emplace_back(Row(rawRow.buffer(), rawRow.size()));
        rawRow.reset();
    }

    return _outputTable;
}