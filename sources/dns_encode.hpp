#pragma once

#include "dns.hpp"

namespace DNS {
auto EncodedSize(Question const& question) -> std::size_t;

auto EncodedSize(Record const& record) -> std::size_t;

auto EncodedSize(Message const& message) -> std::size_t;

auto Encode(std::uint8_t* msg_ptr, std::uint8_t value) -> std::uint8_t*;

auto Encode(std::uint8_t* msg_ptr, std::uint16_t value) -> std::uint8_t*;

auto Encode(std::uint8_t* msg_ptr, std::uint32_t value) -> std::uint8_t*;

auto Encode(std::uint8_t* msg_ptr, Header const& header) -> std::uint8_t*;

auto Encode(std::uint8_t* msg_ptr, Question const& question) -> std::uint8_t*;

auto Encode(std::uint8_t* msg_ptr, Record const& record) -> std::uint8_t*;

auto Encode(std::uint8_t* msg_ptr, Message const& message) -> std::uint8_t*;
}  // namespace DNS
