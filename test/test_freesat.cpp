#include "dvb_utf8.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

using namespace dvb_utf8;

/* \note the freesat huffman decoding is incomplete and unstable at best */
TEST(test_freesat, test_freesat_decode)
{
    std::vector<uint8_t> data = { 0x01, 0xaa, 0xbb, 0xc0};

    auto decoded_text = freesat::decode(
        stream_span(&data[0], &data[data.size()]));

    EXPECT_EQ(decoded_text, "Naton");
}
