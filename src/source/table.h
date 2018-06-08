#ifndef TABLE_H
#define TABLE_H

#include <vector>

#include "schema.h"
#include "row.h"

class Table
{
private:
    Schema _schema;
    std::vector<Row> _rows;
public:
    Table() {}
    Table(Schema schema):_schema(schema),_rows() {}
    Schema& getSchema();
    std::vector<Row>& getRows();
};

#endif // TABLE_H
