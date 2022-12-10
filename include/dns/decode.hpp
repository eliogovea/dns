#pragma once

#include "dns.hpp"

namespace DNS {
auto Decode(const std::uint8_t* msg_ptr, std::uint8_t* value)  //
  -> const std::uint8_t*;                                      //

auto Decode(const std::uint8_t* msg_ptr, std::uint16_t* value)  //
  -> const std::uint8_t*;                                       //

auto Decode(const std::uint8_t* msg_ptr, std::uint32_t* value)  //
  -> const std::uint8_t*;                                       //

auto Decode(const std::uint8_t* msg_ptr, Header* header)  //
  -> const std::uint8_t*;                                 //

auto Decode(const std::uint8_t* msg_ptr, Question* question)  //
  -> const std::uint8_t*;                                     //

auto Decode(const std::uint8_t* msg_ptr, Record* record)  //
  -> const std::uint8_t*;                                 //

auto Decode(const std::uint8_t* msg_ptr, Message* message)  //
  -> const std::uint8_t*;                                   //

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, std::uint8_t* value)  //
  -> const std::uint8_t*;                                                                   //

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, std::uint16_t* value)  //
  -> const std::uint8_t*;                                                                    //

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, std::uint32_t* value)  //
  -> const std::uint8_t*;                                                                    //

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, Header* header)  //
  -> const std::uint8_t*;                                                              //

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, Question* question)  //
  -> const std::uint8_t*;                                                                  //

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, Record* record)  //
  -> const std::uint8_t*;                                                              //

auto Decode(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end, Message* message)  //
  -> const std::uint8_t*;                                                                //
}  // namespace DNS
