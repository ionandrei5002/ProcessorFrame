#include <iostream>
#include <chrono>
#include <experimental/filesystem>

#include "row.h"
#include "schema.h"
#include "visitor.h"
#include "csvreader.h"
#include "comparator.h"

using namespace std;

int main()
{
    Schema schema;
    schema.push(Node("udid", Type::INT64))
            .push(Node("ggi", Type::INT32))
            .push(Node("date", Type::STRING))
            .push(Node("realm", Type::INT32))
            .push(Node("silo", Type::STRING));

    cout << schema << endl;

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
            cout << (*it) << " rows readed : " << reader.read() << endl;
            cout << (*it) << " size: " << rows.size() << endl;
        }

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "read duration = " << elapsed_time.count() << "s" << std::endl;
    }

//    {
//        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
//        start = std::chrono::high_resolution_clock::now();

//        Comparator comp(schema, std::vector<uint64_t>({2,3,4}));

//        std::sort(rows.begin(), rows.end(), std::ref(comp));

//        end = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> elapsed_time = end - start;

//        std::cout << "sort duration = " << elapsed_time.count() << "s" << std::endl;
//    }

    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        std::ofstream out("/home/andrei/Desktop/output.txt");
        const int buffSize = 16 * 1024 * 1024;
        char* buffer = new char[buffSize];
        out.rdbuf()->pubsetbuf(buffer, buffSize);

        std::vector<std::unique_ptr<Visitor>> visitors;
        for(uint64_t i = 0; i < schema.size(); i++)
        {
            visitors.push_back(Visitor::builder(schema.peek(i)));
        }

        std::string date = "2015-08-12";
        std::experimental::string_view view(date.data(), date.size());
        Cast2String str;
        Row rr;
        str.write(&view, rr);
        std::shared_ptr<Visitor> condition = std::make_shared<StringVisitor>();
        condition->set(rr.buffer(), 0);
        condition->print(std::cout);
        std::cout << std::endl;

        int rows_write = 0;
        for(auto jt = rows.begin(); jt != rows.end(); ++jt)
        {
            Row& row = (*jt);
            uint64_t pos = 0;
            for(uint64_t i = 0; i < visitors.size(); i++)
            {
                pos = visitors[i]->set(row.buffer(), pos);
            }
            if (visitors[2]->operator!=(condition) == false)
            {
                for(uint64_t i = 0; i < (visitors.size() - 1); i++)
                {
                    visitors[i]->print(out);
                    out << ",";
                }
                visitors[(visitors.size() - 1)]->print(out);

                out << endl;
                rows_write++;
            }
        }

        std::cout << "rows write: " << rows_write << std::endl;

        out.close();
        delete[] buffer;
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "write duration = " << elapsed_time.count() << "s" << std::endl;
    }

    return 0;
}
