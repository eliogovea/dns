#include "dns_record.hpp"

#include <variant>

namespace DNS {
auto ToRecordDataView(RecordData const& data) -> RecordDataView
{
    return std::visit(
        [](auto&& arg) {
            return RecordDataView(arg.data(), arg.size());
        },
        data);
};
}  // namespace DNS
