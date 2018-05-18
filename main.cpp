#include <iostream>
#include <chrono>
#include <experimental/filesystem>

#include "row.h"
#include "schema.h"
#include "visitor.h"
#include "csvreader.h"
#include "comparator.h"
#include "value.h"
#include "aggregator.h"

int main()
{
    Schema schema;
    schema.push(Node("udid", Type::INT64))
            .push(Node("ggi", Type::INT32))
            .push(Node("date", Type::STRING))
            .push(Node("realm", Type::INT32))
            .push(Node("silo", Type::STRING));

    std::cout << schema << std::endl;

    std::vector<Row> rows;
    std::string source = "/home/andrei/BI Python/Output/RealmImport/";
    std::experimental::filesystem::path path(source);

    std::vector<std::string> files;
    for(auto it : std::experimental::filesystem::directory_iterator(path))
    {
        auto filename = (--it.path().end());
        if (!((*filename).string().find("pau", 0) != std::string::npos))
            files.emplace_back((*filename).string());
    }
    std::sort(files.begin(), files.end());

    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();
        for(auto it = files.begin(); it != files.end(); ++it)
        {
            CsvReader reader(source + (*it), schema, &rows);
            std::cout << (*it) << " rows readed : " << reader.read() << std::endl;
            std::cout << (*it) << " size: " << rows.size() << std::endl;
            break;
        }

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "read duration = " << elapsed_time.count() << "s" << std::endl;
    }

    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        Comparator comp(schema, std::vector<uint64_t>({2}));

        std::sort(rows.begin(), rows.end(), std::ref(comp));

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "sort duration = " << elapsed_time.count() << "s" << std::endl;
    }

    {
        uint64_t rows_bytes = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        std::ofstream out("/home/andrei/Desktop/output.txt");

        std::vector<std::unique_ptr<Visitor>> visitors;
        for(uint64_t i = 0; i < schema.size(); i++)
        {
            visitors.push_back(Visitor::builder(schema.peek(i)));
        }

        std::unique_ptr<Value> date = MakeStringValue("2015-08-15");
        date->print(std::cout);
        std::cout << std::endl;

        std::vector<std::shared_ptr<Aggregator>> aggregators;
        aggregators.push_back(std::make_shared<Count<Int64Type>>());
        aggregators.push_back(std::make_shared<None<Int32Type>>());
        aggregators.push_back(std::make_shared<None<StringType>>());
        aggregators.push_back(std::make_shared<None<Int32Type>>());
        aggregators.push_back(std::make_shared<None<StringType>>());

        int rows_write = 0;
        for(auto jt = rows.begin(); jt != rows.end(); ++jt)
        {
            Row& row = (*jt);
            rows_bytes += row.size();
            uint64_t pos = 0;
            for(uint64_t i = 0; i < visitors.size(); i++)
            {
                pos = visitors[i]->set(row.buffer(), pos);
                aggregators[i]->inputValue(visitors[i]->get());
            }

//            std::unique_ptr<Value> val = visitors[2]->getValue();
//            if (val->operator<(date))
//            {
//                for(uint64_t i = 0; i < (visitors.size() - 1); i++)
//                {
//                    visitors[i]->print(out);
//                    out << ",";
//                }
//                visitors[(visitors.size() - 1)]->print(out);

//                out << std::endl;
//                rows_write++;
//            } else {
//                break;
//            }
        }

        for(auto it = aggregators.begin(); it != aggregators.end(); ++it)
        {
            (*it)->print(std::cout);
            std::cout << ",";
        }
        std::cout << std::endl;

        std::cout << "rows write: " << rows_write << std::endl;

        out.close();

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "write duration = " << elapsed_time.count() << "s" << std::endl;
        std::cout << "capacity : " << rows.capacity() << std::endl;
        std::cout << "size: " << rows.size() << std::endl;
        std::cout << "memory used: " << ((rows.capacity() * sizeof(Row) + rows_bytes) / 1024) << " kb" << std::endl;
        std::cout << "memory rows: " << (rows_bytes / 1024) << " kb" << std::endl;
        std::cout << "avg row size: " << (rows_bytes / rows.size()) << " b" << std::endl;
    }

    return 0;
}
