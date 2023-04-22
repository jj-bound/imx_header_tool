ROSS_COMPILE := aarch64-none-elf-
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump

CFLAGS := -I./common -I./imx8mq -DIMX8_MQ -g
OBJS := $(wildcard *.c ./common/*.c ./imx8mq/*.c)

all:$(OBJS)
	gcc $(CFLAGS) -o imx_tool $^
