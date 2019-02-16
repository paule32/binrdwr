#ifndef BINWRITER_H
#define BINWRITER_H
#pragma once

#include "binheader.h"
#include "emitter.h"

constexpr int ASM32_MAGIC   = 0x19790208;
constexpr int ASM32_VERSION = 0x00000132;

extern "C" void vmExec(uint8_t *img, uint32_t len);

struct h_info_lib {
    uint16_t     h_lib_name_len;
    std::string  h_lib_name;		// name of lib
};

struct h_info_symbol {
    uint16_t     h_symbol_name_len;
    std::string  h_symbol_name;
    uint8_t 	 h_symbol_type;     // 0: exec func, 1: lib func/sym,
    uint8_t 	 h_symbol_subtype;  // int, 1: float, 2: string, ...
    uint16_t	 h_symbol_lib;
    uint32_t	 h_symbol_offset;   // 0 - function in image (offset)
};

struct MyImageStruct {
    uint32_t	 h_magic   = ASM32_MAGIC;
    uint16_t	 h_version = ASM32_VERSION;

    uint8_t		 h_image_type;		// lib/exec ?
    uint16_t     h_image_symbols;   // no of symbols
    uint32_t     h_image_entry;

    uint16_t     h_image_name_len;  // str. length
    std::string  h_image_name;		// image name string

    uint32_t     h_info_lib_no;
    std::vector< h_info_lib>            info_lib;

    uint32_t     h_info_symbol_no;
    std::vector< h_info_symbol>         info_symbol;

    uint32_t	         h_image_len;   // length of code data
    std::vector<uint8_t> h_image_data;  // code data
};

class TStream
{
public:
    std::string m_path = "";
    char *buffer;

    bool  is_open = false;
    FILE *outfile = nullptr;
    uint32_t file_entry = 0;

    MyCodeEmitter code_emitter;
  
    TStream()                       : m_path("") { }
    TStream(const std::string& path,
            const std::string& mode): m_path(path) {
        outfile = fopen(path.c_str(),mode.c_str());
        is_open = true;
    }
    void open(const std::string &path,
              const std::string &mode)
    {   m_path  = path;
        outfile = fopen(path.c_str(),mode.c_str());
        is_open = true;
    }
    void close() {
        if (is_open) {
            fclose(outfile);
            is_open = false;
        }
    }
    int exec() {
        vmExec(
            code_emitter.code_data,
            code_emitter.code_len);
        return 0;
    }
   ~TStream() {
        close();
    }
};


inline TStream &operator << (TStream &os, const uint8_t &v) { fwrite(&v,sizeof(v),1,os.outfile); return os; }
inline TStream &operator >> (TStream &os,       uint8_t &v) {
    uint8_t data;
    fread (&data,sizeof(v),1,os.outfile);
        v = data;
    return os;
}

inline TStream &operator << (TStream &os, const uint16_t &v) { fwrite(&v,sizeof(v),1,os.outfile); return os; }
inline TStream &operator >> (TStream &os,       uint16_t &v) {
    uint16_t data;
    fread (& data,sizeof(data),1,os.outfile);
         v = data;
    return os;
}

inline TStream &operator << (TStream &os, const uint32_t &v) { fwrite(&v,sizeof(v),1,os.outfile); return os; }
inline TStream &operator >> (TStream &os,       uint32_t &v) {
    uint32_t data;
    fread (& data,sizeof(data),1,os.outfile);
         v = data;
    return os;
}

inline TStream &operator << (TStream &os, const uint64_t &v) { fwrite(&v,sizeof(v),1,os.outfile); return os; }
inline TStream &operator >> (TStream &os,       uint64_t &v) {
    uint64_t data;
    fread (& data,sizeof(data),1,os.outfile);
    return os;
}

inline TStream & operator << (TStream &os, const std::string& v) {
    uint16_t vlen = v.size();
    os <<    vlen ;

    fwrite(&v[0],sizeof(char),vlen,os.outfile);
    return os;
}
inline TStream & operator >> (TStream &os, std::string& v) {
    std::string str_data;
    uint16_t    len_data;

    fread(&len_data   ,sizeof(uint16_t),   1,os.outfile);
    fread(&str_data[0],sizeof(char),len_data,os.outfile);

    v = str_data;

    return os;
}

inline TStream & operator << (TStream &os, const std::vector<h_info_lib> &v) {
    uint32_t    vlen = v.size();
    uint16_t    tlen;
    std::string data;

    os  << vlen;

    for (int i = 0; i < vlen; i++)
    os << static_cast<h_info_lib>(v[i]).h_lib_name;

    return os;
}
inline TStream & operator >> (TStream &os, std::vector<h_info_lib> v) {
    std::string str_data;
    uint16_t    len_data;
    uint32_t    vec_size;

    struct h_info_lib i_lib;

    fread(&vec_size,sizeof(vec_size),1,os.outfile);

    for (int i = 0; i < vec_size; i++) {
        fread(&len_data   ,sizeof(uint16_t),1,os.outfile);
        fread(&str_data[0],sizeof(char),len_data,os.outfile);

        i_lib.h_lib_name_len = len_data;
        i_lib.h_lib_name     = str_data;

        v.push_back(i_lib);
    }
    return os;
}

inline TStream & operator << (TStream &os, const std::vector<h_info_symbol> &v) {
    uint32_t vec_len = v.size();
    os <<    vec_len ;

    for (const h_info_symbol n: v) {
        os << n.h_symbol_name;
        os << n.h_symbol_type;
        os << n.h_symbol_subtype;
        os << n.h_symbol_lib;
        os << n.h_symbol_offset;
    }
    return os;
}
inline TStream & operator >> (TStream &os, std::vector<h_info_symbol> &v) {
    struct h_info_symbol i_sym;
    uint32_t data_len;

    fread(&data_len,sizeof(data_len),1,os.outfile);

    for (int i = 0; i < data_len; i++) {
        os >> i_sym.h_symbol_name;
        os >> i_sym.h_symbol_type;
        os >> i_sym.h_symbol_subtype;
        os >> i_sym.h_symbol_lib;
        os >> i_sym.h_symbol_offset;

        v.push_back(i_sym);
    }
    return os;
}

inline TStream & operator << (TStream &os, const std::vector<uint8_t>& v) {
    uint32_t vec_len = v.size();
    os <<    vec_len ;

    for (auto n: v)
        os << n;
	return os;
}
inline TStream & operator >> (TStream &os, std::vector<uint8_t> &v) {
    uint32_t data_len;
    uint8_t  data;
    fread( & data_len,sizeof(uint32_t),1,os.outfile);

    for (int i = 0; i < data_len; i++) {
        fread(&data,sizeof(data),1,os.outfile);
        v.push_back(data);
    }

    return os;
}

inline TStream &operator << (TStream &os, const h_info_lib &v) {
    os << v.h_lib_name;

    return os;
}
inline TStream &operator >> (TStream &os, h_info_lib &v) {
    fread(&v.h_lib_name_len,sizeof(uint16_t),1,os.outfile);
    fread(&v.h_lib_name[0] ,sizeof(char),v.h_lib_name_len,os.outfile);

    return os;
}

inline TStream &operator << (TStream &os, const h_info_symbol &v) { 
    os << v.h_symbol_name;
    os << v.h_symbol_type;
    os << v.h_symbol_subtype;
    os << v.h_symbol_lib;
    os << v.h_symbol_offset;

    return os;
}
inline TStream &operator >> (TStream &os, h_info_symbol &v) {
    os >> v.h_symbol_name;
    os >> v.h_symbol_type;
    os >> v.h_symbol_subtype;
    os >> v.h_symbol_lib;
    os >> v.h_symbol_offset;

    return os;
}

inline TStream &operator << (TStream &os, const MyImageStruct &v) {
    os << v.h_magic;
    os << v.h_version;

    os << v.h_image_type;
    os << v.h_image_symbols;    uint32_t pos = ftell(os.outfile);
    os << v.h_image_entry;
    os << v.h_image_name;

    os << v.info_lib;
    os << v.info_symbol;        uint32_t img = ftell(os.outfile);

    os << v.h_image_data;

    fseek(os.outfile,pos,SEEK_SET);
    os << img;

    return os;
}

inline TStream &operator >> (TStream &os, MyImageStruct &v) {
    os >> v.h_magic;
    os >> v.h_version;

    os >> v.h_image_type;
    os >> v.h_image_symbols;
    os >> v.h_image_entry;
    os >> v.h_image_name;

    os >> v.info_lib;
    os >> v.info_symbol;

    os >> v.h_image_data;

    return os;
}

class MyHeaderWriter {
public:
     MyHeaderWriter();
    ~MyHeaderWriter();
};

#endif
