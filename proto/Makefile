WORKDIR=proto

include misc/mf-template.mk

proto:symbol-table.d \
      scope.d \
      global-scope.d \
      block.d \
      stmt-nodes.d \
      expr-nodes.d \
      inst-mediates.d \
      func-templ.d

clean:
	rm -f $(WORKDIR)/*.o
