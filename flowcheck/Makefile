WORKDIR=flowcheck

include misc/mf-template.mk

flowcheck:stmt-nodes.d expr-nodes.d filter.d block.d accumulator.d function.d

clean:
	rm -f $(WORKDIR)/*.o
