FQBN = adafruit:samd:adafruit_feather_m0
BUILD_PATH = ./build
BIN_PATH = ./bin

default: rover.bin

rover.bin: rover.ino
	if ! [ -d $(BIN_PATH) ]; then mkdir $(BIN_PATH); chmod 700 bin; fi
	arduino-cli compile --fqbn $(FQBN) -o $(BIN_PATH)/rover.bin rover.ino

upload:
	arduino-cli upload --fqbn $(FQBN) -p ${ARD_PORT} -i $(BIN_PATH)/rover.bin

