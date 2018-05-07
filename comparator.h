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
            if (lvpos < lv.size())
                lvpos += _lv[i]->set(lv.buffer(), lvpos);
            if (rvpos < rv.size())
                rvpos += _rv[i]->set(rv.buffer(), rvpos);
        }

        bool _comp = false;

        for(auto it = _columns.begin(); it != _columns.end(); ++it)
        {
            uint64_t pos = (*it);
            ViewByteBuffer lvview = std::move(_lv[pos]->get());
            ViewByteBuffer rvview = std::move(_rv[pos]->get());
            if (lvview != rvview)
            {
                return (lvview < rvview);
            }
        }

        return _comp;
    }
};

#endif // COMPARATOR_H
