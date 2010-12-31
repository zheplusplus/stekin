#ifndef __STACKENING_PARSER_SYNTAX_TYPE_H__
#define __STACKENING_PARSER_SYNTAX_TYPE_H__

#include <string>
#include <list>
#include <vector>

#include "err-report.h"
#include "../grammar/expr-nodes.h"
#include "../misc/pos-type.h"

namespace parser {

    struct op_img {
        std::string const img;

        explicit op_img(char const* image)
            : img(image)
        {}
    };

    struct identifier {
        misc::pos_type const pos;
        std::string const id;

        identifier(misc::pos_type const& ps, char const* id_text)
            : pos(ps)
            , id(id_text)
        {}
    };

    struct param_names {
        param_names* add(std::string const& name)
        {
            _names.push_back(name);
            return this;
        }

        std::list<std::string>::const_iterator begin() const
        {
            return _names.begin();
        }

        std::list<std::string>::const_iterator end() const
        {
            return _names.end();
        }
    private:
        std::list<std::string> _names;
    };

    struct arg_list {
        arg_list* add(grammar::expr_base const* expr)
        {
            _params.push_back(expr);
            return this;
        }

        std::list<grammar::expr_base const*>::const_iterator begin() const
        {
            return _params.begin();
        }

        std::list<grammar::expr_base const*>::const_iterator end() const
        {
            return _params.end();
        }
    private:
        std::list<grammar::expr_base const*> _params;
    };

}

#endif /* __STACKENING_PARSER_SYNTAX_TYPE_H__ */
