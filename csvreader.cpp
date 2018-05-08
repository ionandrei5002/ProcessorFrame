#include "csvreader.h"

uint64_t CsvReader::read()
{
    uint64_t rowsRead = 0;

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();

    std::vector<std::unique_ptr<String2Type>> _casters;
    for(uint64_t i = 0; i < _schema.size(); i++)
    {
        _casters.push_back(builder(_schema.peek(i)));
    }

    std::fstream input(_fileName);
    std::string line;
    line.reserve(1024 * 1024);
    std::vector<std::experimental::string_view> piece;

    std::getline(input, line);
    while(std::getline(input, line))
    {
        Row row;
        split(piece, line, ',');
        for(size_t pos = 0; pos < _casters.size(); ++pos)
        {
            std::experimental::string_view* value = &piece[pos];

            _casters.at(pos)->write(value, row);
        }

        piece.clear();
        _rows->emplace_back(row);
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

std::unique_ptr<String2Type> CsvReader::builder(Node node)
{
    Type::type type = node.getType();
    switch (type) {
        case Type::UINT8:
        case Type::INT8:
        case Type::UINT16:
        case Type::INT16:
        case Type::UINT32:
        case Type::INT32:
            return nullptr;
        case Type::FLOAT:
        case Type::DOUBLE:
            return std::make_unique<Cast2Float>();
        case Type::UINT64:
        case Type::INT64:
            return std::make_unique<Cast2Int64>();
        case Type::STRING:
            return std::make_unique<Cast2String>();
        default:
            return nullptr;

    }
}
