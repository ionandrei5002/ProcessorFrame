#include "groupby.h"

GroupBy& GroupBy::Group(std::shared_ptr<Aggregator> column)
{
    std::vector<std::unique_ptr<Visitor>>& _visitors = _inputTable.getVisitors();
    uint64_t pos = _inputTable.getSchema().position(column->getColumn());
    _inputColumns.push_back(std::make_pair(column, std::ref(_visitors[pos])));
    _group.push_back(pos);
    return *this;
}
GroupBy& GroupBy::Aggregation(std::shared_ptr<Aggregator> column)
{
    std::vector<std::unique_ptr<Visitor>>& _visitors = _inputTable.getVisitors();
    uint64_t pos = _inputTable.getSchema().position(column->getColumn());
    _inputColumns.push_back(std::make_pair(column, std::ref(_visitors[pos])));
    return *this;
}
GroupBy& GroupBy::Const(std::shared_ptr<Aggregator> column)
{
    auto nullVisitor = std::unique_ptr<Visitor>(nullptr);
    _inputColumns.push_back(std::make_pair(column, std::ref(nullVisitor)));
    return *this;
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

void GroupBy::buildComparationColumns()
{
    Schema schema = _inputTable.getSchema();

    for(uint64_t pos = 0; pos < schema.size(); pos++)
    {
        _comparationColumns.push_back(Visitor::builder(schema.peek(pos)));
    }
}

Table& GroupBy::getResult()
{
    RawRow rawRow;
    std::vector<std::unique_ptr<Visitor>>& _visitors = _inputTable.getVisitors();
    buildComparationColumns();

    std::vector<Row>& rows = _inputTable.getRows();

    // Init the aggregation & Init the comparation row
    {
        Row& row = rows[0];
        uint64_t pos = 0;
        for(uint64_t i = 0; i < _visitors.size(); i++)
        {
            pos = _visitors[i]->set(row.buffer(), pos);
        }
        pos = 0;
        for(uint64_t i = 0; i < _comparationColumns.size(); i++)
        {
            pos = _comparationColumns[i]->set(row.buffer(), pos);
        }

        for(auto it = _inputColumns.begin(); it != _inputColumns.end(); ++it)
        {
            auto aggregate = (*it).first;
            auto& visitor = (*it).second;
            aggregate->inputValue(visitor);
        }
    }

    bool flag = true;
    uint64_t iterations = 0;
    for(auto jt = rows.begin() + 1; jt != rows.end(); ++jt)
    {
        Row& row = (*jt);
        uint64_t pos = 0;
        for(uint64_t i = 0; i < _visitors.size(); i++)
        {
            pos = _visitors[i]->set(row.buffer(), pos);
        }

        for(uint64_t i = 0; i < _group.size(); i++)
        {
            uint64_t val = _group[i];
            if (_visitors[val]->get() != _comparationColumns[val]->get())
            {
                flag = false;
                break;
            } else {
                flag = true;
            }
        }
        
        if (flag == true)
        {
            for(auto it = _inputColumns.begin(); it != _inputColumns.end(); ++it)
            {
                auto aggregate = (*it).first;
                auto& visitor = (*it).second;
                aggregate->inputValue(visitor);
            }
        }

        if (flag == false)
        {
            for(auto it = _inputColumns.begin(); it != _inputColumns.end(); ++it)
            {
                auto aggregate = (*it).first;
                auto& visitor = (*it).second;
                aggregate->writeValue(rawRow);
                aggregate->reset();
                aggregate->inputValue(visitor);
            }

            _outputTable.getRows().emplace_back(Row(rawRow.buffer(), rawRow.size()));
            rawRow.reset();

            pos = 0;
            for(uint64_t i = 0; i < _comparationColumns.size(); i++)
            {
                pos = _comparationColumns[i]->set(row.buffer(), pos);
            }
        }

        iterations++;
    }

    std::cout << "iters : " << iterations << std::endl;

    for(auto it = _inputColumns.begin(); it != _inputColumns.end(); ++it)
    {
        auto aggregate = (*it).first;
        aggregate->writeValue(rawRow);
        aggregate->reset();
    }

    _outputTable.getRows().emplace_back(Row(rawRow.buffer(), rawRow.size()));
    rawRow.reset();

    return _outputTable;
}