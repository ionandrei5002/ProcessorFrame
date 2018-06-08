#include "sorter.h"

void Sorter::sort(Table& table)
{
    Schema& schema = table.getSchema();
    std::vector<Row>& rows = table.getRows();
    std::vector<uint64_t> positions;

    for(auto it = _columns.begin(); it != _columns.end(); ++it)
    {
        positions.push_back(schema.position(*it));
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();

    Comparator comp(schema, positions);

    std::sort(rows.begin(), rows.end(), std::ref(comp));

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;

    std::cout << "sort duration = " << elapsed_time.count() << "s" << std::endl;
}
