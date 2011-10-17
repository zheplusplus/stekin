WORKDIR=instance

include misc/mf-template.mk

instance:node-base.d \
         expr-nodes.d \
         list-pipe.d \
         stmt-nodes.d \
         types.d \
         function.d \
         block.d \
         built-in.d

clean:
	rm -f $(WORKDIR)/*.o
