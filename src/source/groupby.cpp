#include "groupby.h"

Schema GroupBy::buildOutputSchema()
{
    Schema schema;

    for(auto it = _inputColumns.begin(); it != _inputColumns.end(); ++it)
    {
        auto pair = (*it);
        std::cout << pair.first << std::endl;
        std::string column = pair.second->getColumn();
        Type::type type = pair.second->getOutputType();
        schema.push(Node(column, type));
    }

    std::cout << schema << std::endl;

    return schema;
}

void GroupBy::buildOutputTable()
{
    Schema schema = buildOutputSchema();
    _outputTable = Table(schema);
}
