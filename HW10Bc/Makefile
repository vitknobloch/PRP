uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))
 
-include modules.mk
 
TARGET=tgraph_search-time
 
CFLAGS+=-std=c99 -O3 -g -march=native -pedantic -Wall -Werror -D_POSIX_C_SOURCE=200809L
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

pqr:
	$(CC) -c pq_rheap.h r_bucket.h pq_rheap.c r_bucket.c $(CFLAGS)

zip:
	zip hw10b.zip dijkstra.c dijkstra.h graph_utils.c graph_utils.h graph.c graph.h my_malloc.c my_malloc.h pq_rheap.h pq_rheap.c r_bucket.h r_bucket.c
