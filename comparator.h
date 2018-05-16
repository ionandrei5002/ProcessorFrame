#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <vector>

#include "row.h"
#include "schema.h"
#include "visitor.h"

class Comparator
{
private:
    Schema _schema;
    std::vector<uint64_t> _columns;
    std::vector<std::shared_ptr<Visitor>> _lv;
    std::vector<std::shared_ptr<Visitor>> _rv;
public:
    Comparator(Schema schema, std::vector<uint64_t> columns):_schema(schema), _columns(columns)
    {
        for(uint64_t i = 0; i < schema.size(); i++)
        {
            _lv.push_back(builder(schema.peek(i)));
            _rv.push_back(builder(schema.peek(i)));
        }
    }
    Comparator(const Comparator& ot)
    {
        this->_schema = ot._schema;
        this->_columns = ot._columns;
        this->_lv = ot._lv;
        this->_rv = ot._rv;
    }
    Comparator& operator=(const Comparator& ot)
    {
        this->_schema = ot._schema;
        this->_columns = ot._columns;
        this->_lv = ot._lv;
        this->_rv = ot._rv;

        return *this;
    }
    Comparator(Comparator&& ot) noexcept
    {
        this->_schema = ot._schema;
        this->_columns = ot._columns;
        this->_lv = ot._lv;
        this->_rv = ot._rv;
    }
    Comparator& operator=(Comparator&& ot) noexcept
    {
        this->_schema = ot._schema;
        this->_columns = ot._columns;
        this->_lv = ot._lv;
        this->_rv = ot._rv;

        return *this;
    }
    static std::shared_ptr<Visitor> builder(Node node)
    {
        Type::type type = node.getType();
        switch (type) {
            case Type::UINT8:
                return std::make_shared<UInt8Visitor>();
            case Type::INT8:
                return std::make_shared<Int8Visitor>();
            case Type::UINT16:
                return std::make_shared<UInt16Visitor>();
            case Type::INT16:
                return std::make_shared<Int16Visitor>();
            case Type::UINT32:
                return std::make_shared<UInt32Visitor>();
            case Type::INT32:
                return std::make_shared<Int32Visitor>();
            case Type::FLOAT:
                return std::make_shared<FloatVisitor>();
            case Type::DOUBLE:
                return std::make_shared<DoubleVisitor>();
            case Type::UINT64:
                return std::make_shared<UInt64Visitor>();
            case Type::INT64:
                return std::make_shared<Int64Visitor>();
            case Type::STRING:
                return std::make_shared<StringVisitor>();
            default:
                return nullptr;

        }
    }
    inline bool operator()(const Row& lv, const Row& rv) const
    {
        uint64_t lvpos = 0;
        uint64_t rvpos = 0;
        for(uint64_t i = 0; i < _lv.size(); ++i)
        {
            lvpos = _lv[i]->set(lv.buffer(), lvpos);
            rvpos = _rv[i]->set(rv.buffer(), rvpos);
        }

        bool _comp = false;

        for(auto it = _columns.begin(); it != _columns.end(); ++it)
        {
            uint64_t pos = (*it);
            if (_lv[pos]->operator!=(_rv[pos]))
            {
                _comp = (_lv[pos]->operator<(_rv[pos]));
                return _comp;
            }
        }

        return _comp;
    }
};

#endif // COMPARATOR_H
