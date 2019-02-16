#include "binheader.h"
#include "externals.h"

void my_func(int p) { std::printf("----> %d\n",p); }
void my_func2(void) {
    printf("Hello World !!!\n");
    printf("numser: %d\n", 1212);
}

void my_func3(char *p) {
    printf("---> %s\n",p);
}

int MyCodeEmitter::emitTest()
{
//    const auto GOTaddr = reinterpret_cast<uint32_t>(&externals);

    uint64_t GOTaddr64    = reinterpret_cast<uint64_t>(&externals);
    uint32_t GOTaddr32_hd =  GOTaddr64 & 0xFFFFFFFF;

    asmjit::Label x86_testfunc = x86_compiler.newLabel();
    asmjit::Label x86_main     = x86_compiler.newLabel();

/* entry point */
    x86_emitter->call(x86_main);
    x86_emitter->ret();

/* testfunc */
    x86_compiler.bind(x86_testfunc);
    //
    x86_emitter->push(x86::ebp);
    x86_emitter->mov (x86::ebp, x86::esp);
    x86_emitter->sub (x86::esp, 24);
    x86_emitter->mov (x86::eax, x86::dword_ptr(GOTaddr32_hd));
    x86_emitter->mov (x86::dword_ptr( x86::ebp, -12), x86::eax);
    x86_emitter->sub (x86::esp, 12);
    x86_emitter->push(x86::ptr( x86::ebp, +8));
    x86_emitter->mov (x86::eax, x86::dword_ptr(x86::ebp, -12));
    x86_emitter->call(x86::eax);
    x86_emitter->add (x86::esp, 16);
    x86_emitter->leave();
    x86_emitter->ret();

/* main */
    asmjit::Label x86_str_L1 = x86_compiler.newLabel();
    char str[] = "Monk";
    x86_emitter->bind (x86_str_L1);
    x86_emitter->embed(str, strlen(str)+1);
    //
    x86_compiler.bind(x86_main);
    //
    x86_emitter->lea (x86::ecx, x86::dword_ptr(x86::esp, 4));
    x86_emitter->and_(x86::esp, -16);
    x86_emitter->push(x86::ptr(x86::ecx, -4));
    x86_emitter->push(x86::ebp);
    x86_emitter->mov (x86::ebp, x86::esp);
    x86_emitter->push(x86::ecx);
    x86_emitter->sub (x86::esp, 4);
    x86_emitter->sub (x86::esp, 12);
    x86_emitter->lea (x86::eax, x86::ptr(x86_str_L1));
    x86_emitter->call(x86_testfunc);
    x86_emitter->add (x86::esp, 16);
    x86_emitter->push(x86::eax);
    x86_emitter->mov (x86::eax, 0);
    x86_emitter->mov (x86::ecx, x86::dword_ptr(x86::ebp, -4));
    x86_emitter->leave();
    x86_emitter->lea (x86::esp, (x86::ecx, -4));
    x86_emitter->ret();

    x86_compiler.endFunc();
    x86_compiler.finalize();


//    x86_emitter->mov (x86::ebx, addr);
//    x86_emitter->lea (x86::rax, x86::ptr(L1));
//    x86_emitter->call(x86::ebx);

//    x86_emitter->jmp(L2);


//    x86_emitter->bind (L1);
//    x86_emitter->embed(text, strlen(text)+1);


    // function end:
//    x86_emitter->bind(L2);
//    x86_emitter->mov (x86::rbx, addr);
//    x86_emitter->lea (x86::rax, x86::ptr(L1));
//    x86_emitter->call(x86::rbx);

    return 0;
}


MyCodeEmitter::MyCodeEmitter()
{
    // init code ...
    x86_codeholder.init(x86_runtime.getCodeInfo());
    x86_codeholder.attach(&x86_compiler);
    x86_emitter = x86_compiler.asEmitter();

    int r = emitTest();

    // save code ...
    CodeBuffer & buffer = x86_codeholder.getSectionEntry(0)->getBuffer();
    code_len   = buffer.getLength();
    code_data  = new uint8_t[code_len];

    std::cout << "len: " << code_len << std::endl;

    memcpy(code_data, buffer.getData(), code_len);
    x86_codeholder.detach(&x86_compiler);
}

