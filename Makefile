include misc/mf-template.mk

all:
	make -f parser/Makefile
	make -f grammar/Makefile

test:
	make -f parser/test/Makefile

clean:
	make -f parser/Makefile clean
	make -f grammar/Makefile clean
	rm -f $(MKTMP)

cleant:clean
	make -f parser/test/Makefile cleant
