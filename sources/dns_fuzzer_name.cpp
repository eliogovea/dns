#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include <iostream>

#include "dns_name.hpp"

extern "C" int LLVMFuzzerTestOneInput(uint8_t const* data, size_t size)
{
    uint16_t msg_idx = 0;

    if (size < sizeof(msg_idx)) {
        return 0;
    }

    msg_idx = (msg_idx << 8) | *data;
    data++;
    size--;

    msg_idx = (msg_idx << 8) | *data;
    data++;
    size--;

    if (size <= msg_idx) {
        return 0;
    }

    auto output = std::vector<uint8_t>(size);
    DNS::UnpackName(std::span{data, size}, msg_idx, output);

    return 0;
}
