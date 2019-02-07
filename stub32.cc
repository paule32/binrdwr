#include "binheader.h"

int main(int argc, char **argv)
{
    std::string   str  = "runtime2.ovl";
    
    MyCodeEmitter em(str);  em.write();
//  MyCodeRunner  rc(str);  rc.call ("main");
    
    return 0;
}
