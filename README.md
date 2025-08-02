<div align="center">
    <h1>memlib</h1>
    <p>memlib that tracks all pointers and free if not manually free'd</p>
</div>

# Installation

```bash
git clone https://github.com/algo1337/memlib.git
cd memlib; sh install.sh
```

# Compilation

Use flag ``-lmem`` to compile

# Use Example

```c
#define CLIBP_MEM
#include <mem.h>

int main() {
  char *test = allocate(10);
  strcpy(test, "TESTING");
  printf("%s\n", test);
  return 0;
}
```
