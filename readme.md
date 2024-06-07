### Requirements

* CMake: 3.24
* gcc: 13.2
* C++ standard: 23
* ninja (or make)

### Compilation, build and run

```bash
mkdir build
cmake -DUCESB_DIR=<ucesb_project_dir> -DBOOST_DIR=<boost_project_dir> ..
ninja -j<n> # make if using gnummake
./test
```
