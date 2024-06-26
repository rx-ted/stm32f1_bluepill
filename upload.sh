#!/bin/bash

FIND_PATH=./output
# UPLOAD_FILE=$(find . -name "LCD.hex" )
UPLOAD_FILE=$(find $FIND_PATH -name "*.bin" )
if [[ -z "$UPLOAD_FILE" ]]; then
  echo Not found $FIND_PATH for binary file
  exit 1
fi
echo Found: $UPLOAD_FILE

sudo stm32flash -w $UPLOAD_FILE -v -g 0x00 /dev/ttyCH3410
