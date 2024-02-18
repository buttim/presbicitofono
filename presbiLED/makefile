SDCCOPTS ?= --iram-size 128

SRC=presbiLED.c iap.c ws2812b.c
#SRC = $(wildcard *.c)
OBJ=$(patsubst %.c,build/%.rel, $(SRC))

build/%.rel: %.c
	mkdir -p $(dir $@)
	sdcc $(SDCCOPTS) -o build/ -c $<

all: presbiLED

presbiLED: $(OBJ)
	sdcc -o build/ $(SDCCOPTS) $^
	cp build/$@.ihx $@.hex

clean:
	rm -f *.ihx *.hex *.bin
	rm -f build/*
