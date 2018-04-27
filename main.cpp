#include <iostream>
#include <chrono>

#include "row.h"
#include "schema.h"
#include "visitor.h"
#include "csvreader.h"
#include "comparator.h"

#include <valgrind/callgrind.h>

#include <parallel/algorithm>

#include "allocator/free_list_pool.h"

FreeListPool freelist_pool(1l * 1024 * 1024 * 1024);

using namespace std;

int main()
{
    Schema schema;
    schema.push(Node("u1", Type::INT64))
            .push(Node("u2", Type::STRING));

    cout << schema << endl;

    std::vector<Row> rows;

    CsvReader reader("/home/andrei/Desktop/desktop/MC5Dau.csv", schema, &rows);
    cout << "rows readed : " << reader.read() << endl;
    cout << rows.size() << endl;

    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        Comparator comp(schema, std::vector<uint64_t>({0,1}));

        std::sort_heap(rows.begin(), rows.end(), comp);
//        __gnu_parallel::sort(rows.begin(), rows.end(), __gnu_parallel::quicksort_tag());

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "sort duration = " << elapsed_time.count() << "s" << std::endl;
    }

    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        std::vector<std::unique_ptr<Visitor>> visitors;
        visitors.push_back(std::make_unique<Int64Visitor>());
        visitors.push_back(std::make_unique<StringVisitor>());
        for(auto jt = rows.begin(); jt != rows.end(); ++jt)
        {
            Row& row = (*jt);
            uint64_t pos = 0;
            for(auto it = visitors.begin(); it != visitors.end(); ++it)
            {
                if (pos < row.size())
                    pos += (*it)->set(row.buffer(), pos);
            }

    //        visitors.at(0)->print(cout);
    //        cout << " ";
    //        visitors.at(1)->print(cout);
    //        cout << endl;
        }

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "write duration = " << elapsed_time.count() << "s" << std::endl;
    }

    return 0;
}
