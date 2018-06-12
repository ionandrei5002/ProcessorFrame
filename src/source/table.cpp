#include "table.h"

Table::Table(Table&& ot)
{
    _schema = ot._schema;
    _rows = std::move(ot._rows);
    _visitors = std::move(ot._visitors);
}

Table& Table::operator=(Table&& ot)
{
    _schema = ot._schema;
    _rows = std::move(ot._rows);
    _visitors = std::move(ot._visitors);
    return *this;
}

Schema& Table::getSchema()
{
    return _schema;
}

std::vector<Row>& Table::getRows()
{
    return _rows;
}

std::vector<std::unique_ptr<Visitor>>& Table::getVisitors() 
{
    return _visitors;
}

void Table::buildVisitors()
{
    for(uint64_t pos = 0; pos < _schema.size(); pos++)
    {
        _visitors.push_back(Visitor::builder(_schema.peek(pos)));
    }
}