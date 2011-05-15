WORKDIR=proto
TESTDIR=$(WORKDIR)/test

test:$(TESTDIR)/test-proto.out
	$(CHEKC_MEMONRY) $(TESTDIR)/test-proto.out

include $(WORKDIR)/Makefile

TEST_DEP=proto \
         phony-instance.dt \
         test-common.dt \
         test-operation-query.dt \
         test-symbol-table.dt \
         test-func-and-call.dt \
         test-stmt-nodes.dt \
         test-expr-nodes.dt
TEST_OBJ=$(WORKDIR)/*.o \
         $(TESTDIR)/phony-instance.o \
         $(TESTDIR)/test-common.o

$(TESTDIR)/test-proto.out:$(TEST_DEP)
	$(LINK) $(TEST_OBJ) \
	        $(TESTDIR)/test-operation-query.o \
	        $(TESTDIR)/test-symbol-table.o \
	        $(TESTDIR)/test-func-and-call.o \
	        $(TESTDIR)/test-expr-nodes.o \
	        $(TESTDIR)/test-stmt-nodes.o \
	        $(TEST_LIBS) \
	     -o $(TESTDIR)/test-proto.out

cleant:clean
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
