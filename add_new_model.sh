#! /bin/bash

if [[ -z $MNIST_ML_ROOT ]]; then
    echo "Please define MNIST_ML_ROOT"
    exit 1
fi

dir=$(echo "$@" | tr a-z A-Z) # makes input all uppercase
model_name_lower=$(echo "$@" | tr A-Z a-z) # makes model_name lower case, $@第一个参数

mkdir -p $MNIST_ML_ROOT/$dir/include $MNIST_ML_ROOT/$dir/src
touch $MNIST_ML_ROOT/$dir/Makefile
touch $MNIST_ML_ROOT/$dir/include/"$model_name_lower.hpp"
touch $MNIST_ML_ROOT/$dir/src/"$model_name_lower.cpp"

# before execute, new to "chmod +x add_new_model.sh"