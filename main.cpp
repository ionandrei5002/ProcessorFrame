#include <iostream>
#include <chrono>

#include "row.h"
#include "schema.h"
#include "visitor.h"
#include "csvreader.h"

#include <valgrind/callgrind.h>

using namespace std;

void quickSort(uint64_t low, uint64_t high, std::vector<Row>& rows);
void swap(uint64_t lv, uint64_t rv, std::vector<Row>& rows);
uint64_t partition (uint64_t low, uint64_t high, std::vector<Row>& rows);

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

    CALLGRIND_START_INSTRUMENTATION;

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();

//    std::sort(rows.begin(), rows.end());
    quickSort(0, rows.size() - 1, rows);

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;

    std::cout << "sort duration = " << elapsed_time.count() << "s" << std::endl;

    CALLGRIND_STOP_INSTRUMENTATION;
    CALLGRIND_DUMP_STATS;

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

    return 0;
}

void swap(uint64_t lv, uint64_t rv, std::vector<Row>& rows)
{
    Row tmp = std::move(rows[lv]);
    rows[lv] = std::move(rows[rv]);
    rows[rv] = std::move(tmp);
}

uint64_t partition (uint64_t low, uint64_t high, std::vector<Row>& rows)
{
    uint64_t pivot = high;
    uint64_t i = (low - 1);

    for (uint64_t j = low; j <= high - 1; j++)
    {
        if (rows[j] < rows[pivot])
        {
            i++;
            swap(i, j, rows);
        }
    }
    swap(i + 1, high, rows);
    return (i + 1);
}

void quickSort(uint64_t low, uint64_t high, std::vector<Row>& rows)
{
    if (low < high)
    {
        uint64_t pi = partition(low, high, rows);

        quickSort(low, pi - 1, rows);
        quickSort(pi + 1, high, rows);
    }
}
