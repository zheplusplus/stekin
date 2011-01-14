WORKDIR=grammar
TESTDIR=$(WORKDIR)/test

test:$(TESTDIR)/test-grammar.out
	$(TESTDIR)/test-grammar.out

include $(WORKDIR)/Makefile

WORK_OBJ=$(WORKDIR)/acceptor.o \
         $(WORKDIR)/clause-builder.o \
         $(WORKDIR)/expr-nodes.o \
         $(WORKDIR)/stmt-nodes.o

TEST_DEP=grammar phony-err-report.dt phony-proto.dt
TEST_OBJ=$(WORK_OBJ) $(TESTDIR)/phony-err-report.o $(TESTDIR)/phony-proto.o

$(TESTDIR)/test-grammar.out:$(TEST_DEP) test-acceptor.dt
	$(LINK) $(TEST_LIBS) $(TEST_OBJ) \
	        $(TESTDIR)/test-acceptor.o \
	     -o $(TESTDIR)/test-grammar.out

cleant:clean
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
