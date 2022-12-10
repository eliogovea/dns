#include <gtest/gtest.h>

#include <dns/name.hpp>

TEST(dns_name_compression, read_compression_pointer) {
    std::vector<uint8_t> data = {0xc0, 0x0C};
    auto value                = DNS::ReadNamePointer(data.data());
    EXPECT_EQ(value, 0x0C);
}

TEST(dns_name, skip_name_without_compression) {
    std::vector<uint8_t> name = {
      0x03, 'a', 'b', 'c',  //
      0x03, 'x', 'y', 'z',  //
      0x00                  //
    };
    auto name_end = DNS::SkipName(name);
    EXPECT_EQ(name_end - name.data(), name.size());
}

TEST(dns_name, unpacke_name_without_compression) {
    std::vector<uint8_t> msg = {0x03, 'a', 'b', 'c',  //
                                0x03, 'x', 'y', 'z',  //
                                0x00};

    std::vector<uint8_t> name_buffer(msg.size());

    auto name_end = DNS::UnpackName(std::span {msg}, 0, std::span {name_buffer});

    EXPECT_NE(name_end, nullptr);

    auto name = DNS::NameView {name_buffer.data(), name_end};

    EXPECT_EQ(name.size(), msg.size());
    for (size_t i = 0; i < msg.size(); i++) {
        EXPECT_EQ(name[i], msg[i]);
    }

    EXPECT_NE(name_end, nullptr);
    EXPECT_EQ(name.size(), msg.size());
    for (size_t i = 0; i < msg.size(); i++) {
        EXPECT_EQ(msg[i], name_buffer[i]);
    }
}

TEST(dns_name, unpack_name_with_compression) {
    const std::vector<uint8_t> msg = {
      0x02, 'a', 'b',       //
      0x00,                 //
      0x03, 'a', 'b', 'c',  //
      0x03, 'x', 'y', 'z',  //
      0xc0, 0x00            // pointer to index 0
    };

    const auto msg_idx = size_t {4};

    const std::vector<uint8_t> name_expected = {
      0x03, 'a', 'b', 'c',  //
      0x03, 'x', 'y', 'z',  //
      0x02, 'a', 'b',       //
      0x00,                 //
    };

    std::vector<uint8_t> name_buffer(name_expected.size());

    auto name_end = DNS::UnpackName(std::span {msg}, msg_idx, std::span {name_buffer});

    EXPECT_NE(name_end, nullptr);

    auto name = DNS::NameView {name_buffer.data(), name_end};

    EXPECT_EQ(name.size(), name_expected.size());
    for (size_t i = 0; i < name_expected.size(); i++) {
        EXPECT_EQ(name[i], name_expected[i]);
    }
}

TEST(dns_name, unpack_name_loop) {
    const std::vector<uint8_t> msg = {
      0x03, 'a', 'b', 'c',  //
      0x03, 'x', 'y', 'z',  //
      0xc0, 0x00            // pointer to index 0
    };

    const auto msg_ptr = size_t {0};

    std::vector<uint8_t> name_buffer(msg.size());

    auto name_end = DNS::UnpackName(std::span {msg}, msg_ptr, std::span {name_buffer});

    EXPECT_EQ(name_end, nullptr);
}

TEST(dns_name, unpack_name_with_bad_pointer) {
    const std::vector<uint8_t> msg = {
      0x03, 'a', 'b', 'c',  //
      0x03, 'x', 'y', 'z',  //
      0xc0, 0x01            // pointer to  index 1 ('a')
    };

    const auto msg_ptr = size_t {0};

    std::vector<uint8_t> name_buffer(msg.size());

    auto name_end = DNS::UnpackName(std::span {msg}, msg_ptr, std::span {name_buffer});

    EXPECT_EQ(name_end, nullptr);
}

TEST(dns_name, unpack_name_with_incomplete_label) {
    const std::vector<uint8_t> msg = {
      0x03, 'a', 'b', 'c',  //
      0x03, 'x', 'y'        //
    };

    const auto msg_ptr = size_t {0};

    std::vector<uint8_t> name_buffer(msg.size());

    auto name_end = DNS::UnpackName(std::span {msg}, msg_ptr, std::span {name_buffer});

    EXPECT_EQ(name_end, nullptr);
}

TEST(dns_name, valid_name_to_dots) {
    const DNS::NameOwner name = {
      0x03, 'a', 'b', 'c',  //
      0x03, 'x', 'y', 'z',  //
      0x00                  //
    };

    EXPECT_EQ(DNS::NameToDots(name), "abc.xyz");
}

TEST(dns_name, valid_dots_to_name) {
    const DNS::NameOwner name_expected = {
      0x03, 'a', 'b', 'c',  //
      0x03, 'x', 'y', 'z',  //
      0x00                  //
    };

    EXPECT_EQ(DNS::DotsToName("abc.xyz"), name_expected);
}
