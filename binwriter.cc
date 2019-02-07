#include "binheader.h"

using namespace std;

void testfunc(void)
{
    printf("Eine Runtimesfiler\n");
}

void testung(uint32_t length, void * buffer)
{
    
/*

    image_struct.h_lib_info.h_lib_name_len = 0;
    image_struct.h_lib_info.h_lib_name = 0x00;

    image_struct.h_symbol_info.h_symbol_len = std::string("print").size();
    image_struct.h_symbol_info.h_symbol_name = new char[
    image_struct.h_symbol_info.h_symbol_len] ; strcpy(
    image_struct.h_symbol_info.h_symbol_name , "print");

    image_struct.h_symbol_info.h_symbol_type = 0;
    image_struct.h_symbol_info.h_symbol_subtype = 0;
    image_struct.h_symbol_info.h_symbol_lib = 0;
    #define print_offset 0x241;
    image_struct.h_symbol_info.h_symbol_offset = print_offset;

    image_struct.h_image_info.h_image_len = length;
    image_struct.h_image_info.h_image_data = buffer;
*/
}

MyHeaderWriter::~MyHeaderWriter() { }

MyHeaderWriter::MyHeaderWriter(
    std::string n,
    uint64_t length,
    void   * buffer)
{
    wr.open("runtime.ovl", ios_base::out | ios_base::binary);
    if (!wr.is_open())
    throw std::string("error: could no open file for writing.\n");

    MyImageStruct image;

    image.h_magic   = ASM32_MAGIC;
    image.h_version = ASM32_VERSION;

    image.h_image_name_len = std::string("runtime.ovl").size();
    image.h_image_name     = std::string("runtime.ovl");

    image.info_lib.emplace_back( h_info_lib{ "Foo" } );
    image.info_lib.emplace_back( h_info_lib{ "BAR" } );

    wr << image;

    wr.close();
}

