#include "table.h"

Schema& Table::getSchema()
{
    return _schema;
}

std::vector<Row>& Table::getRows()
{
    return _rows;
}
