WORKDIR=.

include misc/mf-template.mk

all:main.d head-writer.d libs
	make -f report/Makefile
	make -f parser/Makefile
	make -f grammar/Makefile
	make -f flowcheck/Makefile
	make -f proto/Makefile
	make -f instance/Makefile
	make -f output/Makefile
	$(LINK) main.o \
	        report/*.o \
	        parser/*.o \
	        grammar/*.o \
	        flowcheck/*.o \
	        proto/*.o \
	        instance/*.o \
	        output/*.o \
	        $(LIBS) \
	     -o stk-core.out
	$(LINK) head-writer.o -o head-writer.out $(LIBS)

libs:
	mkdir -p libs
	make -f util/Makefile
	make -f misc/Makefile

runtest:all
	make -f test/Makefile
	make -f util/test/Makefile
	make -f parser/test/Makefile
	make -f grammar/test/Makefile
	make -f flowcheck/test/Makefile
	make -f proto/test/Makefile
	make -f instance/test/Makefile
	./sample-test

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
