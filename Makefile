CC := /usr/bin/g++
CFLAGS := -O2 -mtune=native -march=native
CFLAGS += $(shell pkg-config --cflags --libs opencv4)

ifdef DEBUG
	CFLAGS += -Og -g
endif

srcs := $(wildcard *.cpp)
target := a.out

$(target): $(srcs)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm $(target)

.PHONY: clean
