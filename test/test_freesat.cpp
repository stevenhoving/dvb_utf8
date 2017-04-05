#include "dvb_utf8.hpp"
#include "test_freesat_test_data.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

using namespace dvb_utf8;

/* \note the freesat huffman decoding is incomplete and unstable at best */
TEST(test_freesat, test_freesat_decode)
{
    std::vector<uint8_t> data = { 0x01, 0xaa, 0xbb, 0xc0};

    auto decoded_text = freesat::decode(
        stream_span(data));

    EXPECT_EQ(decoded_text, "Naton");
}

TEST(test_freesat, test_freesat_decode_tv_grab_dvb_plus_data)
{
    for (auto &data : test_data)
    {
        auto stream = stream_span(data);
        uint8_t tableid = stream.read<uint8_t>();
        uint8_t data_len = stream.read<uint8_t>();

        auto decoded_text = dvb_utf8::decode(stream);
        //printf("%s\n", decoded_text.c_str());

        EXPECT_FALSE(decoded_text.empty());
    }
}

#if DVB_UTF8_ENABLE_ENCODE
TEST(test_freesat, test_freesat_encode)
{
    std::string input = "The Country Channel";
    std::vector<uint8_t> expected{ 0x01, 0x05, 0x0b, 0xfe, 0x86, 0x4d, 0x99, 0xed, 0x08 };

    auto encoded_data = freesat::encode(input, 1);
    auto expected_data = dvb_utf8::stream_buffer(expected);
    auto decoded_data = freesat::decode(dvb_utf8::stream_span(encoded_data.data(), expected.size()));


    EXPECT_EQ(expected_data.data_, encoded_data.data_);
    EXPECT_EQ(decoded_data, input);
}
#endif // DVB_UTF8_ENABLE_ENCODE
