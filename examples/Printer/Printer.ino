/*
    Exmaple of using the printer to make short work
    of multi-element printing in arduino without needing
    the entire sprintf or larger library sets.

    The printer will replace % symbols as required without
    needing any additional fanfare. This has limitations
    obviously.

    Note: This is not very compile-size friendly as the template
    expansion is rather large but it's great for quick debugging
*/
#include "lutil.h"
#include "lu_output/printer.h"

namespace util = lutil::LUTIL_VERSION;

float x, y, z = 0;

void setup() {}

void loop() {
    util::Printer::print("x: % y: % z: %\n", x, y, z);

    x++; 
    y--; 
    z += 2;

    delay(50); 
}
