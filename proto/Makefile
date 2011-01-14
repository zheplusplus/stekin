WORKDIR=proto

include misc/mf-template.mk

proto:symbol-table.d \
      node-factory.d \
      func-node-factories.d \
      scope.d \
      stmt-nodes.d \
      expr-nodes.d \
      func-templ.d

clean:
	rm -f $(WORKDIR)/*.o