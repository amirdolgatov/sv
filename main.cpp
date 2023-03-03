#include <iostream>
#include "sv_classes.h"
#include <typeinfo>

int main() {

    SV_generator gen;
    SV_attribute_values s;

    gen.initialize_ASDU(s);
    gen.build_SV_block();
    gen.build_frame();
    gen.print_frame();


    return 0;
}


