#pragma once

#include <cstdint>
#include <cstdlib>
#include <span>
#include <string>
#include <variant>
#include <vector>

namespace DNS {
using NameView  = std::span<const std::uint8_t>;
using NameOwner = std::vector<uint8_t>;
using Name      = std::variant<NameView, NameOwner>;

constexpr auto NameMaxSize = std::uint8_t {0xFF};
constexpr auto NameEnd     = std::uint8_t {0x00};

/**
 * https://www.rfc-editor.org/rfc/rfc1035#section-4.1.4
 *
 * The pointer takes the form of a two octet sequence:
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     | 1  1|                OFFSET                   |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */

constexpr auto LabelMaskType  = std::uint8_t {0xC0};
constexpr auto LabelMaskValue = std::uint8_t {0xFF ^ LabelMaskType};

constexpr auto LabelTypeLength  = std::uint8_t {0x00};
constexpr auto LabelTypePointer = std::uint8_t {LabelMaskType};

constexpr auto LabelMaxLength = LabelMaskValue;

/**
 * @brief
 *
 * @param name
 * @return NameView
 */
auto ToNameView(const Name& name)  //
  -> NameView;                     //

/**
 * @brief Read compression poiner from a DNS message
 * see https://www.rfc-editor.org/rfc/rfc1035#section-4.1.4
 *
 * @param data
 * @return std::uint16_t
 */
auto ReadNamePointer(const std::uint8_t* msg_ptr)  //
  -> std::uint16_t;                                //

/**
 * @brief Skip valid name
 *
 * @param data
 * @return const uint8_t*
 */
auto SkipName(const std::uint8_t* msg_ptr)  //
  -> const uint8_t*;                        //

auto SkipName(const std::uint8_t* msg_ptr, const std::uint8_t* msg_end)  //
  -> const std::uint8_t*;

/**
 * @brief Skip domain name on a DNS message
 *
 * @param data
 * @return const uint8_t*
 */
auto SkipName(std::span<const std::uint8_t> data)  //
  -> const uint8_t*;                               //

/**
 * @brief Unpack a domain name from a DNS message
 *
 * @param msg
 * @param msg_ptr
 * @param msg_end
 * @param name
 * @param name_end
 * @return uint8_t*
 */
auto UnpackName(const uint8_t* msg,      //
                const uint8_t* msg_ptr,  //
                const uint8_t* msg_end,  //
                uint8_t* name,           //
                uint8_t* name_end)       //
  -> uint8_t*;                           //

/**
 * @brief Unpack a domain name from a DNS message
 *
 * @param msg
 * @param msg_idx
 * @param name_buffer
 * @return uint8_t*
 */
auto UnpackName(std::span<const std::uint8_t> msg,    //
                std::size_t msg_idx,                  //
                std::span<std::uint8_t> name_buffer)  //
  -> uint8_t*;                                        //

/**
 * @brief
 *
 * @param name
 * @return NameOwner
 */
auto DotsToName(const std::string& name)  //
  -> NameOwner;                           //

/**
 * @brief
 *
 * @param name
 * @return std::string
 */
auto NameToDots(const NameView& name)  //
  -> std::string;                      //
}  // namespace DNS
