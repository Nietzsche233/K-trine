# K-trine
Coods and Datasets for "The k-Trine Cohesive Subgraph and Its Efficient Algorithms"

## Requirements

* GCC == 11.4.0
* CMAKE
* python == 3.10.0

  
## Code Structure

- `Project/K-trine/k-trine-compute/`: Codes for Algorithm 1.
- `Project/K-trine/insertian_and_removal/`: Codes for Algorithm 3 and Algorithm 4.
- `Project/K-trine/insertian+_and_removal/`: Codes for Algorithm 5 and Algorithm 4.
- `Project/K-trine/effectiveness/`: Extra codes for Exp-1 and Exp-2 in Sec. 6.1.

## Build and Run

Build and run Algorithm 1 with the code below in the project folder.

``` shell
cd Project/K-trine/k-trine-compute/
mkdir build
cd build
cmake ..
make -j
./main facebook-wosn-links.txt 1770
```

Build and run Algorithm 3 and 4 with the code below in the project folder.

``` shell
cd Project/K-trine/insertion_and_removal/
compile.sh
./K_Trine.out
../../../Data/facebook-wosn-links.txt
-2
1 5000 500
```

Codes in `/insertion+_and_removal/` is similar to codes in `/insertion_and_removal/`.



