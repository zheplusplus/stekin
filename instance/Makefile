WORKDIR=instance

include misc/mf-template.mk

instance:expr-nodes.d \
         stmt-nodes.d \
         built-in.d \
         block.d

clean:
	rm -f $(WORKDIR)/*.o
