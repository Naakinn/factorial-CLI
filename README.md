# factorial CLI
## Note:
The biggest standard unsigned integer type in C is `unsigned long long`(uint64_t) whose size is implementation-defined and othen is 64 bits. Thus this program can't compute factorial of a number bigger than 20(

When unsigned variable is assigned to a value bigger than it's type can hold, the value resets to 0, for instance
```
#include <stdio.h> /* for printf */
#include <stdint.h> /* for integer types and macro constants */

int main() {
    unsigned long long a = UINT64_MAX; /* max value */
    uint64_t b = UINT64_MAX; /* max value */

    printf("%lu\n", a);
    printf("%llu\n", b);

    ++a; 
    ++b;

    printf("%lu\n", a);
    printf("%llu\n", b);

    ++a;
    ++b;

    printf("%lu\n", a);
    printf("%llu\n", b);
}
```
produces
```
18446744073709551615
18446744073709551615
0
0
1
1
