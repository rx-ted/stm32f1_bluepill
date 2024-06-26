CURRENCT_DIR=HW
# HW_DIRS := $(shell find $(CURRENCT_DIR) -maxdepth 2 -type d -printf '%P ')
HW_DIRS := $(shell find $(CURRENCT_DIR) -maxdepth 2 -type d )

HW_SRCS := $(foreach dir, $(HW_DIRS), $(wildcard $(dir)/*.c))

HW_INCS := $(foreach dir, $(sort $(dir $(HW_SRCS))), -I$(dir))


# HW_INCS += -IHW/key
# HW_INCS += -IHW/led
# HW_INCS += -IHW/w25qxx
# HW_INCS += -IHW/usart
# HW_INCS += -IHW/lcd
#
# HW_SRCS += HW/key/key.c 
# HW_SRCS += HW/led/led.c 
# HW_SRCS += HW/w25qxx/w25qxx.c 
# HW_SRCS += HW/usart/usart.c 
# HW_SRCS += HW/lcd/lcd.c 
