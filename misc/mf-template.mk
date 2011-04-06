CC=g++ -c -std=c++0x
RESOLVE_DEP=g++ -MM
LINK=g++
AR=ar rcs

CFLAGS=-Wall -Wextra -Werror
MKTMP=tmp.mk

LIB_DIR=libs
LIBS=-lgmp -lgmpxx -L$(LIB_DIR) -lstkn
TEST_LIBS=-lgmp -lgmpxx -L$(LIB_DIR) -lstkntest -lstkn -lgtest -lgtest_main -lpthread

SAMPLEDIR=samples
ERRSAMPLEDIR=$(SAMPLEDIR)/errors

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
