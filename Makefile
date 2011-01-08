include misc/mf-template.mk

all:
	make -f parser/Makefile
	make -f grammar/Makefile
	make -f instance/Makefile

test:
	make -f parser/test/Makefile
	make -f instance/test/Makefile

clean:
	make -f parser/Makefile clean
	make -f grammar/Makefile clean
	make -f instance/Makefile clean
	rm -f $(MKTMP)

cleant:clean
	make -f parser/test/Makefile cleant
	make -f instance/test/Makefile cleant
