WORKDIR=instance
TESTDIR=$(WORKDIR)/test

test:$(TESTDIR)/test-instance.out
	$(TESTDIR)/test-instance.out

include $(WORKDIR)/Makefile
include test/Makefile

TEST_DEP=instance test-commons test-common.dt phony-output.dt
TEST_OBJ=$(WORKDIR)/*.o $(TEST_COMMON_OBJ) $(TESTDIR)/test-common.o $(TESTDIR)/phony-output.o

$(TESTDIR)/test-instance.out:$(TEST_DEP) test-operation-query.dt test-symbol-table.dt test-function.dt
	$(LINK) $(TEST_LIBS) $(TEST_OBJ) \
	        $(TESTDIR)/test-operation-query.o \
	        $(TESTDIR)/test-symbol-table.o \
	        $(TESTDIR)/test-function.o \
	     -o $(TESTDIR)/test-instance.out

cleant:clean
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
