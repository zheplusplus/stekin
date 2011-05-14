#ifndef __STEKIN_INSTANCE_NODE_BASE_H__
#define __STEKIN_INSTANCE_NODE_BASE_H__

namespace inst {

    struct Expression {
        virtual ~Expression() {}

        virtual void write() const = 0;
    protected:
        Expression() {}
    };

    struct Statement {
        virtual ~Statement() {}

        virtual void write() const = 0;
    protected:
        Statement() {}
    };

}

#endif /* __STEKIN_INSTANCE_NODE_BASE_H__ */
