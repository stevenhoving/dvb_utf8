#include "dvb_utf8.hpp"
#include "helpers.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

using namespace dvb_utf8;

TEST(test_iso6937, test_iso6937_decode)
{
    auto data = std::vector<uint8_t>{
        'h', 'e', 'n', 'k'
    };

    auto decoded_text = dvb_utf8::decode(stream_span(&data[0], &data[data.size()]));

    EXPECT_EQ(decoded_text, "henk");
}

TEST(test_iso6937, test_iso6937_euro_sign_decode)
{
    auto data = std::vector<uint8_t>{
        'h', 'e', 'n', 'k', 0xA4
    };

    auto decoded_text = dvb_utf8::decode(stream_span(&data[0], &data[data.size()]));

    auto utf8_data = to_utf8_string({
        'h', 'e', 'n', 'k', 0xE2, 0x82, 0xac
    });

    EXPECT_THAT(utf8_data, ::testing::ContainerEq(decoded_text));
}

// test, the iso6937 2 byte composed character
TEST(test_iso6937, test_iso6937_2_byte_composed_decode)
{
    auto data = std::vector<uint8_t>{
        'h', 'e', 'n', 'k', 0xc1, 0x41, 0xC2, 0x4E
    };

    auto decoded_text = dvb_utf8::decode(stream_span(&data[0], &data[data.size()]));

    auto utf8_data = to_utf8_string({
        'h', 'e', 'n', 'k', 0xC3, 0x80, 0xC5, 0x83
    });

    EXPECT_THAT(utf8_data, ::testing::ContainerEq(decoded_text));
}
