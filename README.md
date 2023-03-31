# AES_Encryption
AES encryption using openssl implementation of AES and a random key generated using a python script


Command:  gcc <file.c> -lcrypto  (or)  gcc <file.c> -lcrypto -o <Name of executable>

Execute : ./a.out (or)  ./<Name of executable>

Requirements:  
                1. libcrypto.so
                    The libcrypto.so shared library should be present in the current working directory.
                    In my case,it Was in :/home/<username>/.PlayOnLinux/wine/linux-x86/1.4.1/lib
          
                2. openssl library
                    sudo apt-get install openssl
                
                3.random module
                    pip install random
          
Dependancies:
          The libcrypto.so and libc.so(contains printf) share libraries are the dependencies.
          The linked libraries can be viewed by using the command
          
          readelf -a a.out | grep Shared
  
  
          OutPut of the command looks like:
  Type:                              DYN (Shared object file)
 0x0000000000000001 (NEEDED)             Shared library: [libcrypto.so.1.1]
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]

                
