#ifndef __STEKIN_INSTANCE_ADDRESS_H__
#define __STEKIN_INSTANCE_ADDRESS_H__

namespace inst {

    struct Address {
        int const level;
        int const offset;

        Address(int l, int o)
            : level(l)
            , offset(o)
        {}
    };

}

#endif /* __STEKIN_INSTANCE_ADDRESS_H__ */
