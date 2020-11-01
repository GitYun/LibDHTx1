TARGET = LibDHTx1TestForMSC51STC12
CC = sdcc
CPP = sdcc

INCLUDE := -I./src -I../../../src/
CFLAGS = -std-c99 --debug --opt-code-speed --model-small --code-size 4096 -mmcs51 $(INCLUDE)
LFLAGS = --out-fmt-ihx --model-small --code-size 4096 -mmcs51

BUILD_OUT = out

OBJ_FILE := \
$(BUILD_OUT)/main.rel \
$(BUILD_OUT)/ser_ir.rel \
$(BUILD_OUT)/systimer.rel \
$(BUILD_OUT)/Lib.DHTx1.rel

SRC := $(wildcard *.c *.h)
SRC_PATH = ./src
DHTx1_SRC_PATH = ../../src

.SILENT:

all: prepare $(TARGET) after
	@echo 'Finished'

prepare:
	cmd /c if not exist $(BUILD_OUT) md $(BUILD_OUT)

after:
	packihx <$(BUILD_OUT)/$(TARGET).ihx >$(BUILD_OUT)/$(TARGET).hex
	makebin -p $(BUILD_OUT)/$(TARGET).ihx $(BUILD_OUT)/$(TARGET).bin
	# cmd /c del /f $(BUILD_OUT)/*.adb $(BUILD_OUT)/*.sym
	# rm -f $(BUILD_OUT)/*.adb $(BUILD_OUT)/*.sym

$(TARGET): $(OBJ_FILE)
	$(CC) $(LFLAGS) -o $(BUILD_OUT)/$@.ihx $?

$(BUILD_OUT)/main.rel: $(SRC_PATH)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_OUT)/ser_ir.rel: $(SRC_PATH)/ser_ir.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_OUT)/systimer.rel: $(SRC_PATH)/systimer.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_OUT)/Lib.DHTx1.rel: $(DHTx1_SRC_PATH)/Lib.DHTx1.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: rebuild clean

rebuild: clean all
	@echo 'rebuild finished'

clean:
	cmd /c del /f/q $(BUILD_OUT)
	@echo 'Clean finished'
