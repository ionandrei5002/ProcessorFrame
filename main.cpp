#include <iostream>
#include <chrono>

#include "row.h"
#include "schema.h"
#include "visitor.h"
#include "csvreader.h"
#include "comparator.h"

#include <valgrind/callgrind.h>

#include <parallel/algorithm>

using namespace std;

int main()
{
    Schema schema;
    schema.push(Node("u1", Type::STRING))
            .push(Node("u2", Type::INT64))
            .push(Node("u3", Type::INT64))
            .push(Node("u4", Type::FLOAT))
            .push(Node("u5", Type::FLOAT))
            .push(Node("u6", Type::STRING))
            .push(Node("u7", Type::FLOAT))
            .push(Node("u8", Type::FLOAT))
            .push(Node("u9", Type::FLOAT))
            .push(Node("u10", Type::STRING))
            .push(Node("u11", Type::STRING))
            .push(Node("u12", Type::INT64));

    cout << schema << endl;

    std::vector<Row> rows;

    CsvReader reader("/home/andrei/Desktop/desktop/sf_android.csv", schema, &rows);
    cout << "rows readed : " << reader.read() << endl;
    cout << rows.size() << endl;

//    {
//        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
//        start = std::chrono::high_resolution_clock::now();

//        Comparator comp(schema, std::vector<uint64_t>({0,1}));

////        std::sort_heap(rows.begin(), rows.end());
//        std::sort_heap(rows.begin(), rows.end(), std::ref(comp));
////        __gnu_parallel::sort(rows.begin(), rows.end(), __gnu_parallel::quicksort_tag());

//        end = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> elapsed_time = end - start;

//        std::cout << "sort duration = " << elapsed_time.count() << "s" << std::endl;
//    }

    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        std::ofstream out("/home/andrei/Desktop/output.txt");

        std::vector<std::unique_ptr<Visitor>> visitors;
        visitors.push_back(std::make_unique<StringVisitor>());
        visitors.push_back(std::make_unique<Int64Visitor>());
        visitors.push_back(std::make_unique<Int64Visitor>());
        visitors.push_back(std::make_unique<FloatVisitor>());
        visitors.push_back(std::make_unique<FloatVisitor>());
        visitors.push_back(std::make_unique<StringVisitor>());
        visitors.push_back(std::make_unique<FloatVisitor>());
        visitors.push_back(std::make_unique<FloatVisitor>());
        visitors.push_back(std::make_unique<FloatVisitor>());
        visitors.push_back(std::make_unique<StringVisitor>());
        visitors.push_back(std::make_unique<StringVisitor>());
        visitors.push_back(std::make_unique<Int64Visitor>());
        for(auto jt = rows.begin(); jt != rows.end(); ++jt)
        {
            Row& row = (*jt);
            std::cout << row.size() << std::endl;
            std::cout << row.capacity() << std::endl;
            uint64_t pos = 0;
            for(uint64_t i = 0; i < visitors.size(); i++)
            {
                if (pos < row.size())
                {
//                    std::cout << pos << std::endl;
                    pos += visitors[i]->set(row.buffer(), pos);
                } else {
                    std::cout << "bad field position: " << pos << std::endl;
                }
            }
            for(uint64_t i = 0; i < visitors.size(); i++)
            {
                visitors[i]->print(out);
                out << ",";
            }
            out << endl;
//            for(auto it = visitors.begin(); it != visitors.end(); ++it)
//            {
//                if (pos < row.size())
//                {
//                    std::cout << pos << std::endl;
//                    pos += (*it)->set(row.buffer(), pos);
//                } else {
//                    std::cout << "bad field position: " << pos << std::endl;
//                }
//            }
//            for(auto it = visitors.begin(); it != visitors.end(); ++it)
//            {
//                (*it)->print(out);
//                out << ",";
//            }
//            out << endl;
        }

        out.close();

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "write duration = " << elapsed_time.count() << "s" << std::endl;
    }

    return 0;
}
