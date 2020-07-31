#include "lutil.h" // Required
#include "lu_storage/map.h"

#include "lu_output/printer.h"

namespace util = lutil::LUTIL_VERSION;

util::Map<int, const char *> mapping;

void setup() {
    Serial.begin(115200);
    mapping.insert(1, "Test");
    mapping[65] = "Another";
}


void loop() {
    Serial.println(mapping[1]);
    Serial.println(mapping[65]);

    // Iteration
    auto it = mapping.begin();
    for (; it != mapping.end(); it++) {
        util::Printer::print("Key: % Value: %", it.key(), it.value());
    }

    delay(500);
}
