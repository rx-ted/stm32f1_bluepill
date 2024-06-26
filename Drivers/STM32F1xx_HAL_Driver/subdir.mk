CURRENCT_DIR=Drivers/STM32F1xx_HAL_Driver
HAL_DIRS := $(shell find $(CURRENCT_DIR) -maxdepth 1 -type d )
HAL_HDRS := $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.h))

HAL_INCS := $(foreach dir, $(sort $(dir $(HAL_HDRS))), -I$(dir))
HAL_SRCS := $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.c))

# HAL_SRCS += $(CURRENCT_DIR)/Src/stm32f1xx_hal_can.c
