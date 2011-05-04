WORKDIR=instance
TESTDIR=$(WORKDIR)/test

test:$(TESTDIR)/test-instance.out
	$(CHEKC_MEMONRY) $(TESTDIR)/test-instance.out

include $(WORKDIR)/Makefile

TEST_DEP=instance test-common.dt phony-output.dt
TEST_OBJ=$(WORKDIR)/*.o \
         $(TESTDIR)/test-common.o \
         $(TESTDIR)/phony-output.o

$(TESTDIR)/test-instance.out:$(TEST_DEP) \
                             test-expr-nodes.dt \
                             test-stmt-nodes.dt \
                             test-operation-query.dt \
                             test-symbol-table.dt \
                             test-function.dt
	$(LINK) $(TEST_OBJ) \
	        $(TESTDIR)/test-expr-nodes.o \
	        $(TESTDIR)/test-stmt-nodes.o \
	        $(TESTDIR)/test-operation-query.o \
	        $(TESTDIR)/test-symbol-table.o \
	        $(TESTDIR)/test-function.o \
	        $(TEST_LIBS) \
	     -o $(TESTDIR)/test-instance.out

cleant:clean
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
