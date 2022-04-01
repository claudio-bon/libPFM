# libPFM
This library provides an easy and accessible way to read and write [.pfm](http://www.pauldebevec.com/Research/HDR/PFM/) (Portable FloatMap) files in C++ by only relying on the standard library.

## Motivations
.pfm file format plays an important role in the storage of floating point images, which is much needed in fields such as Stereo Vision, where it is not possible to rely on 8 or 16 bit integers to store, for instance, disparity and depth maps. <br />
While finding functions that handle .pfm files in Python is quite straightforward, the same does not hold for C++.
The aim of this library is to provide an accessible way to read and write .pfm files without relying on external dependencies.

## Project Structure
The folder `src` containts all the `.cpp` files. <br />
The folder `include` contains all the `.h` files. <br />
The folder `test` contains a `main` in which the functionalities of the library are tested. <br />
<br />
In order to use the library it is only necessary to include `include/pfm.h` where the `write_pfm` (defined in `src/write_pfm.cpp`) and `read_pfm` (defined in `src/read_pfm.cpp`) functions are declared.<br />
All the functions in `include/pfm.h` are included in the namespace `pfm::io::`.

## Usage
`std::vector<std::vector<float>>` will be seen as a grayscale image and `std::vector<std::vector<std::vector<float>>>` as a color image.<br />
The same holds for pointers: `float**` for grayscale images while `float***` for color images.<br />
In case of `<std::vector<float>` and `float *` the height and width and a boolean indicating whether the image is grayscale or color will be taken as argument by `pfm::io::write_pfm` and given as output by `pfm::io::read_pfm`.<br />
<br />
The following interfaces are provided.<br />
For the writing function:
```C++
void write_pfm(std::string path, float* mat, int height, int width, bool color, int scale = 1);
void write_pfm(std::string path, float** mat, int height, int width, int scale = 1);
void write_pfm(std::string path, float*** mat, int height, int width, int scale = 1);

void write_pfm(std::string path, std::vector<float>& mat, int height, int width, bool color, int scale = 1);
void write_pfm(std::string path, std::vector<std::vector<float>>& mat, int scale = 1);
void write_pfm(std::string path, std::vector<std::vector<std::vector<float>>>& mat, int scale = 1);
```
For the reading function:
```C++
void read_pfm(std::string path, std::vector<float>& mat, int& height, int& width, bool& color, int& scale);
void read_pfm(std::string path, std::vector<float>& mat, int& height, int& width, bool& color);

void read_pfm(std::string path, std::vector<std::vector<float>>& mat, int& scale);
void read_pfm(std::string path, std::vector<std::vector<float>>& mat);

void read_pfm(std::string path, std::vector<std::vector<std::vector<float>>>& mat, int& scale);
void read_pfm(std::string path, std::vector<std::vector<std::vector<float>>>& mat);
```

## Examples
Reading a .pfm file:
```C++
...

std::vector<std::vector<float>> disparity;
pfm::io::read_pfm("/path/to/file.pfm", disparity);

std::cout << "Disparity height = " << disparity.size() << std::endl;
std::cout << "Disparity width = " << disparity[0].size() << std::endl;
```
Saving a .pfm file:
```C++
...

using fs = std::filesystem

...

std::unique_ptr<float[]> disparity(new float[height * width]);
compute_disparity(&(*disparity), im_left, im_right); //External function

/*The last argument is false because disparity is a grayscale image. It should be set to true for color images.
Mind the fact that height, width and color argument should be given only if the input image is passed in the flat
form as in this case. The same holds for read_pfm function.*/
pfm::io::write_pfm("/path/to/file.pfm", &(*disparity), height, width, false);

if ( fs::exists(fs::path("/path/to/file.pfm")) )
    std::cout << "File created successfully." << std::endl;
```

Another example saving a .pfm file:
```C++
...

using fs = std::filesystem

std::vector<std::vector<std::vector<float>>> color_im;
f(color_im); //External function

pfm::io::write_pfm("/path/to/file.pfm", color_im);

if ( fs::exists(fs::path("/path/to/file.pfm")) )
    std::cout << "Color file created successfully." << std::endl;
```

## Acknowledgements
The souce code include all the sources it has been taken from or inspired by.

## References
For further readings about .pfm file format refer to [this link](http://www.pauldebevec.com/Research/HDR/PFM/).
