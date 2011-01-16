#ifndef __STACKENING_TEST_DATA_NODE_H__
#define __STACKENING_TEST_DATA_NODE_H__

#include <string>
#include <ostream>

#include "../misc/pos-type.h"

namespace test {

    template <typename _NodeData> struct nothing_node_templ;
    template <typename _NodeData> struct string_node_templ;

    struct node_type {
        std::string const type_img;

        explicit node_type(std::string const& name)
            : type_img(name)
        {}

        bool operator==(node_type const& rhs) const
        {
            return type_img == rhs.type_img;
        }
    };

    template <typename _NodeData>
    struct data_node_templ {
        node_type const type_img;
        _NodeData const sub_data;
    public:
        virtual ~data_node_templ() {}

        bool operator==(data_node_templ const& rhs) const
        {
            return type_img == rhs.type_img && sub_data == rhs.sub_data && cmp(rhs);
        }

        virtual std::ostream& print(std::ostream& os) const = 0;

        virtual bool cmp(data_node_templ<_NodeData> const& rhs) const = 0;

        virtual bool cmp_no_data(nothing_node_templ<_NodeData> const&) const
        {
            return false;
        }

        virtual bool cmp_str_data(string_node_templ<_NodeData> const&) const
        {
            return false;
        }
    protected:
        data_node_templ(node_type const& timg, _NodeData const& sdata)
            : type_img(timg)
            , sub_data(sdata)
        {}
    };

    template <typename _NodeData>
    struct nothing_node_templ
        : public data_node_templ<_NodeData>
    {
        nothing_node_templ(node_type const& type_img, _NodeData const& sub_data)
            : data_node_templ<_NodeData>(type_img, sub_data)
        {}

        std::ostream& print(std::ostream& os) const
        {
            return os;
        }

        bool cmp(data_node_templ<_NodeData> const& rhs) const
        {
            return rhs.cmp_no_data(*this);
        }

        bool cmp_no_data(nothing_node_templ<_NodeData> const&) const
        {
            return true;
        }
    };

    template <typename _NodeData>
    struct string_node_templ
        : public data_node_templ<_NodeData>
    {
        std::string const data;

        string_node_templ(node_type const& type_img, _NodeData const& sub_data, std::string const& d)
            : data_node_templ<_NodeData>(type_img, sub_data)
            , data(d)
        {}

        std::ostream& print(std::ostream& os) const
        {
            return os << " string data: " << data;
        }

        bool cmp(data_node_templ<_NodeData> const& rhs) const
        {
            return rhs.cmp_str_data(*this);
        }

        bool cmp_str_data(string_node_templ<_NodeData> const& lhs) const
        {
            return data == lhs.data;
        }
    };

}

template <typename _NodeData>
std::ostream& operator<<(std::ostream& os, test::data_node_templ<_NodeData> const& node)
{
    return node.print(os <<  "node type: " << node.type_img.type_img) << std::endl
                                                                      << ":: sub data: " << node.sub_data;
}

#endif /* __STACKENING_TEST_DATA_NODE_H__ */
