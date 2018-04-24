#include <iostream>

#include "row.h"
#include "schema.h"
#include "visitor.h"
#include "csvreader.h"

using namespace std;

int main()
{
    std::vector<std::string> record;
    record.push_back(std::string("64"));
    record.push_back(std::string("Hello World!"));

    Schema schema;
    schema.push(Node("u1", Type::STRING))
            .push(Node("u2", Type::STRING));

    cout << schema << endl;

    std::vector<Row> rows;

    CsvReader reader("/home/andrei/Desktop/desktop/MC5Dau.csv", schema, &rows);
    cout << "rows readed : " << reader.read() << endl;
    cout << rows.size() << endl;

    std::vector<std::unique_ptr<Visitor>> visitors;
    visitors.push_back(std::make_unique<StringVisitor>());
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

        visitors.at(0)->get();
        visitors.at(1)->get();
    }

//    Row row;
//    int32_t u1 = std::stoi(record.at(0));
//    row.append(reinterpret_cast<char*>(&u1), sizeof(u1));

//    {
//        uint64_t size = record.at(1).size();
//        const char* data = record.at(1).data();
//        row.append(reinterpret_cast<char*>(&size), sizeof(size));
//        row.append(data, size);
//    }

//    {
//        std::vector<std::unique_ptr<Visitor>> visitors;
//        visitors.push_back(std::make_unique<Int32Visitor>());
//        visitors.push_back(std::make_unique<StringVisitor>());
//        uint64_t pos = 0;
//        for(auto it = visitors.begin(); it != visitors.end(); ++it)
//        {
//            if (pos < row.size())
//                pos += (*it)->set(row.buffer(), pos);
//        }

//        cout << visitors.at(0)->get() << endl;
//        cout << visitors.at(1)->get() << endl;
//    }

//    cout << row << endl;

    return 0;
}
