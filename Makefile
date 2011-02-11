WORKDIR=.

include misc/mf-template.mk

all:main.d
	make -f util/Makefile
	make -f report/Makefile
	make -f parser/Makefile
	make -f grammar/Makefile
	make -f proto/Makefile
	make -f instance/Makefile
	make -f output/Makefile
	$(LINK) main.o util/*.o report/*.o parser/*.o grammar/*.o proto/*.o instance/*.o output/*.o -o stk-core.out

runtest:
	make -f test/Makefile
	make -f util/test/Makefile
	make -f parser/test/Makefile
	make -f grammar/test/Makefile
	make -f proto/test/Makefile
	make -f instance/test/Makefile

clean:
	make -f util/Makefile clean
	make -f report/Makefile clean
	make -f parser/Makefile clean
	make -f grammar/Makefile clean
	make -f proto/Makefile clean
	make -f instance/Makefile clean
	make -f output/Makefile clean
	rm -f $(MKTMP)
	rm -f $(UTILDIR)/*.o
	rm -f tmp.cpp
	rm -f main.o
	rm -f stk-core.out

cleant:
	make -f test/Makefile clean
	make -f util/test/Makefile cleant
	make -f parser/test/Makefile cleant
	make -f grammar/test/Makefile cleant
	make -f instance/test/Makefile cleant
	make -f proto/test/Makefile cleant
