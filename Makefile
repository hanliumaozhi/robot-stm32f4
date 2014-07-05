PROJ_NAME=main

SRCS = src/main.c \
       src/system_stm32f4xx.c \
       src/startup_stm32f4xx.s \
	   $(STM32F4LIB)/Libraries/STM32F4xx_StdPeriph_Driver/src/misc.c \
	   $(STM32F4LIB)/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c \
	   $(STM32F4LIB)/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
	   $(STM32F4LIB)/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c \
	   $(STM32F4LIB)/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c \
	   $(STM32F4LIB)/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c \
	   $(STM32F4LIB)/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c


CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

OBJDIR = build

STDLIBPATH=$(STM32F4LIB)

STDLIBSRC=$(STM32F4LIB)/Libraries/STM32F4xx_StdPeriph_Driver/src/
#STDLIBINCLUDES=-I"$(STDLIBPATH)/Libraries/CMSIS/Include" -I"$(STDLIBPATH)/Libraries/CMSIS/ST/STM32F4xx/Include" -I"$(STDLIBPATH)/Libraries/STM32F4xx_StdPeriph_Driver/inc"



CFLAGS  = -g -Wall -Wno-missing-braces -std=c99
CFLAGS += -mthumb -mcpu=cortex-m4
#CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
#CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -mfloat-abi=soft
LDFLAGS = -Wl,-Map,$(OBJDIR)/$(PROJ_NAME).map -g -Tstm32f4_flash.ld

CFLAGS += -Iinclude -I"$(STDLIBPATH)/Libraries/CMSIS/Include" -I"$(STDLIBPATH)/Libraries/CMSIS/ST/STM32F4xx/Include" -I"$(STDLIBPATH)/Libraries/STM32F4xx_StdPeriph_Driver/inc"

OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
OBJS := $(addprefix $(OBJDIR)/,$(OBJS))


all: $(OBJDIR)/$(PROJ_NAME).elf $(OBJDIR)/$(PROJ_NAME).hex $(OBJDIR)/$(PROJ_NAME).bin

$(OBJDIR)/%.elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.hex: %.elf
	$(OBJCOPY) -O ihex $^ $@

%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.s
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR):
	mkdir -p $@

clean:
	rm -f $(OBJDIR)/$(PROJ_NAME).elf
	rm -f $(OBJDIR)/$(PROJ_NAME).hex
	rm -f $(OBJDIR)/$(PROJ_NAME).bin
	rm -f $(OBJDIR)/$(PROJ_NAME).map
	find $(OBJDIR) -type f -name '*.o' -print0 | xargs -0 -r rm


program: $(OBJDIR)/$(PROJ_NAME).elf
	openocd -f program.cfg


$(OBJDIR)/$(PROJ_NAME).elf: $(OBJS) | $(OBJDIR)
