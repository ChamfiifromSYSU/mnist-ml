CC=g++
SRC := $(PWD)/src
CFLAGS := -std=c++11 -DEUCLID
INCLUDE_DIR := $(PWD)/include/

all : main

main : $(SRC)/knn.cpp
	$(CC) $(CFLAGS) $(SRC)/knn.cpp -o main -I$(INCLUDE_DIR) \
	-I$(MNIST_ML_ROOT)/include/ -L$(MNIST_ML_ROOT)/lib/ -ldata

clean :
	rm main