#include "lutil.h" // Required
#include "lu_storage/map.h"

namespace util = lutil::LUTIL_VERSION;

util::Map<int, const char *> mapping;

void setup() {
    Serial.begin(115200);
    mapping.insert(1, "Test");
    mapping.insert(65, "Another");
}


void loop() {
    Serial.println(mapping[1]);
    Serial.println(mapping[65]);
    delay(500);
}
