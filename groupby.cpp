#include "groupby.h"

Table& GroupBy::getResult()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();

    buildResultTable();

    std::vector<std::unique_ptr<Visitor>> current_visitors;
    std::vector<std::unique_ptr<Visitor>> prev_visitors;
    for(uint64_t i = 0; i < _table.getSchema().size(); i++)
    {
        current_visitors.push_back(Visitor::builder(_table.getSchema().peek(i)));
        prev_visitors.push_back(Visitor::builder(_table.getSchema().peek(i)));
    }

    {
        auto prev = _table.getRows().begin();

        Row& row = (*prev);
        uint64_t pos = 0;
        for(uint64_t i = 0; i < prev_visitors.size(); i++)
        {
            pos = prev_visitors[i]->set(row.buffer(), pos);
        }
        for(auto pt = _positions.begin(); pt != _positions.end(); ++pt)
        {
            _operations[*pt]->inputValue(prev_visitors[*pt]->get());
        }
    }

    for(auto jt = (_table.getRows().begin() + 1); jt != _table.getRows().end(); ++jt)
    {
        Row& row = (*jt);
        uint64_t pos = 0;
        for(uint64_t i = 0; i < current_visitors.size(); i++)
        {
            pos = current_visitors[i]->set(row.buffer(), pos);
        }

        for(auto pt = _group.begin(); pt != _group.end(); ++pt)
        {
            if (current_visitors[*pt]->get() != prev_visitors[*pt]->get())
            {
                RawRow raw_row;
                for(auto it = _operations.begin(); it != _operations.end(); ++it)
                {
                    (*it).second->writeValue(raw_row);
                    it->second->reset();
                }
                _result.getRows().emplace_back(Row(raw_row.buffer(), raw_row.size()));
                raw_row.reset();
                break;
            }
        }

        for(auto pt = _positions.begin(); pt != _positions.end(); ++pt)
        {
            _operations[*pt]->inputValue(prev_visitors[*pt]->get());
        }

        pos = 0;
        for(uint64_t i = 0; i < current_visitors.size(); i++)
        {
            pos = prev_visitors[i]->set(row.buffer(), pos);
        }
    }

    RawRow raw_row;
    for(auto it = _operations.begin(); it != _operations.end(); ++it)
    {
        (*it).second->writeValue(raw_row);
        it->second->reset();
    }
    _result.getRows().emplace_back(Row(raw_row.buffer(), raw_row.size()));
    raw_row.reset();

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;

    std::cout << "group duration = " << elapsed_time.count() << "s" << std::endl;

    return _result;
}

void GroupBy::operationsPosition()
{
    for(auto it = _operations.begin(); it != _operations.end(); ++it)
    {
        _positions.push_back((*it).first);
    }
}

Schema GroupBy::buildResultSchema()
{
    Schema schema;

    for(auto it = _operations.begin(); it != _operations.end(); ++it)
    {
        std::string column = (*it).second->getColumn();
        Type::type type = (*it).second->getOutputType();
        schema.push(Node(column, type));
    }

    std::cout << schema << std::endl;

    return schema;
}

void GroupBy::buildResultTable()
{
    Schema schema = buildResultSchema();
    _result = Table(schema);
    operationsPosition();
}
