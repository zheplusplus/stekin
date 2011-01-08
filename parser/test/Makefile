WORKDIR=parser
TESTDIR=$(WORKDIR)/test

WORK_DEP=parser
WORK_OBJ=$(WORKDIR)/yy-misc.o $(WORKDIR)/lex.o $(WORKDIR)/syn.o

TEST_DEP=$(WORK_DEP) phony-err-report.dt phony-grammar.dt data-node.dt data-trees.dt
TEST_OBJ=$(WORK_OBJ) \
         $(TESTDIR)/phony-err-report.o \
         $(TESTDIR)/phony-grammar.o \
         $(TESTDIR)/data-node.o \
         $(TESTDIR)/data-trees.o

TEST_SRC_CODE=$(TESTDIR)/empty.stkn \
              $(TESTDIR)/inv-chars.stkn \
              $(TESTDIR)/tab-as-ind.stkn \
              $(TESTDIR)/bad-indent.stkn \
              $(TESTDIR)/mix.stkn

test:test-empty test-err-tab-as-ind test-err-inv-chars test-err-bad-indent test-mix $(TEST_SRC_CODE)

test-empty:$(TESTDIR)/test-empty.out
	$(TESTDIR)/test-empty.out < $(TESTDIR)/empty.stkn

$(TESTDIR)/test-empty.out:$(TEST_DEP) test-empty.dt
	$(LINK) $(TEST_OBJ) $(TESTDIR)/test-empty.o $(TEST_LIBS) -o $(TESTDIR)/test-empty.out

test-err-tab-as-ind:$(TESTDIR)/test-err-tab-as-ind.out
	$(TESTDIR)/test-err-tab-as-ind.out < $(TESTDIR)/tab-as-ind.stkn

$(TESTDIR)/test-err-tab-as-ind.out:$(TEST_DEP) test-err-tab-as-ind.dt
	$(LINK) $(TEST_OBJ) $(TESTDIR)/test-err-tab-as-ind.o $(TEST_LIBS) -o $(TESTDIR)/test-err-tab-as-ind.out

test-err-inv-chars:$(TESTDIR)/test-err-inv-chars.out
	$(TESTDIR)/test-err-inv-chars.out < $(TESTDIR)/inv-chars.stkn

$(TESTDIR)/test-err-inv-chars.out:$(TEST_DEP) test-err-inv-chars.dt
	$(LINK) $(TEST_OBJ) $(TESTDIR)/test-err-inv-chars.o $(TEST_LIBS) -o $(TESTDIR)/test-err-inv-chars.out

test-err-bad-indent:$(TESTDIR)/test-err-bad-indent.out
	$(TESTDIR)/test-err-bad-indent.out < $(TESTDIR)/bad-indent.stkn

$(TESTDIR)/test-err-bad-indent.out:$(TEST_DEP) test-err-bad-indent.dt
	$(LINK) $(TEST_OBJ) $(TESTDIR)/test-err-bad-indent.o $(TEST_LIBS) -o $(TESTDIR)/test-err-bad-indent.out

test-mix:$(TESTDIR)/test-mix.out
	$(TESTDIR)/test-mix.out < $(TESTDIR)/mix.stkn

$(TESTDIR)/test-mix.out:$(TEST_DEP) test-mix.dt
	$(LINK) $(TEST_OBJ) $(TESTDIR)/test-mix.o $(TEST_LIBS) -o $(TESTDIR)/test-mix.out

include $(WORKDIR)/Makefile

cleant:clean
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
