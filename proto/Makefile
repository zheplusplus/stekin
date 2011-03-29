WORKDIR=proto

include misc/mf-template.mk

proto:symbol-table.d \
      scope.d \
      global-scope.d \
      sub-scope.d \
      general-scope.d \
      block.d \
      node-base.d \
      stmt-nodes.d \
      expr-nodes.d \
      inst-mediates.d \
      function.d

clean:
	rm -f $(WORKDIR)/*.o
