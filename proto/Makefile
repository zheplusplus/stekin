WORKDIR=proto

include misc/mf-template.mk

proto:scope.d \
      built-in.d \
      block.d \
      stmt-nodes.d \
      expr-nodes.d \
      func-reference-type.d \
      function.d

clean:
	rm -f $(WORKDIR)/*.o
