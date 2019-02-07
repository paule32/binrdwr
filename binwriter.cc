#include "binheader.h"

using namespace std;

MyHeaderWriter::~MyHeaderWriter() { }

MyHeaderWriter::MyHeaderWriter(
/*
    std::string n,
    uint64_t length,
    void   * buffer*/ )
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

