WORKDIR=proto

include misc/mf-template.mk

proto:scope.d \
      variable.d \
      built-in.d \
      block.d \
      stmt-nodes.d \
      expr-nodes.d \
      operation.d \
      symbol-table.d \
      func-reference-type.d \
      func-inst-draft.d \
      function.d

clean:
	rm -f $(WORKDIR)/*.o
