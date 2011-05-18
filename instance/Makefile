WORKDIR=instance

include misc/mf-template.mk

instance:expr-nodes.d \
         stmt-nodes.d \
         function.d \
         block.d \
         built-in.d

clean:
	rm -f $(WORKDIR)/*.o
