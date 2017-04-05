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
        stream_span(&data[0], &data.data()[data.size()]));

    EXPECT_EQ(decoded_text, "Naton");
}



TEST(test_freesat, test_freesat_decode_tv_grab_dvb_plus_data)
{
    for (auto &data : test_data)
    {
        auto stream = stream_span(&data[0], &data.data()[data.size()]);
        uint8_t tableid = stream.read<uint8_t>();
        uint8_t data_len = stream.read<uint8_t>();

        auto decoded_text = dvb_utf8::decode(stream);
        printf("%s\n", decoded_text.c_str());

        //EXPECT_EQ(decoded_text, "Naton");
        EXPECT_FALSE(decoded_text.empty());
    }
}
