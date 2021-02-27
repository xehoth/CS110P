# Lab1

## Exercise 3

Decimal: 6, 9, 15, 9, 9  
Hexadecimal: 0x69F99

0001  
0001  
1001  
1001  
0110  
J

1111  
1000  
1111  
1000  
1000  
F

1110  
1001  
1110  
1001  
1110  
0xE9E9E

1001  
1101  
1011  
1001  
0x9DB9

## Exercise 4

1, 2, 4, 8, 16, 32, 64, 128, 256, 489

## Exercise 5

`main.c`

### 64

``` plain
Size of char: 1
Size of short: 2
Size of short int: 2
Size` of int: 4
Size of long int: 8
Size of unsigned int: 4
Size of void *: 8
Size of size_t: 8
Size of float: 4
Size of double: 8
Size of int8_t: 1
Size of int16_t: 2
Size of int32_t: 4
Size of int64_t: 8
Size of time_t: 8
Size of clock_t: 8
Size of struct tm: 56
Size of NULL: 8
```

### 32

``` plain
Size of char: 1
Size of short: 2
Size of short int: 2
Size of int: 4
Size of long int: 4
Size of unsigned int: 4
Size of void *: 4
Size of size_t: 4
Size of float: 4
Size of double: 8
Size of int8_t: 1
Size of int16_t: 2
Size of int32_t: 4
Size of int64_t: 8
Size of time_t: 4
Size of clock_t: 4
Size of struct tm: 44
Size of NULL: 4
```

`long int`, pointers are different depending on compiling bits.  
`time_t`: a typedef of `long`  
`clock_t`: a typedef of `long`  
`NULL`: a pointer `((void *)0)`
`struct tm`
``` c
struct tm {
  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year	- 1900.  */
  int tm_wday;			/* Day of week.	[0-6] */
  int tm_yday;			/* Days in year.[0-365]	*/
  int tm_isdst;			/* DST.		[-1/0/1]*/

  long int __tm_gmtoff;		/* Seconds east of UTC.  */
  const char *__tm_zone;	/* Timezone abbreviation.  */
};
```
32bit: 4 * 9 + 4 + 4 = 44  
64bit: 4 * (9 + 1) + 8 + 8 = 56 (the first 9 int cost 4 * 9 bytes but need another 1 byte for aligning)
