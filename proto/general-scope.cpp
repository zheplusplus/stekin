#include "general-scope.h"
#include "expr-nodes.h"
#include "function.h"
#include "sub-scope.h"

using namespace proto;

util::sptr<expr_base const> general_scope::make_ref(misc::pos_type const& pos, std::string const& name)
{
    _symbols.ref_var(pos, name);
    return std::move(util::mkptr(new reference(pos, name)));
}

util::sptr<expr_base const> general_scope::make_call(misc::pos_type const& pos
                                                   , std::string const& name
                                                   , std::vector<util::sptr<expr_base const>> args) const
{
    return std::move(util::mkptr(new call(pos, _symbols.query_func(pos, name, args.size()), std::move(args))));
}

void general_scope::def_var(misc::pos_type const& pos
                          , std::string const& name
                          , util::sptr<expr_base const> init)
{
    _symbols.def_var(pos, name);
    _block.add_stmt(std::move(util::mkptr(new var_def(pos, name, std::move(init)))));
}

util::sref<function> general_scope::decl_func(misc::pos_type const& pos
                                            , std::string const& name
                                            , std::vector<std::string> const& param_names)
{
    return _symbols.def_func(pos, name, param_names);
}

util::sptr<scope> general_scope::create_branch_scope()
{
    return std::move(util::mkmptr(new sub_scope(util::mkref(_symbols))));
}
