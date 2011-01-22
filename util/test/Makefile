WORKDIR=$(UTILDIR)
TESTDIR=$(UTILDIR)/test

include misc/mf-template.mk

test:$(TESTDIR)/test-utilities.out
	$(TESTDIR)/test-utilities.out

TEST_DEP=string.du
TEST_OBJ=$(UTILDIR)/string.o

$(TESTDIR)/test-utilities.out:$(TEST_DEP) test-map-compare.dt test-string.dt
	$(LINK) $(TEST_LIBS) $(TEST_OBJ) $(TESTDIR)/test-map-compare.o $(TESTDIR)/test-string.o \
	     -o $(TESTDIR)/test-utilities.out

cleant:
	rm -f $(UTILDIR)/*.o
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
