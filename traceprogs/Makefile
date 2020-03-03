SRCS = simpleloop.c matmul.c blocked.c randomSum.c
PROGS = simpleloop matmul blocked randomSum

all : $(PROGS)

$(PROGS) : % : %.c
	gcc -Wall -g -o $@ $<


traces: $(PROGS)
	./runit simpleloop
	./runit matmul 100
	./runit blocked 100 25
	./runit randomSum

.PHONY: clean
clean : 
	rm -f simpleloop matmul blocked randomSum tr-*.ref *.marker *~
