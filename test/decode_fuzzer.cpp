#include <iterator>
#include <utility>
#include <vector>

//
#include <dns/decode.hpp>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    DNS::Message message;
    DNS::Decode(data, data + size, &message);
    return 0;
}
