#pragma once

#include "dns.hpp"

namespace DNS {
auto EncodedSize(const Question& question)  //
  -> std::size_t;                           //

auto EncodedSize(const Record& record)  //
  -> std::size_t;                       //

auto EncodedSize(const Message& message)  //
  -> std::size_t;                         //

auto Encode(std::uint8_t* msg_ptr, std::uint8_t value)  //
  -> std::uint8_t*;                                     //

auto Encode(std::uint8_t* msg_ptr, std::uint16_t value)  //
  -> std::uint8_t*;                                      //

auto Encode(std::uint8_t* msg_ptr, std::uint32_t value)  //
  -> std::uint8_t*;                                      //

auto Encode(std::uint8_t* msg_ptr, const Header& header)  //
  -> std::uint8_t*;                                       //

auto Encode(std::uint8_t* msg_ptr, const Question& question)  //
  -> std::uint8_t*;                                           //

auto Encode(std::uint8_t* msg_ptr, const Record& record)  //
  -> std::uint8_t*;                                       //

auto Encode(std::uint8_t* msg_ptr, const Message& message)  //
  -> std::uint8_t*;                                         //
}  // namespace DNS
