# Installation Guide

## Installation with dependencies

1. Install dependencies that are needed for libcloudphxx and libmpdata. The list of dependencies can be found [here]().

2. Build libcloudphxx and libmpdataxx

```bash
git clone git@github.com:your-repo/libcloudphxx.git
cd libcloudphxx
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ -DCMAKE_INSTALL_PREFIX=~/builds
make install
```

```bash
git clone git@github.com:your-repo/libmpdataxx.git
cd libmpdataxx/libmpdata++
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=gcc -DCMAKE_INSTALL_PREFIX=~/builds
make install
```
This places the build files for libcloudphxx and libmpdata in the `~/builds` directory (or any other directory of your choice).

3. Build UWLCM

```bash
git clone git@github.com:your-repo/UWLCM.git
cd UWLCM
mkdir build && cd build
cd Github/UWLCM/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ -DCMAKE_INSTALL_PREFIX=~/builds -Dlibmpdata++_DIR=~/builds/share/libmpdata++ -Dlibcloudph++_DIR=~/builds/share/libcloudph++
make install
```

CMake options for UWLCM:
- Running the compilation in parallel for speedup: `make -jN install`, where N is the number of cores.
- Disabling some configurations for speedup: -DUWLCM_DISABLE="PIGGYBACKER;3D_LGRNGN". Possible configurarions are:
    - 3D_LGRNGN - 3D with Lagrangian microphysics
    - 2D_LGRNGN - 2D with Lagrangian microphysics
    - 3D_BLK_2M - 3D with 2-moment bulk microphysics
    - 2D_BLK_2M - 2D with 2-moment bulk microphysics
    - 3D_BLK_1M - 3D with 1-moment bulk microphysics
    - 2D_BLK_1M - 2D with 1-moment bulk microphysics
    - 3D_BLK_1M_ICE - 3D with 1-moment bulk microphysics with ice
    - 2D_BLK_1M_ICE 2D with 1-moment bulk microphysics with ice
    - 2D_NONE - 2-dimensional with no microphysics
    - 3D_NONE - 3-dimensional with no microphysics
    - PIGGYBACKER - Piggybacker option [(Grabowski, 2019)](https://adgeo.copernicus.org/articles/49/105/2019/)
---

## Installation with Apprainer/Singularity
Apptainer is a container platform designed for scientific computing. It allows packaging the software environment — including dependencies into a single, portable Singularity Image File (.sif). To install UWLCM with Apptainer, follow the steps below:

1. You need to have Singularity or Apptainer. It is most probably already installed on your cluster.

2. Download the UWLCM image from:
   
For MPI (parallel computation on multiple nodes), download the Singularity image with MPI (MVAPICH2).

3. Clone libmpdata++, libcloudph++ and UWLCM repositories:
```bash
git clone git@github.com:your-repo/libcloudphxx.git
git clone git@github.com:your-repo/libmpdata.git
git clone git@github.com:your-repo/UWLCM.git
```

4. Install libmpdata++, libcloudph++ and UWLCM using the Singularity image:

```bash
  cd libmpdataxx/libmpdata++/build
   singularity exec --nv [IMAGE_NAME].sif sh -c
  "cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=gcc
  -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} .. ; make install"

cd libcloudphxx/build
  $ singularity exec --nv [IMAGE_NAME].sif sh -c
  "cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++
  -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
  -DLIBCLOUDPHXX_FORCE_MULTI_CUDA=True .. ; make -j4 install"

    cd UWLCM/build
   singularity exec --nv [IMAGE_NAME] sh -c
  "cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=gcc
  -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
  -Dlibmpdata++_DIR=${INSTALL_DIR}/share/libmpdata++
  -Dlibcloudph++_DIR=${INSTALL_DIR}/share/libcloudph++ .. ; make -j4 install" 
```

When building for MPI runs, tell CMake to use the MPI compiler instead of gcc, e.g.:
-DCMAKE_CXX_COMPILER=mpic++.