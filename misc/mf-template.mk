CC=g++ -c -std=c++0x
RESOLVE_DEP=g++ -MM
LINK=g++ $(LIBS)

CFLAGS=-Wall
MKTMP=tmp.mk

LIBS=-lgmp -lgmpxx

TEST_LIBS=$(LIBS) -lgtest -lgtest_main -lpthread

%.d:$(WORKDIR)/%.cpp
	echo -n "$(WORKDIR)/" > $(MKTMP)
	$(RESOLVE_DEP) $< >> $(MKTMP)
	echo "	$(CC) $< $(CFLAGS) -o $(WORKDIR)/$*.o" >> $(MKTMP)
	make -f $(MKTMP)

%.dt:$(TESTDIR)/%.cpp
	echo -n "$(TESTDIR)/" > $(MKTMP)
	$(RESOLVE_DEP) $< >> $(MKTMP)
	echo "	$(CC) $< $(CFLAGS) -o $(TESTDIR)/$*.o" >> $(MKTMP)
	make -f $(MKTMP)
