#include "dns_decode.hpp"

#include <arpa/inet.h>

#include <cstring>
#include <optional>

#include "dns_name.hpp"

namespace DNS {
auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t*       value) -> std::uint8_t const*
{
    *value = *msg_ptr;
    return msg_ptr + sizeof(*value);
}

auto Decode(std::uint8_t const* msg_ptr,
            std::uint16_t*      value) -> std::uint8_t const*
{
    std::memcpy(value, msg_ptr, sizeof(*value));
    *value = ntohs(*value);
    return msg_ptr + sizeof(*value);
}

auto Decode(std::uint8_t const* msg_ptr,
            std::uint32_t*      value) -> std::uint8_t const*
{
    std::memcpy(value, msg_ptr, sizeof(*value));
    *value = ntohl(*value);
    return msg_ptr + sizeof(*value);
}

auto Decode(std::uint8_t const* msg_ptr, Header* header) -> std::uint8_t const*
{
    std::memcpy(header, msg_ptr, sizeof(*header));
    header->id               = ntohs(header->id);
    header->flags            = ntohs(header->flags);
    header->question_count   = ntohs(header->question_count);
    header->answer_count     = ntohs(header->answer_count);
    header->authority_count  = ntohs(header->authority_count);
    header->additional_count = ntohs(header->additional_count);
    return msg_ptr + sizeof(*header);
}

auto DecodeName(std::uint8_t const* msg_ptr) -> std::optional<NameView>
{
    auto const* name_end = SkipName(msg_ptr);
    return name_end != nullptr ? std::make_optional(NameView{msg_ptr, name_end})
                               : std::nullopt;
}

auto DecodeName(std::uint8_t const* msg_ptr, Name* name) -> std::uint8_t const*
{
    auto const* name_end = SkipName(msg_ptr);
    *name                = NameView(msg_ptr, name_end);
    return name_end;
}

auto Decode(std::uint8_t const* msg_ptr,
            Question*           question) -> std::uint8_t const*
{
    msg_ptr = DecodeName(msg_ptr, &question->name);
    msg_ptr = Decode(msg_ptr, &question->type);
    msg_ptr = Decode(msg_ptr, &question->class_);
    return msg_ptr;
}

auto DecodeRecordData(std::uint8_t const* msg_ptr,
                      RecordData*         data) -> std::uint8_t const*
{
    auto length = std::uint16_t{0};
    msg_ptr     = Decode(msg_ptr, &length);
    *data       = RecordDataView{msg_ptr, std::size_t{length}};
    return msg_ptr + length;
}

auto Decode(std::uint8_t const* msg_ptr, Record* record) -> std::uint8_t const*
{
    msg_ptr = DecodeName(msg_ptr, &record->name);
    msg_ptr = Decode(msg_ptr, &record->type);
    msg_ptr = Decode(msg_ptr, &record->class_);
    msg_ptr = Decode(msg_ptr, &record->time_to_live);
    msg_ptr = DecodeRecordData(msg_ptr, &record->data);
    return msg_ptr;
}

auto Decode(std::uint8_t const*  msg_ptr,
            std::uint16_t        count,
            std::vector<Record>* records) -> std::uint8_t const*
{
    for (std::uint32_t i = 0; i < count; i++) {
        records->push_back({});
        msg_ptr = Decode(msg_ptr, &records->back());
    }

    return msg_ptr;
}

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            std::uint8_t*       value) -> std::uint8_t const*
{
    return (static_cast<int>(sizeof(*value)) <= msg_end - msg_ptr)
             ? Decode(msg_ptr, value)
             : nullptr;
}

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            std::uint16_t*      value) -> std::uint8_t const*
{
    return (static_cast<int>(sizeof(*value)) <= msg_end - msg_ptr)
             ? Decode(msg_ptr, value)
             : nullptr;
}

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            std::uint32_t*      value) -> std::uint8_t const*
{
    return (static_cast<int>(sizeof(*value)) <= msg_end - msg_ptr)
             ? Decode(msg_ptr, value)
             : nullptr;
}

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            Header*             header) -> std::uint8_t const*
{
    return (static_cast<int>(sizeof(*header)) <= msg_end - msg_ptr)
             ? Decode(msg_ptr, header)
             : nullptr;
}

auto DecodeName(std::uint8_t const* msg_ptr,
                std::uint8_t const* msg_end) -> std::optional<NameView>
{
    auto const* name_end = SkipName(msg_ptr, msg_end);
    return name_end != nullptr ? std::make_optional(NameView{msg_ptr, name_end})
                               : std::nullopt;
}

auto DecodeName(std::uint8_t const* msg_ptr,
                std::uint8_t const* msg_end,
                Name*               name) -> std::uint8_t const*
{
    auto const* name_end
        = SkipName(std::span<std::uint8_t const>{msg_ptr, msg_end});
    if (name_end != nullptr) {
        *name = NameView{msg_ptr, name_end};
    }
    return name_end;
}

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            Question*           question) -> std::uint8_t const*
{
    msg_ptr = DecodeName(msg_ptr, msg_end, &question->name);

    if (msg_ptr == nullptr) {
        return nullptr;
    }

    msg_ptr = Decode(msg_ptr, msg_end, &question->type);

    if (msg_ptr == nullptr) {
        return nullptr;
    }

    msg_ptr = Decode(msg_ptr, msg_end, &question->class_);

    return msg_ptr;
}

auto DecodeRecordData(std::uint8_t const* msg_ptr,
                      std::uint8_t const* msg_end,
                      RecordData*         data) -> std::uint8_t const*
{
    auto length = std::uint16_t{0};
    msg_ptr     = Decode(msg_ptr, &length);

    if (msg_ptr == nullptr || msg_end - msg_ptr < length) {
        return nullptr;
    }

    *data = RecordDataView{msg_ptr, length};
    return msg_ptr + length;
}

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            Record*             record) -> std::uint8_t const*
{
    msg_ptr = DecodeName(msg_ptr, msg_end, &record->name);

    if (msg_ptr == nullptr) {
        return nullptr;
    }

    msg_ptr = Decode(msg_ptr, msg_end, &record->type);
    if (msg_ptr == nullptr) {
        return nullptr;
    }

    msg_ptr = Decode(msg_ptr, msg_end, &record->class_);

    if (msg_ptr == nullptr) {
        return nullptr;
    }

    msg_ptr = Decode(msg_ptr, msg_end, &record->time_to_live);

    if (msg_ptr == nullptr) {
        return nullptr;
    }

    msg_ptr = DecodeRecordData(msg_ptr, msg_end, &record->data);

    return msg_ptr;
}

auto Decode(std::uint8_t const*  msg_ptr,
            std::uint8_t const*  msg_end,
            std::uint16_t        count,
            std::vector<Record>* records) -> std::uint8_t const*
{
    for (std::uint32_t i = 0; i < count && msg_ptr != nullptr; i++) {
        records->push_back({});
        msg_ptr = Decode(msg_ptr, msg_end, &records->back());
    }

    return msg_ptr;
}

auto Decode(std::uint8_t const* msg_ptr,
            Message*            message) -> std::uint8_t const*
{
    auto header    = Header{};
    msg_ptr        = Decode(msg_ptr, &header);
    message->id    = header.id;
    message->flags = header.flags;

    msg_ptr = Decode(msg_ptr, &message->question);
    msg_ptr = Decode(msg_ptr, header.answer_count, &message->answers);
    msg_ptr = Decode(msg_ptr, header.authority_count, &message->authority);
    msg_ptr = Decode(msg_ptr, header.additional_count, &message->additional);

    return msg_ptr;
}

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            Message*            message) -> std::uint8_t const*
{
    auto header = Header{};
    msg_ptr     = Decode(msg_ptr, msg_end, &header);

    if (msg_ptr == nullptr) {
        return nullptr;
    }

    message->id    = header.id;
    message->flags = header.flags;

    if (header.question_count != 1) {
        return nullptr;
    }

    msg_ptr = Decode(msg_ptr, msg_end, &message->question);

    if (msg_ptr == nullptr) {
        return nullptr;
    }

    msg_ptr = Decode(msg_ptr, msg_end, header.answer_count, &message->answers);

    if (msg_ptr == nullptr) {
        return nullptr;
    }

    msg_ptr
        = Decode(msg_ptr, msg_end, header.authority_count, &message->authority);

    if (msg_ptr == nullptr) {
        return nullptr;
    }

    msg_ptr = Decode(
        msg_ptr, msg_end, header.additional_count, &message->additional);

    return msg_ptr;
}
}  // namespace DNS
