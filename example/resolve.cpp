#include <linux/socket.h>

#include <arpa/inet.h>

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>

//
#include <dns/decode.hpp>
#include <dns/dns.hpp>
#include <dns/encode.hpp>

void HexDump(std::ostream& output, const std::span<const std::uint8_t>& data) {
    static constexpr std::size_t BytesPerLine = 8;

    for (std::size_t i = 0; i < data.size(); i++) {
        if (i != 0 && i % BytesPerLine == 0) {
            output << "\n";
        }

        output << std::hex                             //
               << std::setw(2)                         //
               << std::setfill('0')                    //
               << static_cast<std::uint16_t>(data[i])  //
               << " ";                                 //
    }
}

void Print(const DNS::Question& question) {
    std::cout << "name:          " << DNS::NameToDots(DNS::ToNameView(question.name)) << "\n";
    std::cout << "type:          " << std::dec << question.type << "\n";
    std::cout << "class:         " << std::dec << question.class_ << "\n";
}

void Print(const std::span<const std::uint8_t>& message_data, const DNS::Record& record) {
    std::cout << "name:\n";
    HexDump(std::cout, DNS::ToNameView(record.name));
    std::cout << "\n";
    std::array<std::uint8_t, DNS::NameMaxSize> unpacked_name = {};

    const auto* unpacked_name_end =                                               //
      DNS::UnpackName(message_data,                                               //
                      DNS::ToNameView(record.name).data() - message_data.data(),  //
                      unpacked_name);
    std::cout << "name unpacked: "                                                         //
              << DNS::NameToDots(DNS::NameView {unpacked_name.data(), unpacked_name_end})  //
              << "\n";
    std::cout << "type:          " << std::dec << record.type << "\n";
    std::cout << "class:         " << std::dec << record.class_ << "\n";
    std::cout << "data:\n";
    HexDump(std::cout, DNS::ToRecordDataView(record.data));
    std::cout << "\n";

    if (record.type == DNS::TypeCNAME) {
        const auto* unpacked_name_end =                                                     //
          DNS::UnpackName(message_data,                                                     //
                          DNS::ToRecordDataView(record.data).data() - message_data.data(),  //
                          unpacked_name);

        std::cout << "cname unpacked: "                                                        //
                  << DNS::NameToDots(DNS::NameView {unpacked_name.data(), unpacked_name_end})  //
                  << "\n";
    }
}

void Print(const std::span<const std::uint8_t>& message_data, const DNS::Message& message) {
    std::cout << "id:            " << std::dec << message.id << "\n";
    std::cout << "flags:         " << std::hex << message.flags << "\n";

    std::cout << "question:\n";
    Print(message.question);

    std::cout << "answers:       " << std::dec << message.answers.size() << "\n";
    for (const auto& record : message.answers) {
        Print(message_data, record);
    }

    std::cout << "authority:     " << std::dec << message.authority.size() << "\n";
    for (const auto& record : message.authority) {
        Print(message_data, record);
    }

    std::cout << "additional:    " << std::dec << message.additional.size() << "\n";
    for (const auto& record : message.additional) {
        Print(message_data, record);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " DOMAIN_NAME\n";
        std::quick_exit(EXIT_SUCCESS);
    }

    constexpr auto DnsServerIpv4 = uint32_t {0x08080808};
    constexpr auto DnsServerPort = uint32_t {53};

    sockaddr_in server {};
    server.sin_family = AF_INET;
    server.sin_addr   = {ntohl(DnsServerIpv4)};
    server.sin_port   = ntohs(std::uint16_t {DnsServerPort});

    auto socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd == -1) {
        std::cout << "failed to create socket\n";
        std::quick_exit(EXIT_FAILURE);
    }

    if (connect(socket_fd, reinterpret_cast<const sockaddr*>(&server), sizeof(server)) != 0) {  // NOLINT
        std::cout << "failed to connect to server\n";
        std::quick_exit(EXIT_FAILURE);
    }

    std::array<std::uint8_t, DNS::UpdMessageMaxSize> message = {};

    {
        // clang-format off
        auto query = DNS::Message {
            0x0001,                          
            DNS::FlagsMaskRecursionDesired, 
            DNS::Question {
                DNS::DotsToName(argv[1]),
                DNS::TypeA,                         
                DNS::ClassIN                        
            }                                     
        };
        // clang-format on

        const auto* query_end = DNS::Encode(message.data(), query);
        const auto query_data = std::span<const std::uint8_t>(message.data(), query_end);

        std::cout << "query (" << query_data.size() << "):\n";
        HexDump(std::cout, query_data);
        std::cout << "\n";

        const auto bytes_sent = send(socket_fd, query_data.data(), query_data.size(), 0);

        if (bytes_sent == -1) {
            std::cout << "failed to send\n";
            std::quick_exit(EXIT_SUCCESS);
        }

        std::cout << "bytes_sent: " << std::dec << bytes_sent << "\n";
    }

    {
        const auto bytes_recv = recv(socket_fd, message.data(), message.size(), 0);
        std::cout << "bytes_recv: " << std::dec << bytes_recv << "\n";

        if (bytes_recv == -1) {
            std::cout << "failed to recv\n";
            std::quick_exit(EXIT_FAILURE);
        }

        auto answer_data = std::span<const std::uint8_t>(message.data(), bytes_recv);

        std::cout << "answer:\n";
        HexDump(std::cout, answer_data);
        std::cout << "\n";

        DNS::Message answer;
        const auto* answer_end = DNS::Decode(answer_data.data(), &answer);

        Print(std::span<const std::uint8_t>(message.data(), answer_end), answer);
    }
}