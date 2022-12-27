# commands/Makefile

#ENTRYPOINT	= 0x1000
HD		= ../100m.img

ASM		= nasm
DASM		= objdump
CC		= gcc
LD		= ld
ASMFLAGS	= -I ../include/ -f elf
CFLAGS		= -I ../include/ -c -fno-builtin -fno-stack-protector -Wall --std=c11
LDFLAGS		= -Ttext 0x1000
DASMFLAGS	= -D
LIB		= ../lib/orangescrt.a
BIN		= echo pwd

# our changes
BIN += mm_test
BIN += ls print 24point
BIN += stackoverflow poc
BIN += hello infect readelf
BIN += poc2

# All Phony Targets
.PHONY : everything final clean realclean disasm all install

# Default starting position
everything : $(BIN)

install : all clean
	cp ../boot/hdldr.bin ./ -v
	cp ../kernel.bin ./ -v
	tar vcf inst.tar kernel.bin $(BIN) hdldr.bin
	dd if=inst.tar of=$(HD) seek=`echo "obase=10;ibase=16;(\`egrep -e '^ROOT_BASE' ../boot/include/load.inc | sed -e 's/.*0x//g'\`+\`egrep -e '#define[[:space:]]*INSTALL_START_SECT' ../include/sys/config.h | sed -e 's/.*0x//g'\`)*200" | bc` bs=1 count=`ls -l inst.tar | awk -F " " '{print $$5}'` conv=notrunc

all : realclean everything

final : all clean

clean :
	rm -f *.o

realclean :
	rm -f $(BIN) *.o inst.tar *.bin

kernel.bin :
	cp ../kernel.bin ./

start.o : start.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

echo.o: echo.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

echo : echo.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

pwd.o: pwd.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

pwd : pwd.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

mm_test.o: mm_test.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

mm_test: mm_test.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

stackoverflow.o: stackoverflow.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

stackoverflow: stackoverflow.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

hello.o: hello.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

hello: hello.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

print.o: print.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

print : print.o start.o $(LIB)
	 $(LD) $(LDFLAGS) -o $@ $?

24point.o: 24point.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

24point : 24point.o start.o $(LIB)
	 $(LD) $(LDFLAGS) -o $@ $?

ls.o: ls.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

ls : ls.o start.o $(LIB)
	 $(LD) $(LDFLAGS) -o $@ $?

infect.o: infect.c ../include/type.h ../include/stdio.h ../include/elf.h ../include/string.h
	$(CC) $(CFLAGS) -o $@ $<

infect : infect.o start.o $(LIB)
	 $(LD) $(LDFLAGS) -o $@ $?

readelf.o: readelf.c ../include/type.h ../include/stdio.h ../include/elf.h ../include/string.h
	$(CC) $(CFLAGS) -o $@ $<

readelf : readelf.o start.o $(LIB)
	 $(LD) $(LDFLAGS) -o $@ $?

poc.o: poc.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

poc : poc.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

poc2.o: poc2.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

poc2 : poc2.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?