#include "binheader.h"

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
    const auto addr  = reinterpret_cast<size_t>(&my_func2);

    asmjit::Label L1 = x86_compiler.newLabel();
    asmjit::Label L2 = x86_compiler.newLabel();

    const char* text = "Hello Computer\n";

    x86_emitter->nop ();
    x86_emitter->push(x86::ebp);
    x86_emitter->mov (x86::ebp, x86::esp);
    x86_emitter->sub (x86::esp, 16);

    x86_emitter->mov (x86::ebx, addr);
//    x86_emitter->lea (x86::rax, x86::ptr(L1));
    x86_emitter->call(x86::ebx);

    x86_emitter->jmp(L2);

    x86_emitter->nop ();
    x86_emitter->nop ();
    x86_emitter->nop ();


    x86_emitter->bind (L1);
    x86_emitter->embed(text, strlen(text)+1);


    // function end:
    x86_emitter->bind(L2);
    x86_emitter->mov (x86::rbx, addr);
//    x86_emitter->lea (x86::rax, x86::ptr(L1));
    x86_emitter->call(x86::rbx);

    x86_emitter->add (x86::rsp, 16);
    x86_emitter->mov (x86::esp, x86::rbp);
    x86_emitter->pop (x86::rbp);
    x86_emitter->ret();

    x86_compiler.endFunc();

    return 0;
}

MyCodeEmitter::MyCodeEmitter()
{
    x86_compiler.finalize();
std::cout << "final\n";
    
    // init code ...
    x86_codeholder.init(x86_runtime.getCodeInfo());
    x86_codeholder.attach(&x86_compiler);
    x86_emitter = x86_compiler.asEmitter();
    
    int r = emitTest();
std::cout << "emitser\n";

    // save code ...
    CodeBuffer & buffer = x86_codeholder.getSectionEntry(0)->getBuffer();
    code_len   = buffer.getLength();
    code_data  = buffer.getData();

    x86_codeholder.detach(&x86_compiler);
}

