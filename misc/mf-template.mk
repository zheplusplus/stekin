CC=g++ -c -std=c++0x
RESOLVE_DEP=g++ -MM
LINK=g++
AR=ar rcs

CFLAGS=-Wall -Wextra -Wold-style-cast -Werror
MKTMP=tmp.mk

LIB_DIR=libs
LIBS=-L$(LIB_DIR) -lstkn -lgmp -lgmpxx
TEST_LIBS=-L$(LIB_DIR) -lstkntest -lstkn -lgtest -lgtest_main -lpthread -lgmp -lgmpxx

SAMPLEDIR=samples
ERRSAMPLEDIR=$(SAMPLEDIR)/errors
CHEKC_MEMONRY=valgrind -q --leak-check=full

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
