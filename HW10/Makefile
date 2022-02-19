uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))
 
-include modules.mk
 
TARGET=tgraph_search-time
 
CFLAGS+=-std=c99 -O3 -march=native -pedantic -Wall -Werror -D_POSIX_C_SOURCE=200809L
LDFLAGS+=-lm
 
SRC:=$(TARGET).c $(SRC)
SRC+=$(wildcard *.c)
 
OBJS:=$(patsubst %.c,%.o,$(call uniq,$(SRC)))
 
bin: $(TARGET)
 
$(OBJS): %.o: %.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@
 
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@ 
 
clean:
	$(RM) $(OBJS) $(TARGET)
