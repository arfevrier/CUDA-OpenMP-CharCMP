# CUDA-OpenMP-CharCMP
Comparaison de caractère - Benchmark OpenMP/CUDA - Projet de Calcul Haute Performance

Exemple (attackSingleCore.c and attackSingleCoreV2.c):
```
gcc -Wall -o attackSingleCoreV2 attackSingleCoreV2.c && time ./attackSingleCoreV2 dict.txt shadow.txt
```

OpenMP:
```
gcc -fopenmp -Wall -o attackOpenMP attackOpenMP.c && time ./attackOpenMP dict.txt shadow.txt
```

CUDA:
```
nvcc -o attackCUDA attackCUDA.cu && time ./attackCUDA dict.txt shadow.txt
```

C++ (attackSingleCoreV3.cpp):
```
g++ -Wall -o attackSingleCoreV3 attackSingleCoreV3.cpp && time ./attackSingleCoreV3 dict.txt shadow.txt
```
