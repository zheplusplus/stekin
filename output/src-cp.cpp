struct _stk_type_bool {
    _stk_type_1_byte boolean;

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

template <typename _T>
void push(void* mem, int offset, _T const& value)
{
    *(_T*)(offset + (_stk_type_1_byte*)(mem)) = value;
}

template <int _Size>
struct _stk_composite {
    _stk_type_1_byte mem[_Size];

    _stk_composite() {}

    _stk_composite(_stk_composite const& rhs)
    {
        operator=(rhs);
    }

    _stk_composite const& operator=(_stk_composite const& rhs)
    {
        std::copy(rhs.mem, rhs.mem + _Size, mem);
        return *this;
    }

    template <typename _T>
    _stk_composite push(int offset, _T const& value)
    {
        ::push(mem, offset, value);
        return *this;
    }
};

template <>
struct _stk_composite<0> {
    _stk_composite() {}
    _stk_composite(_stk_composite const&) {}

    _stk_composite const& operator=(_stk_composite const&)
    {
        return *this;
    }
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

template <int _Size>
std::ostream& operator<<(std::ostream& os, _stk_composite<_Size> const& composite)
{
    return os << "Stekin Composite (" << _Size << ')';
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
    void push(int offset, _T const& value)
    {
        ::push(_stk_ext_bases[_Level], offset, value);
    }

    void push(int, _stk_type_void) {}

    void* this_base() const
    {
        return _stk_ext_bases[_Level];
    }

    void* _stk_ext_bases[_Level + 1];
};

