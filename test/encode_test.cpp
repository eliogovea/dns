#include <gtest/gtest.h>

#include <dns/encode.hpp>

TEST(dns_encoder, encode_record) {
    DNS::Record record = {
      DNS::NameOwner {0xc0, 0x0c},                   //
      DNS::TypeA,                                    //
      DNS::ClassIN,                                  //
      0x0000000A,                                    //
      DNS::RecordDataOwner {0x0A, 0x0B, 0x0C, 0x0D}  //
    };

    std::vector<uint8_t> expected = {
      0xc0, 0x0c,              //
      0x00, 0x01,              //
      0x00, 0x01,              //
      0x00, 0x00, 0x00, 0x0A,  //
      0x00, 0x04,              //
      0x0A, 0x0B, 0x0C, 0x0D   //
    };

    EXPECT_EQ(DNS::EncodedSize(record), expected.size());

    std::vector<std::uint8_t> encoded(DNS::EncodedSize(record));
    auto encoded_end = DNS::Encode(encoded.data(), record);

    EXPECT_EQ(encoded_end - encoded.data(), expected.size());

    EXPECT_EQ(encoded, expected);
}