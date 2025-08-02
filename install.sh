gcc -c mem.c
sudo ar rcs libmem.a *.o; rm *.o; sudo mv libclibp.a /usr/local/lib/libclibp.a
mv mem.h /usr/local/include/
