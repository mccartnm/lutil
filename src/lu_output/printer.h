#pragma once
#include "lutil.h"

namespace lutil { namespace LUTIL_VERSION {

/* Cheap means of variadic print via the Serial toolkit for Ardiuno */
struct Printer {
    static void print(const char* format) {
        Serial.print(format);
    }

    template<typename T, typename... Targs>
    static void print(const char* format, T value, Targs... Fargs) {
        for ( ; *format != '\0'; format++ ) {
            if ( *format == '%' ) {
               Serial.print(value);
               Printer::print(format+1, Fargs...); // recursive call
               return;
            }
            Serial.print(*format);
        }
    }    
};

}
}