#pragma once

#include <cstdint>
#include <span>
#include <variant>
#include <vector>

#include "name.hpp"

namespace DNS {
using RecordDataView  = std::span<const uint8_t>;
using RecordDataOwner = std::vector<uint8_t>;
using RecordData      = std::variant<RecordDataView, RecordDataOwner>;

struct Record {
    Name name;
    std::uint16_t type;
    std::uint16_t class_;
    std::uint32_t time_to_live;
    RecordData data;
};

auto ToRecordDataView(const RecordData& data)  //
  -> RecordDataView;                           //

}  // namespace DNS
