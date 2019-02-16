#include "binheader.h"
#include "externals.h"

int main(int argc, char **argv)
{
    MyHeaderWriter hw;  // dummy

    MyImageStruct  image;
    TStream rd("testovl.ovl","rb");
    rd >> image;

    // test
    std::cout << "EXT: " << std::hex << externals << "\n";
    std::cout << image.h_image_name     << std::endl
              << image.h_image_symbols  << std::endl
              << image.h_image_entry    << std::endl;

    rd.exec ();
    rd.close();

    return 0;
}
