#include "dns/record.hpp"

namespace DNS {
auto ToRecordDataView(const RecordData& data)  //
  -> RecordDataView                            //
{
    return std::visit([](auto&& arg) { return RecordDataView(arg.data(), arg.size()); }, data);
};
}  // namespace DNS
