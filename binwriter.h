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

	template<typename T>
	binwriter& operator<<(const std::vector<T>& v) {
		for(auto&& e : v)
            *this << e;
		return *this;
	}
};

template<>
inline binwriter& binwriter::operator<<(const std::string& m) {
	write(m.c_str(), m.size());
	return *this;
}

struct h_image_data_struct {
    std::string       data_name;
    uint32_t          data_len;
    uint8_t         * data;
	friend binwriter& operator << (binwriter& os, const h_image_data_struct & m) {
        os.write(reinterpret_cast<char*>(m.data), m.data_len);
    	return os;
    }
};
extern std::map<std::string,uint8_t*> image_data;

struct h_info_lib {
    uint16_t     h_lib_name_len;
    std::string  h_lib_name;		// name of lib

	friend binwriter& operator << (binwriter& os, const h_info_lib& m) {
        os << m.h_lib_name_len;
		os << m.h_lib_name;
		return os;
	}
};

struct h_info_symbol {
    uint16_t     h_symbol_name_len;
    std::string  h_symbol_name;
    uint8_t 	 h_symbol_type;     // 0: exec func, 1: lib func/sym,
    uint8_t 	 h_symbol_subtype;  // int, 1: float, 2: string, ...
    uint16_t	 h_symbol_lib;
    uint32_t	 h_symbol_offset;   // 0 - function in image (offset)

	friend binwriter& operator << (binwriter& os, const h_info_symbol& m) {
        os << m.h_symbol_name_len;
		os << m.h_symbol_name;
        os << m.h_symbol_type;
        os << m.h_symbol_subtype;
        os << m.h_symbol_lib;
        os << m.h_symbol_offset;
		return os;
	}
};

struct MyImageStruct {
    uint32_t	 h_magic;
    uint16_t	 h_version;

    uint8_t		 h_image_type;		// lib/exec ?
    uint32_t     h_image_entry;

    uint16_t     h_image_name_len;  // str. length
    std::string  h_image_name;		// image name string

    uint32_t	 h_image_len;	// length of code data
    uint8_t 	*h_image_data;	// code data

    std::vector<h_info_lib>     info_lib;
    std::vector<h_info_symbol>  info_symbol;

	friend binwriter& operator << (binwriter& os, const MyImageStruct& m) {
        os << m.h_magic;
        os << m.h_version;

        os << m.h_image_type;
        os << m.h_image_entry;

        os << m.h_image_name_len;
		os << m.h_image_name;

		os << m.info_lib;
		os << m.info_symbol;

		return os;
	}
};

class MyHeaderWriter {
public:
     MyHeaderWriter();
    ~MyHeaderWriter();
    
    binwriter   wr;
};

#endif
