#ifndef TABLE_H
#define TABLE_H

#include <vector>

#include "schema.h"
#include "row.h"
#include "visitor.h"

class Table
{
private:
    Schema _schema;
    std::vector<Row> _rows;
    std::vector<std::unique_ptr<Visitor>> _visitors;
    void buildVisitors();
public:
    Table() {}
    Table(Schema schema):_schema(schema),_rows() { buildVisitors(); }
    Table(Table&& ot);
    Table& operator=(Table&& ot);
    Schema& getSchema();
    std::vector<Row>& getRows();
    std::vector<std::unique_ptr<Visitor>>& getVisitors();
};

#endif // TABLE_H
