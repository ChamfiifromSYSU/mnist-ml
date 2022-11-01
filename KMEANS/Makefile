CC=g++
SRC := $(PWD)/src
CFLAGS := -std=c++11 -DEUCLID
INCLUDE_DIR := $(PWD)/include/

all : main

main : $(SRC)/kmeans.cpp
	$(CC) $(CFLAGS) $(SRC)/kmeans.cpp -o main -I$(INCLUDE_DIR) \
	-I$(MNIST_ML_ROOT)/include/ -L$(MNIST_ML_ROOT)/lib/ -ldata

clean :
	rm main