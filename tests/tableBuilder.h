#ifndef TABLEBUILDER_H
#define TABLEBUILDER_H

#include <gtest/gtest.h>
#include <iostream>

#include "schema.h"
#include "table.h"
#include "aggregator.h"

TEST(Table, Build) 
{
    Schema schema;
    schema.push(Node("ua1", Type::INT32))
        .push(Node("ua2", Type::STRING));

    Table table(schema);
    std::cout << schema << std::endl;
}

#endif // TABLEBUILDER_H