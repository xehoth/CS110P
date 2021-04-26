# Lab 8

## Ex 1

### Scenario 1

(1). 0%  
因为步长为 8，类型大小为 4byte，一次 += 32 byte，cache 大小 32 byte，direct mapping 都映射到 `00` set，故每次都 miss  
(2). 0%  
同理，因为一次 += 32 byte  
(3). step size = 32, rep count 2147483647

### Scenario 2

(1). 75%  
N = 4, set num = 4  
cache size = array size  
r+w cnt = 2 * (array size / 4) / step size * rep cnt = 2 * 32 = 64  
miss = 第一次访问 (compulsory miss) = 16  
x(xx)(xxxx) 后四位 block offset, 中间两位 set, 每次 step += 2 * 4 = 8 int = 32 byte
hit rate = (64 - 16) / 64 = 75%  
(2). $1 - 16 / \infty \rightarrow 1$  
(3). each time fully process a block of data of size Cache Size. Cache blocking.

### Scenario 3

(1).  
Array Size = Cache Size  
step += 8 * 4 = 32  
w times = Array Size / 4 / Step Size = 8
```
access          set
00 00 0000      0
00 10 0000      2
01 00 0000      0
01 10 0000      2
10 00 0000      0
10 10 0000      2
11 00 0000      0
11 10 0000      2
```
min: each time is a miss, hit rate = 0/16 = 0  
max: only compulsory = 8, lines in a set = 4, hit rate = 8/16 = 0.5

(2).  
Associativity = 1,  direct mapping, set 都不同  
0.5  
each line only has compulsory miss.  
1/2=0.5  

## Ex 2

a. jki
b. jkj, kij
c. 最内层循环访问至少两行（因为行是第二维，按行维护好的空间局部性）的更优
