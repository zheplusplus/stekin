WORKDIR=flowcheck

include misc/mf-template.mk

flowcheck:node-base.d \
          stmt-nodes.d \
          expr-nodes.d \
          filter.d \
          symbol-def-filter.d \
          func-body-filter.d \
          block.d \
          accumulator.d \
          function.d

clean:
	rm -f $(WORKDIR)/*.o
