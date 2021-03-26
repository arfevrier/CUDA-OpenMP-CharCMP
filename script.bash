#!/bin/bash
export TIMEFORMAT=%R
echo "Single core V1 (String compare)"
for i in {1..4}
do
	gcc -Wall -o attackSingleCore attackSingleCore.c && time ./attackSingleCore dict.txt shadow.txt > /dev/null
done

echo "Single core V2 (Numerical compare)"
for i in {1..4}
do
gcc -Wall -o attackSingleCoreV2 attackSingleCoreV2.c && time ./attackSingleCoreV2 dict.txt shadow.txt > /dev/null
done

echo "CUDA comparaison"
for i in {1..4}
do
nvcc -o attackCUDA attackCUDA.cu && time ./attackCUDA dict.txt shadow.txt > /dev/null
done

echo "OpenMD comparaison"
for i in {1..4}
do
gcc -fopenmp -Wall -o attackOpenMP attackOpenMP.c && time ./attackOpenMP dict.txt shadow.txt > /dev/null
done

echo "[Hors Projet] Single core V3 (Table de hashage avec un arbre binaire)"
for i in {1..4}
do
g++ -Wall -o attackSingleCoreV3 attackSingleCoreV3.cpp && time ./attackSingleCoreV3 dict.txt shadow.txt > /dev/null
done