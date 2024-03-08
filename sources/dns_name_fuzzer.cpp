#include <iterator>
#include <utility>
#include <vector>

#include "dns_name.hpp"

extern "C" int LLVMFuzzerTestOneInput(uint8_t const* data, size_t size)
{
    uint16_t msg_idx = 0;

    if (size < sizeof(msg_idx)) {
        return 0;
    }

    msg_idx = *reinterpret_cast<uint16_t const*>(data);  // NOLINT

    data += sizeof(msg_idx);
    size -= sizeof(msg_idx);

    if (size < msg_idx) {
        return 0;
    }

    std::vector<std::uint8_t> output(size);
    DNS::UnpackName(std::span{data, size}, msg_idx, output);

    return 0;
}
