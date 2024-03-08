#include "dns_name.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <span>
#include <string>
#include <variant>

namespace DNS {
auto ToNameView(Name const& name) -> NameView
{
    return std::visit(
        [](auto&& arg) {
            return NameView(arg.data(), arg.size());
        },
        name);
};

auto ReadNamePointer(std::uint8_t const* msg_ptr) -> std::uint16_t
{
    return (static_cast<uint16_t>(*msg_ptr & LabelMaskValue) << 8U)
         | static_cast<uint16_t>(*(msg_ptr + 1));  // NOLINT
}

auto SkipName(std::uint8_t const* msg_ptr) -> uint8_t const*
{
    while (*msg_ptr != NameEnd) {
        switch (*msg_ptr & LabelMaskType) {
        case LabelTypeLength: {
            msg_ptr += sizeof(std::uint8_t) + *msg_ptr;
        } break;
        case LabelTypePointer:
            return msg_ptr + sizeof(std::uint16_t);
        default: {
            return nullptr;
        }
        }
    }

    return msg_ptr + sizeof(std::uint8_t);
}

auto SkipName(std::uint8_t const* msg_ptr,
              std::uint8_t const* msg_end) -> std::uint8_t const*
{
    return SkipName(std::span<std::uint8_t const>{msg_ptr, msg_end});
}

auto SkipName(std::span<std::uint8_t const> data) -> uint8_t const*
{
    auto idx = std::size_t{0};

    while (idx + sizeof(uint16_t) <= data.size() && data[idx] != NameEnd) {
        switch (data[idx] & LabelMaskType) {
        case LabelTypeLength: {
            idx += sizeof(std::uint8_t) + data[idx];
        } break;
        case LabelTypePointer:
            return data.data() + idx + sizeof(std::uint16_t);
        default: {
            return nullptr;
        }
        }
    }

    return idx < data.size() && data[idx] == NameEnd
             ? data.data() + idx + sizeof(std::uint8_t)
             : nullptr;
}

auto UnpackName(uint8_t const* msg,
                uint8_t const* msg_ptr,
                uint8_t const* msg_end,
                uint8_t*       name,
                uint8_t*       name_end) -> uint8_t*
{
    auto const* msg_min = msg_ptr;

    if (name_end - name > NameMaxSize) {
        name_end = name + NameMaxSize;
    }

    while (static_cast<int>(sizeof(uint16_t)) <= msg_end - msg_ptr
           && *msg_ptr != NameEnd) {
        switch (*msg_ptr & LabelMaskType) {
        case LabelTypeLength: {
            auto bytes_need = sizeof(std::uint8_t) + *msg_ptr;
            if (std::min(msg_end - msg_ptr, name_end - name)
                < static_cast<std::ptrdiff_t>(bytes_need)) {
                return nullptr;
            }
            name = std::copy(msg_ptr, msg_ptr + bytes_need, name);
            msg_ptr += bytes_need;
        } break;
        case LabelTypePointer: {
            auto msg_idx = ReadNamePointer(msg_ptr);
            if (msg_min - msg <= msg_idx) {
                return nullptr;  // avoid loops or forward pointers
            }
            msg_ptr = msg + msg_idx;
            msg_min = msg_ptr;
        } break;
        default: {
            return nullptr;
        }
        }
    }

    return *msg_ptr == NameEnd
             ? std::copy(msg_ptr, msg_ptr + sizeof(std::uint8_t), name)
             : nullptr;
}

auto UnpackName(std::span<std::uint8_t const> msg,
                std::size_t                   msg_idx,
                std::span<std::uint8_t>       name) -> uint8_t*
{
    return UnpackName(msg.data(),
                      msg.data() + msg_idx,
                      msg.data() + msg.size(),
                      name.data(),
                      name.data() + name.size());
}

auto DotsToName(std::string const& name_dots) -> NameOwner
{
    NameOwner name;
    name.reserve(name.size() + 2);

    auto idx = std::size_t{0};
    auto prv = std::size_t{0};

    while (idx <= name_dots.size()) {
        if (name_dots[idx] == '.' || idx == name_dots.size()) {
            if (idx - prv > LabelMaxLength) {
                return NameOwner{};
            }
            name.push_back(idx - prv);
            std::copy(name_dots.data() + prv,
                      name_dots.data() + idx,
                      std::back_inserter(name));
            idx++;
            prv = idx;
        } else {
            // TODO: check name_dots[i]
            idx++;
        }
    }

    name.push_back(NameEnd);

    return name;
}

auto NameToDots(NameView const& name) -> std::string
{
    std::string name_dots;
    name_dots.reserve(name.size());

    auto const* name_ptr = name.data();
    auto const* name_end = name.data() + name.size();

    while (name_ptr != name_end) {
        std::copy(name_ptr + sizeof(std::uint8_t),
                  name_ptr + sizeof(std::uint8_t) + *name_ptr,
                  std::back_inserter(name_dots));
        name_dots += '.';
        name_ptr += sizeof(std::uint8_t) + *name_ptr;
    }

    while (!name_dots.empty() && name_dots.back() == '.') {
        name_dots.pop_back();
    }

    return name_dots;
}
}  // namespace DNS
