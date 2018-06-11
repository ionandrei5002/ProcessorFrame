#ifndef TABLEBUILDER_H
#define TABLEBUILDER_H

#include <gtest/gtest.h>
#include <iostream>

#include "row.h"
#include "schema.h"
#include "table.h"
#include "groupby.h"
#include "aggregator.h"
#include "csvreader.h"

TEST(Table, Build) 
{
    Schema schema;
    schema.push(Node("ua1", Type::INT32))
        .push(Node("ua2", Type::STRING));

    Table table(schema);
    std::cout << schema << std::endl;

    std::string a1("99");
    std::experimental::string_view aa1(a1);
    std::string a2("test");
    std::experimental::string_view aa2(a2);

    Cast2Int32 ua1;
    Cast2String ua2;

    RawRow tmp;
    ua1.write(&aa1, tmp);
    ua2.write(&aa2, tmp);
    table.getRows().emplace_back(Row(tmp.buffer(), tmp.size()));
    table.getRows().emplace_back(Row(tmp.buffer(), tmp.size()));

    std::vector<std::unique_ptr<Visitor>> visitors;
    visitors.push_back(std::make_unique<Int32Visitor>());
    visitors.push_back(std::make_unique<StringVisitor>());

    for(auto jt = table.getRows().begin(); jt != table.getRows().end(); ++jt)
    {
        Row& row = (*jt);
        uint64_t pos = 0;
        for(uint64_t i = 0; i < visitors.size(); i++)
        {
            pos = visitors[i]->set(row.buffer(), pos);
        }

        for(uint64_t i = 0; i < (visitors.size() - 1); i++)
        {
            visitors[i]->print(std::cout);
            std::cout << ",";
        }
        visitors[(visitors.size() - 1)]->print(std::cout);
        std::cout << std::endl;
    }

    std::vector<std::pair<std::shared_ptr<Aggregator>, std::reference_wrapper<std::unique_ptr<Visitor>>>> aggregator;
    aggregator.push_back(std::make_pair(std::make_shared<None<Int32Type>>("ua1"), std::ref(visitors[0])));
    aggregator.push_back(std::make_pair(std::make_shared<None<StringType>>("ua2"), std::ref(visitors[1])));
    auto nullVisitor = std::unique_ptr<Visitor>(nullptr);
    aggregator.push_back(std::make_pair(std::make_shared<Const<Int32Type>>("constant", MakeInt64Value(999)), std::ref(nullVisitor)));

    for(auto jt = table.getRows().begin(); jt != table.getRows().end(); ++jt)
    {
        Row& row = (*jt);
        uint64_t pos = 0;
        for(uint64_t i = 0; i < visitors.size(); i++)
        {
            pos = visitors[i]->set(row.buffer(), pos);
        }
        
        for(auto it = aggregator.begin(); it != aggregator.end(); ++it)
        {
            auto aggregate = (*it).first;
            auto& visitor = (*it).second;
            aggregate->inputValue(visitor);
            aggregate->print(std::cout);
            std::cout << std::endl;
        }
    }

    GroupBy agg(table);
    agg.Group(std::make_shared<None<Int32Type>>("ua1"));
    agg.Group(std::make_shared<None<StringType>>("ua2"));
    agg.Const(std::make_shared<Const<Int32Type>>("constant", MakeInt64Value(999)));

    std::cout << agg.buildOutputSchema() << std::endl;
}

#endif // TABLEBUILDER_H