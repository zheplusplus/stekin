WORKDIR=.

include misc/mf-template.mk

all:main.d head-writer.d lib checkout-subs
	make -f report/Makefile MODE=$(MODE)
	make -f parser/Makefile MODE=$(MODE)
	make -f grammar/Makefile MODE=$(MODE)
	make -f flowcheck/Makefile MODE=$(MODE)
	make -f proto/Makefile MODE=$(MODE)
	make -f instance/Makefile MODE=$(MODE)
	make -f output/Makefile MODE=$(MODE)
	make -f inspect/Makefile MODE=$(MODE)
	$(LINK) main.o \
	        report/*.o \
	        parser/*.o \
	        grammar/*.o \
	        flowcheck/*.o \
	        proto/*.o \
	        instance/*.o \
	        output/*.o \
	        $(LIBS) \
	     -o stkn-core.out
	$(LINK) head-writer.o -o head-writer.out $(LIBS)

lib:checkout-subs
	mkdir -p libs
	make -f util/Makefile MODE=$(MODE)
	make -f misc/Makefile MODE=$(MODE)
	make -f backtracpp/Makefile LIB_DIR=libs REL_PATH=backtracpp

runtest:all test-lib checkout-subs
	make -f util/test/Makefile MODE=$(MODE)
	make -f parser/test/Makefile MODE=$(MODE)
	make -f grammar/test/Makefile MODE=$(MODE)
	make -f flowcheck/test/Makefile MODE=$(MODE)
	make -f proto/test/Makefile MODE=$(MODE)
	make -f instance/test/Makefile MODE=$(MODE)
	./sample-test.sh

test-lib:checkout-subs
	mkdir -p libs
	make -f test/Makefile MODE=$(MODE)

checkout-subs:
	test ! -b backtracpp || git clone git@github.com:neuront/backtracpp.git

clean:
	make -f util/Makefile clean
	make -f misc/Makefile clean
	make -f report/Makefile clean
	make -f parser/Makefile clean
	make -f grammar/Makefile clean
	make -f flowcheck/Makefile clean
	make -f proto/Makefile clean
	make -f instance/Makefile clean
	make -f output/Makefile clean
	make -f inspect/Makefile clean
	rm -f $(MKTMP)
	rm -f $(UTILDIR)/*.o
	rm -f tmp.*
	rm -f *.o
	rm -f *.out
	rm -rf $(LIB_DIR)

cleant:clean
	make -f test/Makefile clean
	make -f util/test/Makefile cleant
	make -f parser/test/Makefile cleant
	make -f grammar/test/Makefile cleant
	make -f flowcheck/test/Makefile cleant
	make -f instance/test/Makefile cleant
	make -f proto/test/Makefile cleant
