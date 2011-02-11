WORKDIR=$(UTILDIR)
TESTDIR=$(UTILDIR)/test

include misc/mf-template.mk

test:$(TESTDIR)/test-utilities.out
	$(TESTDIR)/test-utilities.out

include $(UTILDIR)/Makefile

$(TESTDIR)/test-utilities.out:util test-map-compare.dt test-string.dt test-pointer.dt
	$(LINK) $(TEST_LIBS) $(UTILDIR)/*.o \
	        $(TESTDIR)/test-map-compare.o \
	        $(TESTDIR)/test-string.o \
	        $(TESTDIR)/test-pointer.o \
	     -o $(TESTDIR)/test-utilities.out

cleant:
	rm -f $(UTILDIR)/*.o
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTDIR)/*.out
