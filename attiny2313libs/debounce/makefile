PRG            = debounce
OBJ            = debounce.S
LIBNAME		   = debounce
PROGRAMMER     = usbasp
PORT           = usb
MCU_TARGET     = attiny2313
AVRDUDE_TARGET = attiny2313
OPTIMIZE       = -Os

HZ          = 8000000

# You should not have to change anything below here.

CC             = avr-gcc

# Override is only needed by avr-lib build system.

override ASFLAGS       = -mmcu=$(MCU_TARGET) -c -Wa,-adhlns=$(<:.S=.lst)

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).o 

$(PRG).o: $(OBJ)
	$(CC) $(ASFLAGS) -o $@ $^ $(LIBS)
	
library: 
	avr-ar rcs lib$(LIBNAME).a *.o
		
clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak *.hex *.bin *.srec
	rm -rf *.lst *.map *.a 

