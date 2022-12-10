#include <iterator>
#include <utility>
#include <vector>

//
#include <dns/name.hpp>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    uint16_t msg_idx;

    if (size < sizeof(msg_idx)) {
        return 0;
    }

    msg_idx = *reinterpret_cast<const uint16_t *>(data);

    data += sizeof(msg_idx);
    size -= sizeof(msg_idx);

    if (size < msg_idx) {
        return 0;
    }

    std::vector<std::uint8_t> output(size);
    DNS::UnpackName(std::span {data, size}, msg_idx, output);

    return 0;
}
