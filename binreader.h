#ifndef BINREADER_H
#define BINREADER_H

class binreader : public std::ifstream
{
public:
	const std::string m_Path;
 
    binreader() : std::ifstream(), m_Path("") { }
    binreader(const
        std::string& path,
        std::ios_base::openmode attr =
        std::ios::binary) :
        std::ifstream(path, (attr |
        std::ios::binary)), m_Path(path)
    { }
 
    template<typename T>
    binreader& operator >> (T& m) {
        read(reinterpret_cast<char*>(&m), sizeof(m));
        return *this;
    }
};

class MyHeaderReader {
public:
    MyHeaderReader(std::string n);

    uint32_t codebeg = 0;
    uint32_t codelen = 0;

    uint8_t * image;
    binreader rd;
    
    void call(std::string);
    std::string m_path;
};

#endif
