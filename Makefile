CFLAGS := -std=c99 -ggdb0 \
	-Wall -Wextra \
	-Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
	-D_GNU_SOURCE \
	$(CFLAGS)

LDLIBS = -lnetlink

all: rmif addvlan

rmif: rmif.o
	$(LINK.o) $^ $(LDLIBS) -o $@

addvlan: addvlan.o
	$(LINK.o) $^ $(LDLIBS) -o $@

clean:
	$(RM) *.o
	$(RM) rmif addvlan

.PHONY: clean
