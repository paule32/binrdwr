#include "binheader.h"

using namespace std;

MyHeaderWriter::MyHeaderWriter()
{
    MyImageStruct image;
    TStream wr("runtime.ovl","wb");

    image.h_image_type  = 1;  // library
    image.h_image_entry = 0;  // if exec, entry point (main)

    image.h_image_name_len = std::string("runtime.ovl").size();
    image.h_image_name     = std::string("runtime.ovl");

    image.info_lib.clear();
    image.info_lib.emplace_back( h_info_lib { 7, "FooLib1" } );
    image.info_lib.emplace_back( h_info_lib { 7, "BarLib2" } );


    MyCodeEmitter code_emitter;
    image.h_image_data.clear();

    image.h_image_data.insert(
    image.h_image_data.end(),

    code_emitter.code_data,
    code_emitter.code_data+
    code_emitter.code_len);


    image.info_symbol.clear();
    image.info_symbol.emplace_back( h_info_symbol { 5, "write", 0, 0, 101 } );
    image.info_symbol.emplace_back( h_info_symbol { 4, "read" , 0, 0, 202 } );
    //
    image.h_image_symbols = 2;

    wr << image;
    wr.close();

    wr.open("testovl.ovl","rb")
    wr >> image;
    wr.close();
}

MyHeaderWriter::~MyHeaderWriter() { }
