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
    std::vector<std::unique_ptr<Visitor>> _lv;
    std::vector<std::unique_ptr<Visitor>> _rv;
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
    static std::unique_ptr<Visitor> builder(Node node)
    {
        Type::type type = node.getType();
        switch (type) {
            case Type::UINT8:
            case Type::INT8:
            case Type::UINT16:
            case Type::INT16:
            case Type::UINT32:
            case Type::INT32:
            case Type::FLOAT:
            case Type::DOUBLE:
                return nullptr;
            case Type::UINT64:
            case Type::INT64:
                return std::make_unique<Int64Visitor>();
            case Type::STRING:
                return std::make_unique<StringVisitor>();
            default:
                return nullptr;

        }
    }
    inline bool operator()(const Row& lv, const Row& rv) const
    {
        {
            uint64_t pos = 0;
            for(auto it = _lv.begin(); it != _lv.end(); ++it)
            {
                if (pos < lv.size())
                    pos += (*it)->set(lv.buffer(), pos);
            }
        }
        {
            uint64_t pos = 0;
            for(auto it = _rv.begin(); it != _rv.end(); ++it)
            {
                if (pos < rv.size())
                    pos += (*it)->set(rv.buffer(), pos);
            }
        }

        bool _comp = false;

        for(auto it = _columns.begin(); it != _columns.end(); ++it)
        {
            uint64_t pos = (*it);
            if (_lv[pos]->get() != _rv[pos]->get())
            {
                _comp = (_lv[pos]->get() < _rv[pos]->get());
            }
        }

        return _comp;
    }
};

#endif // COMPARATOR_H
