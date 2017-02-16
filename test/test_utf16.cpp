#include "dvb_utf8.h"
#include "helpers.h"
#include <gtest/gtest.h>
#include <string>

using namespace dvb_utf8;

TEST(test_utf16, test_utf16be_decode)
{
    // dvb data stream with encoding identifier token for utf16be (0x16).
    auto data = std::vector<uint8_t>{ 0x16,
        0xD8, 0x08, 0xDF, 0x45, 0x00, 0x3D, 0x00, 0x52, 0x00, 0x61
    };

    auto decoded_text = dvb_utf8::decode(stream_buffer(data));

    auto data_utf8 = to_utf8_string({
        0xF0, 0x92, 0x8D, 0x85, 0x3D, 0x52, 0x61
    });

    EXPECT_EQ(decoded_text, data_utf8);
}

TEST(test_utf16, test_utf16le_decode)
{
    // dvb data stream with encoding identifier token for utf16le (0x17).
    auto data = std::vector<uint8_t>{ 0x17,
        0x08, 0xD8, 0x45, 0xDF, 0x3D, 0x00, 0x52, 0x00, 0x61, 0x00
    };

    auto decoded_text = dvb_utf8::decode(stream_buffer(data));

    auto data_utf8 = to_utf8_string({
        0xF0, 0x92, 0x8D, 0x85, 0x3D, 0x52, 0x61
    });

    EXPECT_EQ(decoded_text, data_utf8);
}
