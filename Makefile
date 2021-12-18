CC := /usr/bin/g++

ifdef DEBUG
	CFLAGS := -Og -g
else
	CFLAGS := -O2 -mtune=native -march=native
endif

srcs := $(wildcard *.cpp)
target := a.out

$(target): $(srcs)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm $(target)

.PHONY: clean
