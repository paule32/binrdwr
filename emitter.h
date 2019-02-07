#ifndef MYCODEEMITTER_H_
#define MYCODEEMITTER_H_
#pragma once

class MyCodeEmitter {
    int emitTest();
    std::string lib_name;
public:
    MyCodeEmitter();
    
    void write();

    uint32_t   code_len  ;
    uint8_t  * code_data ;

       
    asmjit::X86Compiler  x86_compiler;
    asmjit::X86Emitter * x86_emitter ;
    
    asmjit::JitRuntime   x86_runtime;
    asmjit::CodeHolder   x86_codeholder;
};

class MyCodeRunner {
    std::string lib_name;
public:
    MyCodeRunner(std::string name): lib_name(name) { }
    int     call(std::string fun );
};

#endif
