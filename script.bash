#!/bin/bash
echo "Single core V1 (String compare)"
gcc -Wall -o attackSingleCore attackSingleCore.c && time ./attackSingleCore dict.txt shadow.txt > /dev/null
echo "Single core V2 (Numerical compare)"
gcc -Wall -o attackSingleCoreV2 attackSingleCoreV2.c && time ./attackSingleCoreV2 dict.txt shadow.txt > /dev/null
echo "CUDA comparaison"
nvcc -o attackCUDA attackCUDA.cu && time ./attackCUDA dict.txt shadow.txt > /dev/null
echo "OpenMD comparaison"
gcc -fopenmp -Wall -o attackOpenMP attackOpenMP.c && time ./attackOpenMP dict.txt shadow.txt > /dev/null
echo "[Hors Projet] Single core V3 (Table de hashage avec un arbre binaire)"
g++ -Wall -o attackSingleCoreV3 attackSingleCoreV3.cpp && time ./attackSingleCoreV3 dict.txt shadow.txt > /dev/null