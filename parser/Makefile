WORKDIR=parser
AWK=gawk -f

include misc/mf-template.mk

parser:yy-misc.d lexical syntactically err-report.d

syntactically:
	$(RESOLVE_DEP) $(WORKDIR)/syn-include.h | $(AWK) $(WORKDIR)/syn-script > $(MKTMP)
	make -f $(MKTMP)

lexical:syntactically
	$(RESOLVE_DEP) $(WORKDIR)/lex-include.h | $(AWK) $(WORKDIR)/lex-script > $(MKTMP)
	make -f $(MKTMP)

clean:
	rm -f $(WORKDIR)/syn.cpp
	rm -f $(WORKDIR)/syn.hpp
	rm -f $(WORKDIR)/lex.cpp
	rm -f $(WORKDIR)/*.o
