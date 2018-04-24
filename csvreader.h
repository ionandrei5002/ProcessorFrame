#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>

#include "row.h"
#include "schema.h"

class CsvReader
{
private:
    std::string _fileName;
    Schema _schema;
    std::vector<Row>* _rows;
public:
    CsvReader(std::string fileName, Schema schema, std::vector<Row>* rows)
        :_fileName(fileName),_schema(schema),_rows(rows) {}
    uint64_t read();
private:
    void split(std::vector<std::experimental::string_view>& results, std::string const& original, char separator);
};

#endif // CSVREADER_H
