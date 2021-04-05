

CC := avr-gcc
CFLAGS := -Wall -Os -mmcu=attiny45

SOURCEFILE := "cc_program.c"

.PHONY: clean all

all: bin hex

bin:
	$(CC) -w -o output.bin $(SOURCEFILE) $(CFLAGS)


hex: bin
	avr-objcopy -j .text -j .data -O ihex output.bin output.hex


upload: hex
	avrdude -p t45 -P /dev/ttyUSB0 -c avrisp -b 19200 -U output.hex

clean:
	rm output.hex output.bin
