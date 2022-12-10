#include <gtest/gtest.h>

#include <dns/decode.hpp>

TEST(dns_decoder, decode_16_bits_integer) {
    const std::vector<uint8_t> msg = {
      0x00, 0x1c,  // AAAA
    };

    std::uint16_t value = 0;
    DNS::Decode(msg.data(), &value);
    EXPECT_EQ(value, DNS::TypeAAAA);
}

TEST(dns, question) {
    const std::vector<uint8_t> msg = {
      0x03, 'a',  'b', 'c',  //
      0x03, 'x',  'y', 'z',  //
      0x00,                  //
      0x00, 0x1c,            // AAAA
      0x00, 0x01             // IN
    };

    DNS::Question question;
    auto question_end = DNS::Decode(msg.data(), &question);

    EXPECT_EQ(question_end, msg.data() + msg.size());
    EXPECT_EQ(DNS::ToNameView(question.name).data(), msg.data());
    EXPECT_EQ(DNS::ToNameView(question.name).size(), 9);
    EXPECT_EQ(DNS::NameToDots(DNS::ToNameView(question.name)), "abc.xyz");

    EXPECT_EQ(question.type, DNS::TypeAAAA);
    EXPECT_EQ(question.class_, DNS::ClassIN);
}

TEST(dns_decode, record) {
    const std::vector<uint8_t> msg = {
      0x03, 'a',  'b',  'c',   //
      0x03, 'x',  'y',  'z',   //
      0x00,                    //
      0x00, 0x01,              // A
      0x00, 0x01,              // IN
      0x00, 0x00, 0x00, 0x0A,  // TTL
      0x00, 0x04,              // length
      0x0A, 0x0B, 0x0C, 0x0D   //
    };

    DNS::Record record;
    auto record_end = DNS::Decode(msg.data(), &record);

    EXPECT_EQ(record_end - msg.data(), msg.size());
}
