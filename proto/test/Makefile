WORKDIR=proto
UTILDIR=util
TESTDIR=$(WORKDIR)/test

test:$(TESTDIR)/test-proto.out
	$(TESTDIR)/test-proto.out

include $(WORKDIR)/Makefile
include test/Makefile

WORK_OBJ=$(WORKDIR)/scope.o \
         $(WORKDIR)/block.o \
         $(WORKDIR)/function.o \
         $(WORKDIR)/expr-nodes.o \
         $(WORKDIR)/stmt-nodes.o \
         $(WORKDIR)/inst-mediates.o \
         $(WORKDIR)/symbol-table.o

UTIL_OBJ=$(UTILDIR)/string.o

TEST_DEP=proto \
         string.du \
         test-commons \
         phony-instance.dt \
         test-common.dt \
         test-scope.dt \
         test-func-and-call.dt
TEST_OBJ=$(WORK_OBJ) \
         $(UTILDIR)/string.o \
         $(TEST_COMMON_OBJ) \
         $(TESTDIR)/phony-instance.o \
         $(TESTDIR)/test-common.o

$(TESTDIR)/test-proto.out:$(TEST_DEP) \
                          test-symbol-table.dt \
                          test-expr-nodes.dt
	$(LINK) $(TEST_LIBS) $(TEST_OBJ) \
	        $(TESTDIR)/test-symbol-table.o \
	        $(TESTDIR)/test-expr-nodes.o \
	        $(TESTDIR)/test-scope.o \
	        $(TESTDIR)/test-func-and-call.o \
	     -o $(TESTDIR)/test-proto.out

cleant:clean
	rm -f $(UTILDIR)/string.o
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
