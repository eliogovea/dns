#include "dns/decode.hpp"

#include <arpa/inet.h>

#include <cstring>
#include <optional>

#include "dns/name.hpp"

namespace DNS {
auto Decode(const std::uint8_t* msg_ptr, std::uint8_t* value)  //
  -> const std::uint8_t*                                       //
{
    *value = *msg_ptr;
    return msg_ptr + sizeof(*value);
}

auto Decode(const std::uint8_t* msg_ptr, std::uint16_t* value)  //
  -> const std::uint8_t*                                        //
{
    std::memcpy(value, msg_ptr, sizeof(*value));
    *value = ntohs(*value);
    return msg_ptr + sizeof(*value);
}

auto Decode(const std::uint8_t* msg_ptr, std::uint32_t* value)  //
  -> const std::uint8_t*                                        //
{
    std::memcpy(value, msg_ptr, sizeof(*value));
    *value = ntohl(*value);
    return msg_ptr + sizeof(*value);
}

auto Decode(const std::uint8_t* msg_ptr, Header* header)  //
  -> const std::uint8_t*                                  //
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

auto DecodeName(const std::uint8_t* msg_ptr)  //
  -> std::optional<NameView>                  //
{
    const auto* name_end = SkipName(msg_ptr);
    return name_end != nullptr ? std::make_optional(NameView {msg_ptr, name_end}) : std::nullopt;
}

auto DecodeName(const std::uint8_t* msg_ptr, Name* name)  //
  -> const std::uint8_t*                                  //
{
    const auto* name_end = SkipName(msg_ptr);
    *name                = NameView(msg_ptr, name_end);
    return name_end;
}

auto Decode(const std::uint8_t* msg_ptr, Question* question)  //
  -> const std::uint8_t*                                      //
{
    msg_ptr = DecodeName(msg_ptr, &question->name);
    msg_ptr = Decode(msg_ptr, &question->type);
    msg_ptr = Decode(msg_ptr, &question->class_);
    return msg_ptr;
}

auto DecodeRecordData(const std::uint8_t* msg_ptr, RecordData* data)  //
  -> const std::uint8_t*                                              //
{
    auto length = std::uint16_t {0};
    msg_ptr     = Decode(msg_ptr, &length);
    *data       = RecordDataView {msg_ptr, std::size_t {length}};
    return msg_ptr + length;
}

auto Decode(const std::uint8_t* msg_ptr, Record* record)  //
  -> const std::uint8_t*                                  //
{
    msg_ptr = DecodeName(msg_ptr, &record->name);
    msg_ptr = Decode(msg_ptr, &record->type);
    msg_ptr = Decode(msg_ptr, &record->class_);
    msg_ptr = Decode(msg_ptr, &record->time_to_live);
    msg_ptr = DecodeRecordData(msg_ptr, &record->data);
    return msg_ptr;
}

auto Decode(const std::uint8_t* msg_ptr, std::uint16_t count, std::vector<Record>* records)  //
  -> const std::uint8_t*                                                                     //
{
    for (std::uint32_t i = 0; i < count; i++) {
        records->push_back({});
        msg_ptr = Decode(msg_ptr, &records->back());
    }

    return msg_ptr;
}

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, std::uint8_t* value)  //
  -> const std::uint8_t*                                                                    //
{
    return (static_cast<int>(sizeof(*value)) <= msg_end - msg_ptr)  //
             ? Decode(msg_ptr, value)
             : nullptr;
}

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, std::uint16_t* value)  //
  -> const std::uint8_t*                                                                     //
{
    return (static_cast<int>(sizeof(*value)) <= msg_end - msg_ptr)  //
             ? Decode(msg_ptr, value)
             : nullptr;
}

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, std::uint32_t* value)  //
  -> const std::uint8_t*                                                                     //
{
    return (static_cast<int>(sizeof(*value)) <= msg_end - msg_ptr)  //
             ? Decode(msg_ptr, value)
             : nullptr;
}

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, Header* header)  //
  -> const std::uint8_t*                                                               //
{
    return (static_cast<int>(sizeof(*header)) <= msg_end - msg_ptr)  //
             ? Decode(msg_ptr, header)
             : nullptr;
}

auto DecodeName(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end)  //
  -> std::optional<NameView>                                               //
{
    const auto* name_end = SkipName(msg_ptr, msg_end);
    return name_end != nullptr ? std::make_optional(NameView {msg_ptr, name_end}) : std::nullopt;
}

auto DecodeName(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, Name* name)  //
  -> const std::uint8_t*                                                               //
{
    const auto* name_end = SkipName(std::span<const std::uint8_t> {msg_ptr, msg_end});
    if (name_end != nullptr) {
        *name = NameView {msg_ptr, name_end};
    }
    return name_end;
}

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, Question* question)  //
  -> const std::uint8_t*                                                                   //
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

auto DecodeRecordData(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, RecordData* data)  //
  -> const std::uint8_t*                                                                           //
{
    auto length = std::uint16_t {0};
    msg_ptr     = Decode(msg_ptr, &length);

    if (msg_ptr == nullptr || msg_end - msg_ptr < length) {
        return nullptr;
    }

    *data = RecordDataView {msg_ptr, length};
    return msg_ptr + length;
}

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, Record* record)  //
  -> const std::uint8_t*                                                               //
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

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end,  //
            std::uint16_t count, std::vector<Record>* records)         //
  -> const std::uint8_t*                                               //
{
    for (std::uint32_t i = 0; i < count && msg_ptr != nullptr; i++) {
        records->push_back({});
        msg_ptr = Decode(msg_ptr, msg_end, &records->back());
    }

    return msg_ptr;
}

auto Decode(const std::uint8_t* msg_ptr, Message* message)  //
  -> const std::uint8_t*                                    //
{
    auto header    = Header {};
    msg_ptr        = Decode(msg_ptr, &header);
    message->id    = header.id;
    message->flags = header.flags;

    msg_ptr = Decode(msg_ptr, &message->question);
    msg_ptr = Decode(msg_ptr, header.answer_count, &message->answers);
    msg_ptr = Decode(msg_ptr, header.authority_count, &message->authority);
    msg_ptr = Decode(msg_ptr, header.additional_count, &message->additional);

    return msg_ptr;
}

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, Message* message)  //
  -> const std::uint8_t*                                                                 //
{
    auto header = Header {};
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

    msg_ptr = Decode(msg_ptr, msg_end, header.authority_count, &message->authority);

    if (msg_ptr == nullptr) {
        return nullptr;
    }

    msg_ptr = Decode(msg_ptr, msg_end, header.additional_count, &message->additional);

    return msg_ptr;
}
}  // namespace DNS
