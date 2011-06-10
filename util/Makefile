WORKDIR=util

include misc/mf-template.mk

util:string.d pointer.d sn.d
	$(AR) $(LIB_DIR)/libstkn.a $(WORKDIR)/*.o

clean:
	rm -f $(WORKDIR)/*.o

