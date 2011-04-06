WORKDIR=flowcheck
TESTDIR=$(WORKDIR)/test

test:$(TESTDIR)/test-flowcheck.out
	$(TESTDIR)/test-flowcheck.out

include $(WORKDIR)/Makefile
include test/Makefile

TEST_DEP=flowcheck test-commons phony-proto.dt test-common.dt
TEST_OBJ=$(WORKDIR)/*.o $(TEST_COMMON_OBJ) $(TESTDIR)/phony-proto.o $(TESTDIR)/test-common.o

$(TESTDIR)/test-flowcheck.out:$(TEST_DEP) \
                              test-expr-nodes.dt
	$(LINK) $(TEST_OBJ) \
	        $(TESTDIR)/test-expr-nodes.o \
	        $(TEST_LIBS) \
	     -o $(TESTDIR)/test-flowcheck.out

cleant:clean
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
