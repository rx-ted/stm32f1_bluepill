# UPLOAD_FILE=$(find . -name "LCD.hex" )
UPLOAD_FILE=$(find . -name "main.bin" )
echo Found: $UPLOAD_FILE

sudo stm32flash -w $UPLOAD_FILE -v -g 0x00 /dev/ttyCH3410
