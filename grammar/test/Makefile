WORKDIR=grammar
TESTDIR=$(WORKDIR)/test

test:$(TESTDIR)/test-grammar.out
	$(TESTDIR)/test-grammar.out

include $(WORKDIR)/Makefile

WORK_OBJ=$(WORKDIR)/acceptor.o \
         $(WORKDIR)/clause-builder.o \
         $(WORKDIR)/expr-nodes.o \
         $(WORKDIR)/stmt-nodes.o

TEST_DEP=grammar phony-err-report.dt phony-proto.dt test-common.dt
TEST_OBJ=$(WORK_OBJ) $(TESTDIR)/phony-err-report.o $(TESTDIR)/phony-proto.o $(TESTDIR)/test-common.o

$(TESTDIR)/test-grammar.out:$(TEST_DEP) test-acceptor.dt test-expr-nodes.dt test-stmt-nodes.dt
	$(LINK) $(TEST_LIBS) $(TEST_OBJ) \
	        $(TESTDIR)/test-acceptor.o \
	        $(TESTDIR)/test-expr-nodes.o \
	        $(TESTDIR)/test-stmt-nodes.o \
	     -o $(TESTDIR)/test-grammar.out

cleant:clean
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
