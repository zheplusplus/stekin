WORKDIR=grammar
TESTDIR=$(WORKDIR)/test

test:$(TESTDIR)/test-grammar.out
	$(TESTDIR)/test-grammar.out

include $(WORKDIR)/Makefile
include test/Makefile

TEST_DEP=grammar test-commons phony-proto.dt test-common.dt
TEST_OBJ=$(WORKDIR)/*.o $(TEST_COMMON_OBJ) $(TESTDIR)/phony-proto.o $(TESTDIR)/test-common.o

$(TESTDIR)/test-grammar.out:$(TEST_DEP) \
                            test-expr-nodes.dt \
                            test-stmt-nodes.dt \
                            test-acceptor.dt \
                            test-clause-builder.dt
	$(LINK) $(TEST_LIBS) $(TEST_OBJ) \
	        $(TESTDIR)/test-expr-nodes.o \
	        $(TESTDIR)/test-stmt-nodes.o \
	        $(TESTDIR)/test-acceptor.o \
	        $(TESTDIR)/test-clause-builder.o \
	     -o $(TESTDIR)/test-grammar.out

cleant:clean
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
