#include <iterator>
#include <utility>
#include <vector>

#include "dns_decode.hpp"

extern "C" int LLVMFuzzerTestOneInput(uint8_t const* data, size_t size)
{
    DNS::Message message;
    DNS::Decode(data, data + size, &message);
    return 0;
}
