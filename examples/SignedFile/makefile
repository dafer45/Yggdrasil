INCLUDE_FLAGS = -I../../include/Streams/
INCLUDE_FLAGS += -I../../json/
INCLUDE_FLAGS += -I../../include/Cryptography/

all:
	g++ src/main.cpp -std=c++11 -o build/a.out $(INCLUDE_FLAGS) -L../../build/ -lYggdrasil -lcurl -lssl -lcrypto
