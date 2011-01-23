#ifndef __STACKENING_INSTANCE_FUNCTION_H__
#define __STACKENING_INSTANCE_FUNCTION_H__

#include <map>
#include <vector>
#include <list>
#include <string>

#include "stmt-nodes.h"
#include "type.h"
#include "inst-mediate.h"
#include "../misc/pos-type.h"

namespace inst {

    struct function {
        virtual ~function() {}

        virtual void set_return_type(misc::pos_type const& pos, type const* return_type);
        virtual type const* get_return_type() const;
        virtual bool is_return_type_resolved() const;
    public:
        std::vector<type const*> const arg_types;
        std::map<std::string, variable const> const external_vars;
    public:
        bool operator<(function const& rhs) const;
    public:
        static util::sref<function> create_instance(std::vector<type const*> const& arg_types
                                                  , std::map<std::string, variable const> const& extvars
                                                  , bool has_void_returns);

    protected:
        function(std::vector<type const*> const& atypes, std::map<std::string, variable const> const& extvars)
            : arg_types(atypes)
            , external_vars(extvars)
        {}
    protected:
        function(function const&) = delete;
    public:
        void add_path(util::sref<mediate_base> path);
        void inst_next_path(util::sref<scope const> sc);
    private:
        std::list<util::sref<mediate_base>> _candidate_paths;
    };

}

#endif /* __STACKENING_INSTANCE_FUNCTION_H__ */
