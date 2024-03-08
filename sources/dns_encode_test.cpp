#include <gtest/gtest.h>

#include "dns_encode.hpp"

TEST(dns_encoder, encode_record)
{
    EXPECT_TRUE(true);

    DNS::Record record = {
        DNS::NameOwner{0xc0, 0x0c},
        DNS::TypeA,
        DNS::ClassIN,
        0x0000000A,
        DNS::RecordDataOwner{0x0A, 0x0B, 0x0C, 0x0D}
    };

    // clang-format off
    auto const expected = std::vector<std::uint8_t>{
        0xc0, 0x0c,             // name (pointer)
        0x00, 0x01,             // type
        0x00, 0x01,             // class
        0x00, 0x00, 0x00, 0x0A, // ttl
        0x00, 0x04,             // rdata length
        0x0A, 0x0B, 0x0C, 0x0D  // rdata
    };
    // clang-format on

    auto encoded_buffer = std::vector<std::uint8_t>(DNS::EncodedSize(record));
    auto encoded_end    = DNS::Encode(encoded_buffer.data(), record);

    EXPECT_EQ(encoded_end - encoded_buffer.data(), std::size(expected));

    EXPECT_EQ(encoded_buffer, expected);
}
