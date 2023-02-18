# Homepage of RemCCG

**RemCCG** is a remanufactured C program generator achieved by the framework *RemGen* proposed in the paper "RemGen: Remanufacturing A Program Generator for Compiler Testing" [[PDF]](https://haoxintu.github.io/files/issre2022-camera-ready.pdf) (Accepted in the Research Track of [ISSRE 2022](https://issre2022.github.io/index.html)).

----
## Installation
```
./install.sh
```

or

```
sudo apt-get install autoconf automake libtool curl make g++ unzip
wget https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.11.0.zip
unzip v3.11.0.zip
cd protobuf-3.11.0
./autogen.sh
./configure
make
sudo make install
cd ..

sudo apt-get install clang-6.0 clang++-6.0
mkdir build
cd build
cmake ..
make -j4
cd ..
```
you can find the `remccg` in build/src and use it to perform compiler testing.

## The following is a full list of all the reported bugs found by *RemCCG* (remanufactured by *RemGen* framework) (Update by 2022-5-20).


### GCC bugs

1. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=99694 (Performance, Fixed)
2. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=99880 (Crash, Fixed)
3. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=99947 (Crash, Fixed)
4. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100349 (Crash, Fixed)
5. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100512 (Crash, Fixed)
6. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100578 (Crash, Duplicate)
7. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100626 (Crash, Fixed)
8. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=102057 (Crash, Fixed)
9. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=102356 (Performance, Fixed)
10. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=102561 (Crash, Duplicate)

### LLVM bugs

1. https://bugs.llvm.org/show_bug.cgi?id=49171 (Performance, Fixed)
2. https://bugs.llvm.org/show_bug.cgi?id=49205 (Performance, Fixed)
3. https://bugs.llvm.org/show_bug.cgi?id=49206 (Performance, Duplicate)
4. https://bugs.llvm.org/show_bug.cgi?id=49218 (Crash, Fixed)
5. https://bugs.llvm.org/show_bug.cgi?id=49396 [[Github issue](https://github.com/llvm/llvm-project/issues/48740)](Crash, Fixed)
6. https://bugs.llvm.org/show_bug.cgi?id=49451 (Crash, Fxied)
7. https://bugs.llvm.org/show_bug.cgi?id=49466 (Crash, Fixed)
8. https://bugs.llvm.org/show_bug.cgi?id=49475 (Performance, Fixed)
9. https://bugs.llvm.org/show_bug.cgi?id=49537 (Crash, Duplicate)
10. https://bugs.llvm.org/show_bug.cgi?id=49541 (Performance, Fixed)
11. https://bugs.llvm.org/show_bug.cgi?id=49556 (Performance, Duplicate)
12. https://bugs.llvm.org/show_bug.cgi?id=49670 (Crash)
13. https://bugs.llvm.org/show_bug.cgi?id=49697 [[Github issue](https://github.com/llvm/llvm-project/issues/49041)](Crash, Fixed)
14. https://bugs.llvm.org/show_bug.cgi?id=49783 (Crash)
15. https://bugs.llvm.org/show_bug.cgi?id=49784 (Crash, Worksforme)
16. https://bugs.llvm.org/show_bug.cgi?id=49785 (Performance, Fixed)
17. https://bugs.llvm.org/show_bug.cgi?id=49786 (Crash)
18. https://bugs.llvm.org/show_bug.cgi?id=49788 (Performance, Fixed)
19. https://bugs.llvm.org/show_bug.cgi?id=49993 (Crash, Fixed)
20. https://bugs.llvm.org/show_bug.cgi?id=50009 (Crash, Fixed)
21. https://bugs.llvm.org/show_bug.cgi?id=50050 (Crash, Fixed)
22. https://bugs.llvm.org/show_bug.cgi?id=50191 (Crash, Fixed)
23. https://bugs.llvm.org/show_bug.cgi?id=50238 [[Github issue](https://github.com/llvm/llvm-project/issues/49582)](Crash, Fixed)
24. https://bugs.llvm.org/show_bug.cgi?id=50254 (Performance, Fixed)
25. https://bugs.llvm.org/show_bug.cgi?id=50279 (Performance, Fixed)
26. https://bugs.llvm.org/show_bug.cgi?id=50302 (Performance, Fixed)
27. https://bugs.llvm.org/show_bug.cgi?id=50307 (Crash, Fixed)
28. https://bugs.llvm.org/show_bug.cgi?id=50308 (Performance, Fixed)
29. https://bugs.llvm.org/show_bug.cgi?id=50325 (Crash)
30. https://bugs.llvm.org/show_bug.cgi?id=50331 (Performance)
31. https://bugs.llvm.org/show_bug.cgi?id=50352 (Performance)
32. https://bugs.llvm.org/show_bug.cgi?id=50368 (Crash, Worksforme)
33. https://bugs.llvm.org/show_bug.cgi?id=50464 (Performance)
34. https://bugs.llvm.org/show_bug.cgi?id=50480 (Crash)
35. https://bugs.llvm.org/show_bug.cgi?id=51553 (Crash, Fixed)
36. https://bugs.llvm.org/show_bug.cgi?id=51584 (Performance, Fixed)
37. https://bugs.llvm.org/show_bug.cgi?id=51612 (Crash, Fixed)
38. https://bugs.llvm.org/show_bug.cgi?id=51656 (Crash, Fixed)
39. https://bugs.llvm.org/show_bug.cgi?id=51657 (Performance, Fixed)
40. https://bugs.llvm.org/show_bug.cgi?id=51762 (Performance, Fixed)
41. https://bugs.llvm.org/show_bug.cgi?id=51830 (Crash)
42. https://bugs.llvm.org/show_bug.cgi?id=51841 (Performance)
43. https://bugs.llvm.org/show_bug.cgi?id=51922 (Performance)
44. https://bugs.llvm.org/show_bug.cgi?id=51953 (Performance)
45. https://bugs.llvm.org/show_bug.cgi?id=52018 (Crash, Fixed)
46. https://bugs.llvm.org/show_bug.cgi?id=52024 (Crash, Fixed)


## Acknowledgment
We appreciate the developers from GCC and LLVM very much for their quick responses for confirming and fixing bugs.


