#include <string>

unsigned long long bin_string_to_int64(std::string bin) {
    unsigned long long result = 0;
    unsigned long long mult = 1;
    for (int i = bin.size() - 1; i >= 0; i--) {
        if (bin[i] == '1') {
            result += mult;
        }
        mult *= 2;
    }
    return result;
}

int bin_string_to_int(std::string bin) {
    int result = 0;
    int mult = 1;
    for (int i = bin.size() - 1; i >= 0; i--) {
        if (bin[i] == '1') {
            result += mult;
        }
        mult *= 2;
    }
    return result;
}