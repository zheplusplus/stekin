#ifndef __STEKIN_PARSER_SYNTAX_TYPE_H__
#define __STEKIN_PARSER_SYNTAX_TYPE_H__

#include <string>
#include <vector>

#include "../grammar/fwd-decl.h"
#include "../report/errors.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace parser {

    struct OpImage {
        std::string const img;

        explicit OpImage(char const* image)
            : img(image)
        {}
    };

    struct Identifier {
        misc::position const pos;
        std::string const id;

        Identifier(misc::position const& ps, char const* id_text)
            : pos(ps)
            , id(id_text)
        {}
    };

    struct ParamNames {
        ParamNames* add(std::string const& name)
        {
            _names.push_back(name);
            return this;
        }

        std::vector<std::string> get() const
        {
            return _names;
        }
    private:
        std::vector<std::string> _names;
    };

    struct ArgList {
        ArgList* add(grammar::Expression const* expr)
        {
            _params.push_back(std::move(util::mkptr(expr)));
            return this;
        }

        std::vector<util::sptr<grammar::Expression const>> deliver()
        {
            return std::move(_params);
        }
    private:
        std::vector<util::sptr<grammar::Expression const>> _params;
    };

}

#endif /* __STEKIN_PARSER_SYNTAX_TYPE_H__ */
