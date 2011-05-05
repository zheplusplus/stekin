WORKDIR=proto

include misc/mf-template.mk

proto:scope.d \
      built-in.d \
      block.d \
      stmt-nodes.d \
      expr-nodes.d \
      function.d

clean:
	rm -f $(WORKDIR)/*.o
