# projetPerformSHA1
Projet de calcul haute performance - CUDA - OpenMP

Exemple:
```
gcc -Wall -o attackSingleCoreV2 attackSingleCoreV2.c -lcrypto && time ./attackSingleCoreV2 dict.txt shadow.txt > /dev/null
```

OpenMP:
```
gcc -fopenmp -Wall -o attackSingleCoreV2 attackSingleCoreV2_OpenMP.c -lcrypto && time ./attackSingleCoreV2 dict.txt shadow.txt > /dev/null
```
