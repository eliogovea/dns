#pragma once

#include <cstdint>

#include "record.hpp"

namespace DNS {
struct __attribute__((packed)) Header {
    std::uint16_t id;
    std::uint16_t flags;
    std::uint16_t question_count;
    std::uint16_t answer_count;
    std::uint16_t authority_count;
    std::uint16_t additional_count;
};

struct Question {
    Name name;
    std::uint16_t type;
    std::uint16_t class_;
};

struct Message {
    std::uint16_t id;
    std::uint16_t flags;
    Question question;
    std::vector<Record> answers;
    std::vector<Record> authority;
    std::vector<Record> additional;
};

constexpr std::uint16_t FlagsMaskQuery               = 0x8000;
constexpr std::uint16_t FlagsMaskOpCode              = 0x7800;
constexpr std::uint16_t FlagsMaskAuthoritativeAnswer = 0x0400;
constexpr std::uint16_t FlagsMaskTruncation          = 0x0200;
constexpr std::uint16_t FlagsMaskRecursionDesired    = 0x0100;
constexpr std::uint16_t FlagsMaskRecursionAvailable  = 0x0080;
constexpr std::uint16_t FlagsMaskZ                   = 0x0070;
constexpr std::uint16_t FlagsMaskResponseCode        = 0x000F;

constexpr std::uint16_t FlagsValueOpCodeStandardQuery             = 0x0000;
constexpr std::uint16_t FlagsValueOpCodeInverseQuery              = 0x0800;
constexpr std::uint16_t FlagsValueOpCodeServerStatus              = 0x1000;
constexpr std::uint16_t FlagsValueResponseCodeOk                  = 0x0000;
constexpr std::uint16_t FlagsValueResponseCodeErrorFormat         = 0x0001;
constexpr std::uint16_t FlagsValueResponseCodeErrorServerFailure  = 0x0002;
constexpr std::uint16_t FlagsValueResponseCodeErrorName           = 0x0003;
constexpr std::uint16_t FlagsValueResponseCodeErrorNotImplemented = 0x0004;
constexpr std::uint16_t FlagsValueResponseCodeErrorRefused        = 0x0005;

constexpr std::uint16_t TypeA     = 0x0001;
constexpr std::uint16_t TypeCNAME = 0x0005;
constexpr std::uint16_t TypeAAAA  = 0x001C;

constexpr std::uint16_t ClassIN = 0x0001;

constexpr std::uint16_t UpdMessageMaxSize = 512;
}  // namespace DNS
