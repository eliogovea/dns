#include "dns/encode.hpp"

#include <arpa/inet.h>

#include <cstring>
#include <numeric>

namespace DNS {
Header MakeHeader(const Message& message) {
    return {
      message.id,                                             //
      message.flags,                                          //
      0x0001,                                                 //
      static_cast<std::uint16_t>(message.answers.size()),     //
      static_cast<std::uint16_t>(message.authority.size()),   //
      static_cast<std::uint16_t>(message.additional.size()),  //
    };
}

auto EncodedSize(const Question& question)  //
  -> std::size_t                            //
{
    return ToNameView(question.name).size()  //
           + sizeof(question.type)           //
           + sizeof(question.class_);        //
}

auto EncodedSize(const Record& record)  //
  -> std::size_t                        //
{
    return ToNameView(record.name).size()           //
           + sizeof(record.type)                    //
           + sizeof(record.class_)                  //
           + sizeof(record.time_to_live)            //
           + sizeof(std::uint16_t)                  //
           + ToRecordDataView(record.data).size();  //
}

auto EncodedSize(const std::vector<Record>& records)  //
  -> std::size_t                                      //
{
    return std::accumulate(                         //
      std::cbegin(records),                         //
      std::cend(records),                           //
      std::size_t {0},                              //
      [](std::size_t size, const Record& record) {  //
          return size + EncodedSize(record);        //
      });                                           //
}

auto EncodedSize(const Message& message)  //
  -> std::size_t                          //
{
    return sizeof(Header)                      //
           + EncodedSize(message.question)     //
           + EncodedSize(message.answers)      //
           + EncodedSize(message.authority)    //
           + EncodedSize(message.additional);  //
}

auto Encode(std::uint8_t* msg_ptr, std::uint8_t value)  //
  -> std::uint8_t*                                      //
{
    *msg_ptr = value;
    return msg_ptr + sizeof(value);
}

auto Encode(std::uint8_t* msg_ptr, std::uint16_t value)  //
  -> std::uint8_t*                                       //
{
    value = htons(value);
    std::memcpy(msg_ptr, &value, sizeof(value));
    return msg_ptr + sizeof(value);
}

auto Encode(std::uint8_t* msg_ptr, std::uint32_t value)  //
  -> std::uint8_t*                                       //
{
    value = htonl(value);
    std::memcpy(msg_ptr, &value, sizeof(value));
    return msg_ptr + sizeof(value);
}

auto Encode(std::uint8_t* msg_ptr, const std::span<const std::uint8_t>& data)  //
  -> std::uint8_t*                                                             //
{
    std::memcpy(msg_ptr, data.data(), data.size());
    return msg_ptr + data.size();
}

auto Encode(std::uint8_t* msg_ptr, const Header& header)  //
  -> std::uint8_t*                                        //
{
    msg_ptr = Encode(msg_ptr, header.id);
    msg_ptr = Encode(msg_ptr, header.flags);
    msg_ptr = Encode(msg_ptr, header.question_count);
    msg_ptr = Encode(msg_ptr, header.answer_count);
    msg_ptr = Encode(msg_ptr, header.authority_count);
    msg_ptr = Encode(msg_ptr, header.additional_count);
    return msg_ptr;
}

auto Encode(std::uint8_t* msg_ptr, const Question& question)  //
  -> std::uint8_t*                                            //
{
    msg_ptr = Encode(msg_ptr, ToNameView(question.name));
    msg_ptr = Encode(msg_ptr, question.type);
    msg_ptr = Encode(msg_ptr, question.class_);
    return msg_ptr;
}

auto Encode(std::uint8_t* msg_ptr, const Record& record)  //
  -> std::uint8_t*                                        //
{
    msg_ptr = Encode(msg_ptr, ToNameView(record.name));
    msg_ptr = Encode(msg_ptr, record.type);
    msg_ptr = Encode(msg_ptr, record.class_);
    msg_ptr = Encode(msg_ptr, record.time_to_live);
    msg_ptr = Encode(msg_ptr, static_cast<std::uint16_t>(ToRecordDataView(record.data).size()));
    msg_ptr = Encode(msg_ptr, ToRecordDataView(record.data));
    return msg_ptr;
}

auto Encode(std::uint8_t* msg_ptr, const Message& message)  //
  -> std::uint8_t*                                          //
{
    msg_ptr = Encode(msg_ptr, MakeHeader(message));
    msg_ptr = Encode(msg_ptr, message.question);
    for (auto record : message.answers) {
        msg_ptr = Encode(msg_ptr, record);
    }
    for (auto record : message.authority) {
        msg_ptr = Encode(msg_ptr, record);
    }
    for (auto record : message.additional) {
        msg_ptr = Encode(msg_ptr, record);
    }
    return msg_ptr;
}
}  // namespace DNS
