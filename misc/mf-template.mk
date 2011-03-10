CC=g++ -c -std=c++0x
RESOLVE_DEP=g++ -MM
LINK=g++ $(LIBS)
UTILDIR=util

CFLAGS=-Wall -Wextra -Werror
MKTMP=tmp.mk

LIBS=-lgmp -lgmpxx
TEST_LIBS=-lgtest -lgtest_main -lpthread

SAMPLEDIR=samples
ERRSAMPLEDIR=$(SAMPLEDIR)/errors

%.d:$(WORKDIR)/%.cpp
	echo -n "$(WORKDIR)/" > $(MKTMP)
	$(RESOLVE_DEP) $< >> $(MKTMP)
	echo "	$(CC) $< $(CFLAGS) -o $(WORKDIR)/$*.o" >> $(MKTMP)
	make -f $(MKTMP)

%.du:$(UTILDIR)/%.cpp
	echo -n "$(UTILDIR)/" > $(MKTMP)
	$(RESOLVE_DEP) $< >> $(MKTMP)
	echo "	$(CC) $< $(CFLAGS) -o $(UTILDIR)/$*.o" >> $(MKTMP)
	make -f $(MKTMP)

%.dt:$(TESTDIR)/%.cpp
	echo -n "$(TESTDIR)/" > $(MKTMP)
	$(RESOLVE_DEP) $< >> $(MKTMP)
	echo "	$(CC) $< $(CFLAGS) -o $(TESTDIR)/$*.o" >> $(MKTMP)
	make -f $(MKTMP)

%.dtc:test/%.cpp
	echo -n "test/" > $(MKTMP)
	$(RESOLVE_DEP) $< >> $(MKTMP)
	echo "	$(CC) $< $(CFLAGS) -o test/$*.o" >> $(MKTMP)
	make -f $(MKTMP)
