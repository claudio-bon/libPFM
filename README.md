# libPFM
This library provides an easy and accessible way to read and write [.pfm](http://www.pauldebevec.com/Research/HDR/PFM/) (Portable FloatMap) files in C++ by only relying on the standard library.

## Motivations
.pfm file format plays an important role in the storage of floating point images, which is much needed in fields such as Stereo Vision, where it is not possible to rely on 8 or 16 bit integers to store, for instance, disparity and depth maps. <br />
While finding functions that handle .pfm files in Python is quite straightforward, the same does not hold for C++.
The aim of this library is to provide and accessible way to read and write .pfm files without relying on external dependencies.

## Project Structure
The folder `src` containts all the `.cpp` files. <br />
The folder `include` contains all the `.h` files. <br />
The folder `test` contains a `main` in which the functionalities of the library are tested. <br />
<br />
In order to use the library it is only necessary to include `include/pfm.h` where the `write_pfm` (defined in `src/write_pfm.cpp`) and `read_pfm` (defined in `src/read_pfm.cpp`) functions are declared.<br />
All the functions in `include/pfm.h` are included in the namespace `pfm::io::`.

## Example
```C++
std::vector<std::vector<float>> disparity;
pfm::io::read_pfm("/path/to/file.pfm", disparity);

std::cout << "Disparity height = " << disparity.size() << std::endl;
std::cout << "Disparity width = " << disparity[0].size() << std::endl;
```

## Acknowledgements
The souce code include all the sources it has been taken from or inspired by.
