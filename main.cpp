#include <iostream>
#include <chrono>
#include <experimental/filesystem>

#include "row.h"
#include "schema.h"
#include "visitor.h"
#include "csvreader.h"
#include "value.h"
#include "aggregator.h"
#include "table.h"
#include "sorter.h"
#include "groupby.h"

int main()
{
    Schema schema;
    schema.push(Node("udid", Type::INT64))
            .push(Node("ggi", Type::INT32))
            .push(Node("date", Type::STRING))
            .push(Node("realm", Type::INT32))
            .push(Node("silo", Type::STRING));

    std::cout << schema << std::endl;

    Table table(schema);

    std::vector<Row>& rows = table.getRows();
    std::string source = "/home/andrei/BI Python/Output/RealmImport/";
    std::experimental::filesystem::path path(source);

    std::vector<std::string> files;
    for(auto it : std::experimental::filesystem::directory_iterator(path))
    {
        auto filename = (--it.path().end());
        if (!((*filename).string().find("pau", 0) != std::string::npos))
            files.emplace_back((*filename).string());
        break;
    }
//    std::sort(files.begin(), files.end());

    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();
        for(auto it = files.begin(); it != files.end(); ++it)
        {
            CsvReader reader(source + (*it), table);
            std::cout << (*it) << " rows readed : " << reader.read() << std::endl;
            std::cout << (*it) << " size: " << rows.size() << std::endl;
        }

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "read duration = " << elapsed_time.count() << "s" << std::endl;
    }

    GroupBy agg(table);
    agg.Group(std::make_shared<None<Int32Type>>("ggi"))
//                .Group(std::make_shared<None<StringType>>("date"))
            .Group(std::make_shared<None<Int32Type>>("realm"))
            .Group(std::make_shared<None<StringType>>("silo"))
//                .Count(std::make_shared<Count<Int64Type>>("udid"))
            .CountDistinct(std::make_shared<CountDistinct<Int64Type>>("udid"))
            .Const(std::make_shared<Const<Int64Type>>("constant", MakeInt64Value(999)));
    agg.buildOutputSchema();
    agg.buildOutputTable();

//    {
//        GroupBy agg(table);
//        agg.Group(std::make_shared<None<Int32Type>>("ggi"))
////                .Group(std::make_shared<None<StringType>>("date"))
//                .Group(std::make_shared<None<Int32Type>>("realm"))
//                .Group(std::make_shared<None<StringType>>("silo"))
////                .Count(std::make_shared<Count<Int64Type>>("udid"))
//                .CountDistinct(std::make_shared<CountDistinct<Int64Type>>("udid"));

//        std::ofstream out("/home/andrei/Desktop/output.txt");

//        Table& result = agg.getResult();

//        Sorter sorter(std::vector<std::string>({"ggi","realm","silo"}));
//        sorter.sort(result);

//        std::vector<std::unique_ptr<Visitor>> visitors;

//        for(uint64_t i = 0; i < result.getSchema().size(); i++)
//        {
//            visitors.push_back(Visitor::builder(result.getSchema().peek(i)));
//        }

//        for(auto jt = result.getRows().begin(); jt != result.getRows().end(); ++jt)
//        {
//            Row& row = (*jt);
//            uint64_t pos = 0;
//            for(uint64_t i = 0; i < visitors.size(); i++)
//            {
//                pos = visitors[i]->set(row.buffer(), pos);
//            }

//            for(uint64_t i = 0; i < (visitors.size() - 1); i++)
//            {
//                visitors[i]->print(out);
//                out << ",";
//            }
//            visitors[(visitors.size() - 1)]->print(out);
//            out << std::endl;
//        }

//        out.close();
//    }

    return 0;
}
