VM space: $2^8$ words  
16 blocks in main memory  
8 words/block  
page size 32 words, each page 4 block  
4 page in physical memory, 8 in VM  


ten addr:

34, 61, 79, F4, 32, 89, 8E, 6B, B6, BE

## EX1

(1) page size = 32 words = 128 Bytes

(2) TLB Hits: 5, TLB Miss: 5, Page Hits: 0, Page Faults: 5

(3) TLB entries = #pages in physical memory, so if TLB misses, the data is definitely not in physical memory, and therefore not in Page Table.

So it's impossible to think of a set of ten addresses that would result in Page Hit.

(4) 0x34 (0b001|10100)

VPN: 0b001 = 1, offset: 0b10100

check 1 in TLB, miss

then find row 1 in page table, not valid

page0 frame is free, map page0



## EX2

ADDR: 00, 20, 40, 60, 80, 00, 20, 40, 60, 80

VPN:   0,  1,  2,  3,  4,  0,  1,  2,  3,  4

## EX3

increase the physical memory size, then we can maintain more frames in the page table.