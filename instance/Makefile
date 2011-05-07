WORKDIR=instance

include misc/mf-template.mk

instance:type.d \
         variable.d \
         operation.d \
         function.d \
         symbol-table.d \
         scope.d \
         expr-nodes.d \
         stmt-nodes.d \
         built-in.d \
         block.d

clean:
	rm -f $(WORKDIR)/*.o
