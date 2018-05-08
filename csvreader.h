#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>

#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_numeric.hpp>

#include "row.h"
#include "schema.h"

class String2Type
{
public:
    virtual ~String2Type() {}
    virtual void write(std::experimental::string_view* value, Row& row) = 0;
};

template<typename T>
class Cast2Type: public String2Type
{
private:
    typedef typename T::c_type _val;
public:
    void write(std::experimental::string_view* value, Row& row) override
    {
        throw;
    }
};

template<>
class Cast2Type<Int64Type>: public String2Type
{
private:
    typedef typename Int64Type::c_type _val;
public:
    void write(std::experimental::string_view* value, Row& row) override
    {
        _val cast_value = 0;
        boost::spirit::qi::parse(value->begin(), value->end(), boost::spirit::qi::long_, cast_value);
        row.append(reinterpret_cast<char*>(&cast_value), sizeof(_val));
    }
};

template<>
class Cast2Type<FloatType>: public String2Type
{
private:
    typedef typename FloatType::c_type _val;
public:
    void write(std::experimental::string_view* value, Row& row) override
    {
        _val cast_value = 0;
        boost::spirit::qi::parse(value->begin(), value->end(), boost::spirit::qi::float_, cast_value);
        row.append(reinterpret_cast<char*>(&cast_value), sizeof(_val));
    }
};

template<>
class Cast2Type<StringType>: public String2Type
{
public:
    void write(std::experimental::string_view* value, Row& row) override
    {
        uint64_t size = value->size();
        const char* data = value->data();
        row.append(reinterpret_cast<char*>(&size), sizeof(size));
        row.append(data, size);
    }
};

typedef Cast2Type<UInt8Type> Cast2UInt8;
typedef Cast2Type<Int8Type> Cast2Int8;
typedef Cast2Type<UInt16Type> Cast2UInt16;
typedef Cast2Type<Int16Type> Cast2Int16;
typedef Cast2Type<UInt32Type> Cast2UInt32;
typedef Cast2Type<Int32Type> Cast2Int32;
typedef Cast2Type<UInt64Type> Cast2UInt64;
typedef Cast2Type<Int64Type> Cast2Int64;
typedef Cast2Type<FloatType> Cast2Float;
typedef Cast2Type<DoubleType> Cast2Double;
typedef Cast2Type<StringType> Cast2String;

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
    static std::unique_ptr<String2Type> builder(Node node);
private:
    void split(std::vector<std::experimental::string_view>& results, std::string const& original, char separator);
};

#endif // CSVREADER_H
