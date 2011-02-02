WORKDIR=instance
TESTDIR=$(WORKDIR)/test

test:$(TESTDIR)/test-instance.out
	$(TESTDIR)/test-instance.out

include $(WORKDIR)/Makefile

WORK_OBJ=$(WORKDIR)/block.o \
         $(WORKDIR)/type.o \
         $(WORKDIR)/expr-nodes.o \
         $(WORKDIR)/operation.o \
         $(WORKDIR)/symbol-table.o \
         $(WORKDIR)/scope.o \
         $(WORKDIR)/function.o

TEST_DEP=$(WORK_DEP) instance phony-err-report.dt test-common.dt phony-output.dt
TEST_OBJ=$(WORK_OBJ) $(TESTDIR)/phony-err-report.o $(TESTDIR)/test-common.o $(TESTDIR)/phony-output.o

$(TESTDIR)/test-instance.out:$(TEST_DEP) test-operation-query.dt test-symbol-table.dt test-function.dt
	$(LINK) $(TEST_LIBS) $(TEST_OBJ) \
	        $(TESTDIR)/test-operation-query.o \
	        $(TESTDIR)/test-symbol-table.o \
	        $(TESTDIR)/test-function.o \
	     -o $(TESTDIR)/test-instance.out

cleant:clean
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
