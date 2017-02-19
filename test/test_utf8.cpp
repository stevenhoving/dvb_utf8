#include "dvb_utf8.h"
#include "helpers.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

using namespace dvb_utf8;

TEST(test_utf8, test_utf8_decode)
{
    // dvb data stream with encoding identifier token for utf8 (0x15).
    std::vector<uint8_t> data = { 0x15,
        'h', 'e', 'n', 'k'
    };

    auto decoded_text = dvb_utf8::decode(stream_buffer(data));

    EXPECT_EQ(decoded_text, "henk");
}

TEST(test_utf8, test_utf8_non_latin_decode)
{
    // dvb data stream with encoding identifier token for utf8 (0x15).
    // \note this is a greek snippet from the libiconv tests
    auto data = std::vector<uint8_t>{ 0x15,
        0xEF, 0xBB, 0xBF, 0xCE, 0x91, 0xCE, 0x92, 0xCE, 0x93, 0xCE, 0x94, 0xCE,
        0x95, 0xCE, 0x96, 0xCE, 0x97, 0xCE, 0x98, 0xCE, 0x99, 0xCE, 0x9A, 0xCE,
        0x9B, 0xCE, 0x9C, 0xCE, 0x9D, 0xCE, 0x9E, 0xCE, 0x9F, 0xCE, 0xA0, 0xCE,
        0xA1, 0xCE, 0xA3, 0xCE, 0xA4, 0xCE, 0xA5, 0xCE, 0xA6, 0xCE, 0xA7, 0xCE,
        0xA8, 0xCE, 0xA9
    };

    auto decoded_text = dvb_utf8::decode(stream_buffer(data));

    // \note because utf8 is 'pass though all we need todo is remove the first
    // byte (this byte is the encoding identifier token).
    auto data_utf8 = std::string(data.begin(), data.end());
    data_utf8.erase(data_utf8.begin());

    EXPECT_THAT(data_utf8, ::testing::ContainerEq(decoded_text));
}