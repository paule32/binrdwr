#ifndef BINWRITER_H
#define BINWRITER_H
#pragma once

class binwriter : public std::ofstream
{
public:
	const std::string m_Path;
 
    binwriter() : std::ofstream(), m_Path("") { }
    binwriter(const
        std::string& path,
        std::ios_base::openmode attr =
        std::ios::binary) :
        std::ofstream(path, (attr |
        std::ios::binary)), m_Path(path)
    { }
 
    template<typename T>
    binwriter& operator << (const T& m) {
        static_assert(!std::is_pointer<T>::value, "T must not be a pointer type");
        static_assert(std::is_standard_layout<T>::value, "T must be an standard layout type");
        write(reinterpret_cast<const char*>(&m), sizeof(m));
        return *this;
    }
};

struct h_info_lib {
    std::string  h_lib_name;		// name of lib

	friend std::ostream& operator<<(std::ostream& os, const h_info_lib& m) {
		os << m.h_lib_name;
		return os;
	}
};

struct h_info_symbol {
    std::string  h_symbol_name;
    uint8_t 	 h_symbol_type;     // 0: exec func, 1: lib func/sym,
    uint8_t 	 h_symbol_subtype;  // int, 1: float, 2: string, ...
    uint16_t	 h_symbol_lib;
    uint32_t	 h_symbol_offset;   // 0 - function in image (offset)

	friend std::ostream& operator<<(std::ostream& os, const h_info_symbol& m) {
		os << m.h_symbol_name;
        os << m.h_symbol_type;
        os << m.h_symbol_subtype;
        os << m.h_symbol_lib;
        os << m.h_symbol_offset;
		return os;
	}
};
inline std::ostream& operator<<(std::ostream& os, const std::vector<h_info_lib>& v) {
    os << v;
    return os;
}
inline std::ostream& operator<<(std::ostream& os, const std::vector<h_info_symbol>& v) {
    os << v;
    return os;
}

struct MyImageStruct {
    uint32_t	 h_magic;
    uint16_t	 h_version;

    uint8_t		 h_image_type;		// lib/exec ?
    uint16_t     h_image_name_len;  // str. length
    std::string  h_image_name;		// image name string

    uint32_t	 h_image_len;	// length of code data
    uint8_t 	*h_image_data;	// code data

    std::vector<h_info_lib>     info_lib;
    std::vector<h_info_symbol>  info_symbol;

	friend std::ostream& operator<<(std::ostream& os, const MyImageStruct& m) {
        os << m.h_magic;
        os << m.h_version;

        os << m.h_image_type;
        os << m.h_image_name_len;
		os << m.h_image_name;

        os << m.info_lib;
        os << m.info_symbol;

		return os;
	}
};

class MyHeaderWriter {
public:
/*
    MyHeaderWriter(
         std::string n,
         uint64_t length,
         void   * buffer);*/  MyHeaderWriter();
    ~MyHeaderWriter();
    
    binwriter   wr;
    std::string m_path;
};

#endif
