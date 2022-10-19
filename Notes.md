## Part 1 : Extract and Transform the data

- data_handler.cpp中的bytes变量，定义为unsigned char bytes[4]这个4并不是header有4个32bit int的意思，而是用来存储单个32bit int的。一个32bit int如0x 12 34 56 78，那么bytes[0]存储'12'，这样类推。所以在read_feature_labels函数中bytes的size还是4.
- convert_to_little_endian： 就是换位置，大端换成小端。为什么要这么做呢？因为读文件流，bytes读进来第一个是32bit int的高位，存到bytes的低位，是倒序，变成了大端存储。所以要转换为我们正常数据的小端方式。


### Makefile相关：
1. Makefile里的tab必须用[Tab]键，不能用空格。否则会报missing operator的错误
2. `-shared`、`-fPIC`指令用来生成动态库（.so），这样就不需要每次都编译data_handler这些文件。其中`-shared`表明生成共享库，`-fPIC`表示Position Independent Code，位置无关。
    - 共享对象可能会被不同的进程加载到不同的位置上，如果共享对象中的指令使用了绝对地址，外部模块地址，那么在共享对象被加载时就必须根据相关模块的加载位置对这个地址做调整，也就是修改这些地址，让它在对应进程中能正确访问
3. 如果no such directory之类的报错，可以在终端设置一下位置。比如这里并没有指定MNIST_ML_ROOT变量值，那就可以`export MNIST_ML_ROOT=$PWD`
    - 之后再用.so的时候其实也是export LD_LIBRARY_PATH=lib的路径就可以了。


## Part 2 : KNN

- 测试密钥