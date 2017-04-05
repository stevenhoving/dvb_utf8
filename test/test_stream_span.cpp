#include "dvb_utf8.hpp"
#include "helpers.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

using namespace dvb_utf8;

TEST(test_stream_span, test_constructing_vector)
{
    std::vector<uint8_t> data{ 1, 2, 3, 4 };
    auto stream = stream_span(data);
    EXPECT_FALSE(stream.empty());
}

TEST(test_stream_span, test_eos)
{
    std::vector<uint8_t> data{ 1, 2, 3, 4 };
    auto stream = stream_span(data);
    auto temp = stream.read<uint32_t>();
    EXPECT_TRUE(stream.eos());
}

TEST(test_stream_span, test_subspan)
{
    std::vector<uint8_t> data{ 1, 2, 3, 4 };
    auto stream = stream_span(data);
    auto temp = stream.read_buffer(4);
    EXPECT_TRUE(stream.eos());
    EXPECT_FALSE(temp.eos());
}
