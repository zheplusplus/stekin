WORKDIR=instance
TESTDIR=$(WORKDIR)/test

test:$(TESTDIR)/test-instance.out
	$(CHEKC_MEMONRY) $(TESTDIR)/test-instance.out

include $(WORKDIR)/Makefile

TEST_DEP=instance \
         test-common.dt \
         test-function.dt \
         test-expr-nodes.dt \
         test-stmt-nodes.dt \
         test-built-in.dt \
         phony-output.dt
TEST_OBJ=$(WORKDIR)/*.o \
         $(TESTDIR)/test-common.o \
         $(TESTDIR)/test-function.o \
         $(TESTDIR)/test-expr-nodes.o \
         $(TESTDIR)/test-stmt-nodes.o \
         $(TESTDIR)/test-built-in.o \
         $(TESTDIR)/phony-output.o

$(TESTDIR)/test-instance.out:$(TEST_DEP)
	$(LINK) $(TEST_OBJ) \
	        $(TEST_LIBS) \
	     -o $(TESTDIR)/test-instance.out

cleant:clean
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
