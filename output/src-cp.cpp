#include <algorithm>
#include <iostream>

typedef int _stk_type_int;
typedef double _stk_type_float;

struct _stk_type_bool {
    char boolean;

    _stk_type_bool& operator=(_stk_type_bool const& rhs)
    {
        boolean = rhs.boolean;
        return *this;
    }

    operator bool() const
    {
        return boolean;
    }

    _stk_type_bool(bool b)
        : boolean(b)
    {}
};

std::ostream& operator<<(std::ostream& os, _stk_type_bool const& b)
{
    return os << (0 == b.boolean ? "false" : "true");
}

struct _stk_type_void {
    _stk_type_void& operator=(_stk_type_void const&)
    {
        return *this;
    }
};

std::ostream& operator<<(std::ostream& os, _stk_type_void)
{
    return os;
}

template <int _Level>
struct _stk_frame_bases {
    template <int _ExtLevel>
    _stk_frame_bases(_stk_frame_bases<_ExtLevel> const& ext_bases, void* this_base)
    {
        std::copy(ext_bases._stk_ext_bases, ext_bases._stk_ext_bases + _Level, _stk_ext_bases);
        _stk_ext_bases[_Level] = this_base;
    }

    explicit _stk_frame_bases(void* this_base)
    {
        _stk_ext_bases[_Level] = this_base;
    }

    template <typename _T>
    void push(int offset, _T value)
    {
        *(_T*)(((char*)_stk_ext_bases[_Level]) + offset) = value;
    }

    void push(int, _stk_type_void) {}

    void* this_base() const
    {
        return _stk_ext_bases[_Level];
    }

    void* _stk_ext_bases[_Level + 1];
};

