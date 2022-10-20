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

- 虽然作者在文中不会在乎内存，即new了也不delete或者啥的。但千万别手贱去delete，因为总会在不合适的地方delete掉。（以后有时间再全改成智能指针

- 遇到了好多问题
1. fPIC的使用，是在生成每个.o的时候要用-fPIC，然后在合成.o生成.so的时候不用-fPIC了，但是要加-shared。
2. 要用.so的时候，-L是路径，-l是名字（l是小写L，名字是指去掉lib，比如libxxx.so，则要-lxxx）。然后，链接.so编译的时候不用加-shared也不用加-fPIC，搞清楚用途。
3. C/C++编译时没问题，但运行却说该.so没有，是因为-L的路径只在编译时用。在运行时，要自己添加全局变量。如export LD_LIBRARY_PATH=/root/mnist-ml/lib。但是！又报了诸如“libstdc++.so.6: version `GLIBCXX_3.4.20' not found”这样的错误，解决方案参考这篇神文：https://www.cnblogs.com/emanlee/p/14873176.html。
为防止页面失效，我摘抄一下
-----------------
这个错误是目前的libstdc++.so.6没有对应的GLBCXX造成的。可以通过如下命令查看：
```
[ss@~]$ strings /usr/lib64/libstdc++.so.6 | grep GLIBCXX
GLIBCXX_3.4
GLIBCXX_3.4.1
...
GLIBCXX_3.4.13
```

可以看到，最高版本为3.4.13，没有对应的3.4.20。
通过查看libstdc++.so.6可以看到它链接到了另外一个库
```
[ss@~]$ ll /usr/lib64/libstdc++.so.6
  /usr/lib64/libstdc++.so.6 -> libstdc++.so.6.0.13
```
接下来看看系统还有没有更高版本的lib库
```
[ss@server1 service]# find / -name libstdc++.so.6*
/usr/lib64/libstdc++.so.6.bak
/usr/lib64/libstdc++.so.6.0.13
/usr/lib64/libstdc++.so.6
/usr/lib64/libstdc++.so.6.0.20
/usr/local/lib64/libstdc++.so.6
/usr/local/lib64/libstdc++.so.6.0.20
```
刚好还有一个6.0.20版本，查看它的信息
```
[root@spider-server1 service]# strings /usr/local/lib64/libstdc++.so.6.0.20 | grep GLIBCXX
GLIBCXX_3.4
GLIBCXX_3.4.1
GLIBCXX_3.4.2
...
GLIBCXX_3.4.19
GLIBCXX_3.4.20
```
看来这个版本满足我们的需求，重新做链接
```
$ cp /usr/local/lib64/libstdc++.so.6.0.20 /usr/lib64/libstdc++.so.6.0.20


$ rm -f /usr/lib64/libstdc++.so.6


$ ln -s /usr/lib64/libstdc++.so.6.0.20 /usr/lib64/libstdc++.so.6 
```
---------------------

这样跑完之后基本上就没问题了，就是跑得有点慢，在辣鸡单核服务器上没办法