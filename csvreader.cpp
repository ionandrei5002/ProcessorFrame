#include "csvreader.h"

uint64_t CsvReader::read()
{
    uint64_t rowsRead = 0;

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();

    std::fstream input(_fileName);
    std::string line;
    line.reserve(1024 * 1024);
    std::vector<std::experimental::string_view> piece;

    std::getline(input, line);
    while(std::getline(input, line))
    {
        Row row;
        split(piece, line, ',');
        for(size_t pos = 0; pos < piece.size(); ++pos)
        {
            std::experimental::string_view* value = &piece[pos];

            uint64_t size = value->size();
            const char* data = value->data();
            row.append(reinterpret_cast<char*>(&size), sizeof(size));
            row.append(data, size);
        }

        piece.clear();
        _rows->push_back(row);
        rowsRead++;
    }

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;

    std::cout << "read duration = " << elapsed_time.count() << "s" << std::endl;

    return rowsRead;
}

void CsvReader::split(std::vector<std::experimental::string_view>& results, std::string const& original, char separator)
{
    std::string::const_iterator start = original.begin();
    std::string::const_iterator end = original.end();
    std::string::const_iterator next = std::find(start, end, separator);
    while(next != end && *start == '"' && *(next-1) != '"')
    {
        next = std::find(next + 1, end, separator);
    }
    while (next != end) {
        std::experimental::string_view str(start.operator ->(), static_cast<uint64_t>(next.operator ->() - start.operator ->()));
        results.push_back(str);

        start = next + 1;
        next = std::find(start, end, separator);
        while(*start == '"' && *(next-1) != '"' && next != end)
        {
            next = std::find(next + 1, end, separator);
        }
    }

    std::experimental::string_view str(start.operator ->(), static_cast<uint64_t>(next.operator ->() - start.operator ->()));
    results.push_back(str);
}
