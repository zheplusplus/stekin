ifeq ($(MODE), inspect)
	OPT_FLAGS=
	DYN_LINK=-rdynamic
else
	OPT_FLAGS=-O3
	DYN_LINK=
endif

CC=g++ -c -std=c++0x
RESOLVE_DEP=g++ -MM
LINK=g++ $(DYN_LINK)
AR=ar rcs

CFLAGS=-Wall -Wextra -Wold-style-cast -Werror $(OPT_FLAGS)
MKTMP=tmp.mk

LIB_DIR=libs
MISC_LIBS=-lgmp -lgmpxx -ltrace -lbacktracpp
LIBS=-L$(LIB_DIR) -lstkn $(MISC_LIBS)
TEST_LIBS=-L$(LIB_DIR) -lstkntest -lstkn -lgtest -lgtest_main -lpthread $(MISC_LIBS)

SAMPLEDIR=samples
ERRSAMPLEDIR=$(SAMPLEDIR)/errors
CHEKC_MEMONRY=valgrind --log-file=tmp.log.memcheck --leak-check=full

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
