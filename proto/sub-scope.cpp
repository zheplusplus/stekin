#include "sub-scope.h"
#include "expr-nodes.h"
#include "symbol-table.h"
#include "function.h"
#include "../report/errors.h"

using namespace proto;

util::sptr<expr_base const> sub_scope::make_ref(misc::pos_type const& pos, std::string const& name)
{
    _symbols->ref_var(pos, name);
    return std::move(util::mkptr(new reference(pos, name)));
}

util::sptr<expr_base const> sub_scope::make_call(misc::pos_type const& pos
                                               , std::string const& name
                                               , std::vector<util::sptr<expr_base const>> args) const
{
    return std::move(util::mkptr(new call(pos, _symbols->query_func(pos, name, args.size()), std::move(args))));
}

void sub_scope::def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const>)
{
    error::forbid_def_var(pos, name);
}

util::sref<function> sub_scope::decl_func(misc::pos_type const&
                                        , std::string const&
                                        , std::vector<std::string> const&)
{
    return util::sref<function>(NULL);
}

util::sptr<scope> sub_scope::create_branch_scope()
{
    return std::move(util::mkmptr(new sub_scope(_symbols)));
}
