#include "binheader.h"

using namespace std;

std::map<std::string, uint8_t*> image_data;

MyHeaderWriter::MyHeaderWriter()
{
    wr.open("runtime.ovl", ios_base::out | ios_base::binary);
    if (!wr.is_open())
    throw std::string("error: could no open file for writing.\n");

    MyImageStruct image;

    image.h_magic   = ASM32_MAGIC;
    image.h_version = ASM32_VERSION;

    image.h_image_type  = 1;  // library
    image.h_image_entry = 0;  // if exec, entry point (main)

    image.h_image_name_len = std::string("runtime.ovl").size();
    image.h_image_name     = std::string("runtime.ovl");

    image.info_lib.emplace_back( h_info_lib { 7, "FooLib1" } );
    image.info_lib.emplace_back( h_info_lib { 7, "BarLib2" } );


    MyCodeEmitter code_emitter;
    image_data.emplace(std::string("write"), code_emitter.code_data);
    image_data.emplace(std::string("read" ), code_emitter.code_data);


    image.info_symbol.emplace_back( h_info_symbol { 5, "write", 0, 0, 101 } );
    image.info_symbol.emplace_back( h_info_symbol { 4, "read" , 0, 0, 202 } );

    wr << image;
    wr << image_data;

    wr.close();
}

MyHeaderWriter::~MyHeaderWriter() { }
