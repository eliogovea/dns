#pragma once

#include "dns.hpp"

namespace DNS {
auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t*       value) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr,
            std::uint16_t*      value) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr,
            std::uint32_t*      value) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr, Header* header) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr,
            Question*           question) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr, Record* record) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr,
            Message*            message) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            std::uint8_t*       value) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            std::uint16_t*      value) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            std::uint32_t*      value) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            Header*             header) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            Question*           question) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            Record*             record) -> std::uint8_t const*;

auto Decode(std::uint8_t const* msg_ptr,
            std::uint8_t const* msg_end,
            Message*            message) -> std::uint8_t const*;
}  // namespace DNS
