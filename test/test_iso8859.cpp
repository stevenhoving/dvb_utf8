#include "dvb_utf8.hpp"
#include "helpers.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

using namespace dvb_utf8;

TEST(test_iso8859, test_iso8859_1_decode)
{
    // dvb data stream with encoding identifier token for
    // iso8859-1 (0x10, 0x00, 0x01).
    auto data = std::vector<uint8_t>{ 0x10,
        0x00, 0x01, 'h', 'e', 'n', 'k'
    };

    auto decoded_text = dvb_utf8::decode(stream_span(data));

    EXPECT_EQ(decoded_text, "henk");
}

TEST(test_iso8859, test_iso8859_2_decode)
{
    // dvb data stream with encoding identifier token for
    // iso8859-2 (0x10, 0x00, 0x02).
    auto data = std::vector<uint8_t>{ 0x10,
        0x00, 0x02, 0x48, 0xEA, 0x4E, 0x4B, 0xFF
    };

    auto decoded_text = dvb_utf8::decode(stream_span(data));

    auto utf8_data = to_utf8_string({
        0x48, 0xC4, 0x99, 0x4E, 0x4B, 0xCB, 0x99
    });

    EXPECT_THAT(utf8_data, ::testing::ContainerEq(decoded_text));
}
