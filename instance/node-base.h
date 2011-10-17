#ifndef __STEKIN_INSTANCE_NODE_BASE_H__
#define __STEKIN_INSTANCE_NODE_BASE_H__

namespace inst {

    struct Expression {
        Expression() {}
        virtual ~Expression() {}

        virtual void write() const = 0;
        virtual void writePipeDef(int level) const;
    };

    struct Statement {
        Statement() {}
        virtual ~Statement() {}

        virtual void write() const = 0;
    };

}

#endif /* __STEKIN_INSTANCE_NODE_BASE_H__ */
