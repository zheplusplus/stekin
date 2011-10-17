WORKDIR=proto

include misc/mf-template.mk

proto:type.d \
      variable.d \
      built-in.d \
      block.d \
      node-base.d \
      stmt-nodes.d \
      expr-nodes.d \
      list-pipe.d \
      operation.d \
      symbol-table.d \
      func-reference-type.d \
      list-types.d \
      func-inst-draft.d \
      function.d

clean:
	rm -f $(WORKDIR)/*.o
