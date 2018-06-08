#include "aggregator.h"

std::string Aggregator::getColumn()
{
    return _column;
}

Type::type Aggregator::getInputType()
{
    return _inputType;
}

Type::type Aggregator::getOutputType()
{
    return _outputType;
}
