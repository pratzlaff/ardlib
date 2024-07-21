SYS = $(shell ./lc_uname)
CC = gcc
CFLAGS = -W -Wall -I$(ASCDS_INSTALL)/include
LDFLAGS = -L$(ASCDS_INSTALL)/ots/lib -L$(ASCDS_LIB) -lardlib -lascdm -lcaldb4 -lNewHdr2 -lcxcparam -lregion -lreadline -lhistory -lstk -lcfitsio -lm

all: ardlib_qe.$(SYS) ardlib_eff.$(SYS) ardlib_ea.$(SYS)

%.$(SYS) : %.o.$(SYS) ardlib_common.o.$(SYS)
	$(CC) $(CFLAGS_$(SYS)) -o $(@) $^ $(LDFLAGS)

%.o.$(SYS) : %.c
	$(CC) -c $(CFLAGS_$(SYS)) $(CFLAGS) $(CFLAGS) -o $(@) $<
