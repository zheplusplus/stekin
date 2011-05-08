WORKDIR=instance

include misc/mf-template.mk

instance:type.d \
         variable.d \
         expr-nodes.d \
         stmt-nodes.d \
         built-in.d \
         block.d

clean:
	rm -f $(WORKDIR)/*.o
