WORKDIR=proto
TESTDIR=$(WORKDIR)/test

test:$(TESTDIR)/test-proto.out
	$(TESTDIR)/test-proto.out

include $(WORKDIR)/Makefile

WORK_OBJ=$(WORKDIR)/scope.o \
         $(WORKDIR)/general-scope.o \
         $(WORKDIR)/sub-scope.o \
         $(WORKDIR)/block.o \
         $(WORKDIR)/function.o \
         $(WORKDIR)/expr-nodes.o \
         $(WORKDIR)/stmt-nodes.o \
         $(WORKDIR)/inst-mediates.o \
         $(WORKDIR)/symbol-table.o

TEST_DEP=proto \
         phony-instance.dt \
         test-common.dt \
         test-general-scope.dt \
         test-func-and-call.dt
TEST_OBJ=$(WORK_OBJ) \
         $(TESTDIR)/phony-instance.o \
         $(TESTDIR)/test-common.o

$(TESTDIR)/test-proto.out:$(TEST_DEP) \
                          test-symbol-table.dt \
                          test-expr-nodes.dt
	$(LINK) $(TEST_OBJ) \
	        $(TESTDIR)/test-symbol-table.o \
	        $(TESTDIR)/test-expr-nodes.o \
	        $(TESTDIR)/test-general-scope.o \
	        $(TESTDIR)/test-func-and-call.o \
	        $(TEST_LIBS) \
	     -o $(TESTDIR)/test-proto.out

cleant:clean
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
