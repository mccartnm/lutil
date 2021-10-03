/*
    ini format reader
*/
#include "lutil.h"
#include "lu_storage/map.h"
#include "lu_memory/managed_ptr.h"

namespace lutil {

class Ini {

public:
    explicit Ini(String data);
    explicit Ini(lutil::managed_data data);

    template<typename T>
    T value(const char *name, const char *section = nullptr) const
    {
        // get_string()
        return T();
    }

    // String get_string(const char *name, const char *section = nullptr) const;

    void set_value(const char *name, String value, const char *section = nullptr);
    void set_value(const char *name, int value, const char *section = nullptr);
    void set_value(const char *name, float value, const char *section = nullptr);
    void set_value(const char *name, bool value, const char *section = nullptr);

private:
    void parse();

    Map<lutil::managed_string, lutil::managed_string> _data;
};

}