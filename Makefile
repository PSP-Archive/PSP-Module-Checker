all:
	rm *.SFO
	rm *.ELF
	rm *.o
	rm *.PRX
	psp-packer EBOOT.PBP

TARGET = checker
OBJS = main.o imports.o menu.o common.o
 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c 

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = PSP Module Checker 4.0 (Final)

BUILD_PRX = 1

LIBS = -lpspkubridge -lpspsystemctrl_user -lpsppower -lpspwlan -lpspreg
LDFLAGS = 

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak