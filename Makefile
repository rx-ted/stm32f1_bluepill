# project name
TARGET_NAME=bluepill
OUTPUT_DIR = output
BUILD_DIR = build
TARGET=$(OUTPUT_DIR)/$(TARGET_NAME)

CROSS_COMPILE=arm-none-eabi-
#CROSS_COMPILE =/opt/arm-2011.03/bin/arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
AS=$(CROSS_COMPILE)as
OC=$(CROSS_COMPILE)objcopy
OD=$(CROSS_COMPILE)objdump
SZ=$(CROSS_COMPILE)size

DEFFLAGS=-DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB


FLAGS= $(DEFFLAGS) -c \
	-mcpu=cortex-m3 \
	-g3 --specs=nano.specs -mfloat-abi=soft \
	-mthumb  \
	-MMD -MP -MF"$(@:%.o=%.d)" \
	-MT"$@" 


CFLAGS= $(FLAGS) \
	-std=c11 -O0 \
	-ffunction-sections \
	-fdata-sections -Wall\
	-fstack-usage 

SFLAGS= $(FLAGS) -DDEBUG \
	-x assembler-with-cpp



LDSCRIPT=Startup/STM32F103C8TX_FLASH.ld
LDFLAGS	= -mcpu=cortex-m3 -T"$(LDSCRIPT)" --specs=nosys.specs -Wl,-Map="$(TARGET).map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group


include Core/subdir.mk
include HW/subdir.mk
include Drivers/subdir.mk
include Startup/subdir.mk
# if you need to expand other library, pls add the path here.


INCLUDE = \
		$(HW_INCS) \
	$(FW_INCS) \
		$(CORE_INCS) \
		$(CMSIS_INCS) \
		$(HAL_INCS) \

# this's weird! why cann't list the static library? Not automatically!
# arm-gcc static include
INCLUDE += -I/usr/lib/gcc/arm-none-eabi/10.3.1/include 
INCLUDE += -I/usr/lib/gcc/arm-none-eabi/10.3.1/include-fixed 
INCLUDE += -I/usr/lib/gcc/arm-none-eabi/10.3.1/../../../arm-none-eabi/include

SRCS = \
	$(FW_SRCS) \
	$(HW_SRCS) \
	$(CORE_SRCS) \
	$(HAL_SRCS) \


.PHONY : clean all

all: $(TARGET).bin $(TARGET).hex  $(TARGET).list

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
vpath %.c $(sort $(dir $(SRCS)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))


clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(OUTPUT_DIR)/*

$(OUTPUT_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

$(TARGET).list: $(TARGET).elf 
	@echo '[LIST] Finished building: $@'
	@$(OD) -h -S "$<" > "$@" 

$(TARGET).hex: $(TARGET).elf 
	@echo '[HEX] Finished building: $@'
	@$(OC) -Oihex "$<" "$@"

$(TARGET).bin: $(TARGET).elf 
	@echo '[BIN] Finished building: $@'
	@$(OC) -Obinary "$<" "$@"

$(TARGET).elf: $(OBJECTS) Makefile | $(OUTPUT_DIR)
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	@echo '[ELF] Finished building: $@'
	@$(SZ) "$@"

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo "[CC] $<" 
	@$(CC) $(INCLUDE) $(CFLAGS)  "$<" -o "$@" 

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo "[AS] $<" 
	@$(CC) $(INCLUDE) $(SFLAGS)  "$<" -o "$@"

# Usually use openocd in the stlink to write image.
flash:
	openocd  \
		-f /usr/share/openocd/scripts/interface/stlink.cfg \
		-f /usr/share/openocd/scripts/target/stm32f1x.cfg \
		-c init -c halt \
		-c "flash write_image erase $(TARGET).elf" \
		-c reset -c shutdown

# /dev/ttyXXXX, need to list /dev/tty* to get driver name.
upload:
	sudo stm32flash -w $(TARGET).bin -v -g 0x00 /dev/ttyCH3410

