#include "lutil.h" // Required
#include "lu_storage/map.h"

namespace util = lutil::LUTIL_VERSION;

Map<int, String> map;

void setup() {
    Serial.begin(115200);
    map.insert(1, "Test");
    map.insert(65, "Another");
}


void loop() {
    Serial.println(map[1]);
    Serial.println(map[65]);
    delay(500);
}
