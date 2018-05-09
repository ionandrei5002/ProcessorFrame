#include <iostream>
#include <chrono>

#include "row.h"
#include "schema.h"
#include "visitor.h"
#include "csvreader.h"
#include "comparator.h"

#include <valgrind/callgrind.h>

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

    CsvReader reader("/home/andrei/Desktop/desktop/mc5_android.csv", schema, &rows);
    cout << "rows readed : " << reader.read() << endl;
    cout << rows.size() << endl;

    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        Comparator comp(schema, std::vector<uint64_t>({0,1,2,3,4,5,6,7,8,9,10}));

        std::sort_heap(rows.begin(), rows.end(), std::ref(comp));

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "sort duration = " << elapsed_time.count() << "s" << std::endl;
    }

    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        std::ofstream out("/home/andrei/Desktop/output.txt");

        std::vector<std::unique_ptr<Visitor>> visitors;
        for(uint64_t i = 0; i < schema.size(); i++)
        {
            visitors.push_back(Visitor::builder(schema.peek(i)));
        }
        for(auto jt = rows.begin(); jt != rows.end(); ++jt)
        {
            Row& row = (*jt);
            uint64_t pos = 0;
            for(auto it = visitors.begin(); it != visitors.end(); ++it)
            {
                if (pos < row.size())
                {
                    pos = (*it)->set(row.buffer(), pos);
                } else {
                    std::cout << "bad field position: " << pos << std::endl;
                }
            }
            for(auto it = visitors.begin(); it != visitors.end(); ++it)
            {
                (*it)->print(out);
                out << ",";
            }
            out << endl;
        }

        out.close();

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "write duration = " << elapsed_time.count() << "s" << std::endl;
    }

    return 0;
}
