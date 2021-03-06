﻿#include "dvb_utf8.hpp"
#include "helpers.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

using namespace dvb_utf8;

#if DVB_UTF8_ENABLE_ENCODE
TEST(test_gb18030, test_gb18030_encode_decode)
{
    auto test_text = std::string("hello world");
    auto encoded_data = dvb_utf8::encode(test_text, dvb_utf8::character_encoding::gb18030);
    auto decoded_text = dvb_utf8::decode(stream_span(encoded_data.data(), encoded_data.size()));

    EXPECT_THAT(test_text, ::testing::ContainerEq(decoded_text));
}
#endif // DVB_UTF8_ENABLE_ENCODE

TEST(test_gb18030, test_gb18030_decode)
{
    // source: https://github.com/scheib/chromium/blob/master/chrome/test/data/encoding_tests/alias_mapping/gb18030.html
    auto data = std::vector<uint8_t>{ 0x13,
        0x4E, 0x42, 0x41, 0xB9, 0xD9, 0xB7, 0xBD, 0xBD, 0xF1, 0xC8, 0xD5, 0xB9, 0xAB, 0xB2, 0xBC, 0xC1,
        0xCB, 0x30, 0x39, 0xC4, 0xEA, 0xC8, 0xAB, 0xC3, 0xF7, 0xD0, 0xC7, 0xC8, 0xFC, 0xC6, 0xB1, 0xD1,
        0xA1, 0xBB, 0xEE, 0xB6, 0xAF, 0xB5, 0xC4, 0xBD, 0xD7, 0xB6, 0xCE, 0xD0, 0xD4, 0xBD, 0xE1, 0xB9,
        0xFB, 0xA3, 0xAC, 0xB0, 0xC2, 0xC0, 0xBC, 0xB6, 0xE0, 0xC4, 0xA7, 0xCA, 0xF5, 0xB5, 0xC4, 0xD6,
        0xD0, 0xB7, 0xE6, 0xBB, 0xF4, 0xBB, 0xAA, 0xB5, 0xC2, 0xD2, 0xD4, 0x32, 0x2C, 0x31, 0x30, 0x32,
        0x2C, 0x33, 0x36, 0x38, 0xC6, 0xB1, 0xC8, 0xD4, 0xC8, 0xBB, 0xC1, 0xEC, 0xC5, 0xDC, 0xB6, 0xAB,
        0xCE, 0xF7, 0xB2, 0xBF, 0xA3, 0xAC, 0xB5, 0xAB, 0xC6, 0xBE, 0xBD, 0xE8, 0xB1, 0xBE, 0xC8, 0xFC,
        0xBC, 0xBE, 0xB1, 0xED, 0xCF, 0xD6, 0xB3, 0xF6, 0xC0, 0xB4, 0xB5, 0xC4, 0xC1, 0xEC, 0xD0, 0xE4,
        0xB1, 0xBE, 0xC9, 0xAB, 0xA3, 0xAC, 0xD0, 0xA1, 0xBB, 0xCA, 0xB5, 0xDB, 0xD5, 0xB2, 0xC4, 0xB7,
        0xCB, 0xB9, 0xBB, 0xF1, 0xB5, 0xC3, 0xC8, 0xCF, 0xBF, 0xC9, 0xA3, 0xAC, 0xC4, 0xBF, 0xC7, 0xB0,
        0xD7, 0xDC, 0xBC, 0xC6, 0x31, 0x2C, 0x39, 0x34, 0x30, 0x2C, 0x31, 0x36, 0x32, 0xC6, 0xB1, 0xB3,
        0xAC, 0xD4, 0xBD, 0xC1, 0xCB, 0xBF, 0xC6, 0xB1, 0xC8, 0xBE, 0xD3, 0xC1, 0xAA, 0xC3, 0xCB, 0xB5,
        0xDA, 0xB6, 0xFE, 0xA1, 0xA3, 0xCE, 0xF7, 0xB2, 0xBF, 0xB7, 0xBD, 0xC3, 0xE6, 0xA3, 0xAC, 0xD0,
        0xA1, 0xB7, 0xC9, 0xCF, 0xC0, 0xD2, 0xC0, 0xBE, 0xC9, 0xCE, 0xDE, 0xBF, 0xC9, 0xD5, 0xF9, 0xD2,
        0xE9, 0xB5, 0xC4, 0xCE, 0xFC, 0xC6, 0xB1, 0xBB, 0xFA, 0xC6, 0xF7, 0xA3, 0xAC, 0xD2, 0xD4, 0x31,
        0x2C, 0x39, 0x30, 0x33, 0x2C, 0x37, 0x39, 0x38, 0xC6, 0xB1, 0xCE, 0xC8, 0xBE, 0xD3, 0xCA, 0xD7,
        0xCE, 0xBB, 0xA3, 0xAC, 0xB6, 0xF8, 0xD2, 0xA6, 0xC3, 0xF7, 0xD4, 0xF2, 0xBB, 0xF1, 0xB5, 0xC3,
        0x31, 0x2C, 0x37, 0x35, 0x38, 0x2C, 0x34, 0x39, 0x39, 0xC6, 0xB1, 0xA3, 0xAC, 0xCA, 0xD7, 0xB7,
        0xA2, 0xD6, 0xD0, 0xB7, 0xE6, 0xD3, 0xA6, 0xCA, 0xF4, 0xC4, 0xD2, 0xD6, 0xD0, 0xD6, 0xAE, 0xCE,
        0xEF, 0xA1, 0xA3, 0xB1, 0xB8, 0xCA, 0xDC, 0xB9, 0xD8, 0xD7, 0xA2, 0xB5, 0xC4, 0xCE, 0xD2, 0xB9,
        0xFA, 0xC7, 0xF2, 0xD4, 0xB1, 0xD2, 0xD7, 0xBD, 0xA8, 0xC1, 0xAA, 0xD4, 0xDA, 0xB4, 0xCB, 0xB4,
        0xCE, 0xB9, 0xAB, 0xB2, 0xBC, 0xB5, 0xC4, 0xBD, 0xE1, 0xB9, 0xFB, 0xB5, 0xB1, 0xD6, 0xD0, 0xC5,
        0xC5, 0xC3, 0xFB, 0xB6, 0xAB, 0xB2, 0xBF, 0xC7, 0xB0, 0xB7, 0xE6, 0xB5, 0xDA, 0xC8, 0xFD, 0xCE,
        0xBB, 0xA3, 0xAC, 0xD3, 0xEB, 0xB5, 0xDA, 0xB6, 0xFE, 0xCE, 0xBB, 0xB5, 0xC4, 0xBC, 0xD3, 0xC4,
        0xDA, 0xCC, 0xD8, 0xC8, 0xD4, 0xC8, 0xBB, 0xB1, 0xA3, 0xB3, 0xD6, 0x31, 0x36, 0xCD, 0xF2, 0xC6,
        0xB1, 0xB5, 0xC4, 0xB2, 0xEE, 0xBE, 0xE0, 0xA3, 0xAC, 0xBF, 0xBC, 0xC2, 0xC7, 0xB5, 0xBD, 0xC2,
        0xCC, 0xC9, 0xC0, 0xBE, 0xFC, 0xD7, 0xEE, 0xBD, 0xFC, 0xC8, 0xC3, 0xC8, 0xCB, 0xB4, 0xF3, 0xB5,
        0xF8, 0xD1, 0xDB, 0xBE, 0xB5, 0xB5, 0xC4, 0xD5, 0xBD, 0xBC, 0xA8, 0xD2, 0xD4, 0xBC, 0xB0, 0xB0,
        0xA2, 0xC1, 0xAA, 0xB5, 0xC4, 0xB1, 0xAC, 0xB7, 0xA2, 0xC7, 0xF7, 0xCA, 0xC6, 0xA3, 0xAC, 0xBA,
        0xF3, 0xD5, 0xDF, 0xD4, 0xDA, 0xD7, 0xEE, 0xBA, 0xF3, 0x31, 0x30, 0xCC, 0xEC, 0xB5, 0xC4, 0xD4,
        0xDA, 0xCF, 0xDF, 0xCD, 0xB6, 0xC6, 0xB1, 0xBB, 0xEE, 0xB6, 0xAF, 0xB5, 0xB1, 0xD6, 0xD0, 0xC8,
        0xD4, 0xD3, 0xD0, 0xCF, 0xA3, 0xCD, 0xFB, 0xD3, 0xAD, 0xCD, 0xB7, 0xB8, 0xCF, 0xC9, 0xCF, 0xA1,
        0xA3,
        //0x0A, 0x20
    };

    auto decoded_text = dvb_utf8::decode(stream_span(data));

    auto utf8_data = to_utf8_string({
        0x4E, 0x42, 0x41, 0xE5, 0xAE, 0x98, 0xE6, 0x96, 0xB9, 0xE4, 0xBB, 0x8A, 0xE6, 0x97, 0xA5, 0xE5,
        0x85, 0xAC, 0xE5, 0xB8, 0x83, 0xE4, 0xBA, 0x86, 0x30, 0x39, 0xE5, 0xB9, 0xB4, 0xE5, 0x85, 0xA8,
        0xE6, 0x98, 0x8E, 0xE6, 0x98, 0x9F, 0xE8, 0xB5, 0x9B, 0xE7, 0xA5, 0xA8, 0xE9, 0x80, 0x89, 0xE6,
        0xB4, 0xBB, 0xE5, 0x8A, 0xA8, 0xE7, 0x9A, 0x84, 0xE9, 0x98, 0xB6, 0xE6, 0xAE, 0xB5, 0xE6, 0x80,
        0xA7, 0xE7, 0xBB, 0x93, 0xE6, 0x9E, 0x9C, 0xEF, 0xBC, 0x8C, 0xE5, 0xA5, 0xA5, 0xE5, 0x85, 0xB0,
        0xE5, 0xA4, 0x9A, 0xE9, 0xAD, 0x94, 0xE6, 0x9C, 0xAF, 0xE7, 0x9A, 0x84, 0xE4, 0xB8, 0xAD, 0xE9,
        0x94, 0x8B, 0xE9, 0x9C, 0x8D, 0xE5, 0x8D, 0x8E, 0xE5, 0xBE, 0xB7, 0xE4, 0xBB, 0xA5, 0x32, 0x2C,
        0x31, 0x30, 0x32, 0x2C, 0x33, 0x36, 0x38, 0xE7, 0xA5, 0xA8, 0xE4, 0xBB, 0x8D, 0xE7, 0x84, 0xB6,
        0xE9, 0xA2, 0x86, 0xE8, 0xB7, 0x91, 0xE4, 0xB8, 0x9C, 0xE8, 0xA5, 0xBF, 0xE9, 0x83, 0xA8, 0xEF,
        0xBC, 0x8C, 0xE4, 0xBD, 0x86, 0xE5, 0x87, 0xAD, 0xE5, 0x80, 0x9F, 0xE6, 0x9C, 0xAC, 0xE8, 0xB5,
        0x9B, 0xE5, 0xAD, 0xA3, 0xE8, 0xA1, 0xA8, 0xE7, 0x8E, 0xB0, 0xE5, 0x87, 0xBA, 0xE6, 0x9D, 0xA5,
        0xE7, 0x9A, 0x84, 0xE9, 0xA2, 0x86, 0xE8, 0xA2, 0x96, 0xE6, 0x9C, 0xAC, 0xE8, 0x89, 0xB2, 0xEF,
        0xBC, 0x8C, 0xE5, 0xB0, 0x8F, 0xE7, 0x9A, 0x87, 0xE5, 0xB8, 0x9D, 0xE8, 0xA9, 0xB9, 0xE5, 0xA7,
        0x86, 0xE6, 0x96, 0xAF, 0xE8, 0x8E, 0xB7, 0xE5, 0xBE, 0x97, 0xE8, 0xAE, 0xA4, 0xE5, 0x8F, 0xAF,
        0xEF, 0xBC, 0x8C, 0xE7, 0x9B, 0xAE, 0xE5, 0x89, 0x8D, 0xE6, 0x80, 0xBB, 0xE8, 0xAE, 0xA1, 0x31,
        0x2C, 0x39, 0x34, 0x30, 0x2C, 0x31, 0x36, 0x32, 0xE7, 0xA5, 0xA8, 0xE8, 0xB6, 0x85, 0xE8, 0xB6,
        0x8A, 0xE4, 0xBA, 0x86, 0xE7, 0xA7, 0x91, 0xE6, 0xAF, 0x94, 0xE5, 0xB1, 0x85, 0xE8, 0x81, 0x94,
        0xE7, 0x9B, 0x9F, 0xE7, 0xAC, 0xAC, 0xE4, 0xBA, 0x8C, 0xE3, 0x80, 0x82, 0xE8, 0xA5, 0xBF, 0xE9,
        0x83, 0xA8, 0xE6, 0x96, 0xB9, 0xE9, 0x9D, 0xA2, 0xEF, 0xBC, 0x8C, 0xE5, 0xB0, 0x8F, 0xE9, 0xA3,
        0x9E, 0xE4, 0xBE, 0xA0, 0xE4, 0xBE, 0x9D, 0xE6, 0x97, 0xA7, 0xE6, 0x97, 0xA0, 0xE5, 0x8F, 0xAF,
        0xE4, 0xBA, 0x89, 0xE8, 0xAE, 0xAE, 0xE7, 0x9A, 0x84, 0xE5, 0x90, 0xB8, 0xE7, 0xA5, 0xA8, 0xE6,
        0x9C, 0xBA, 0xE5, 0x99, 0xA8, 0xEF, 0xBC, 0x8C, 0xE4, 0xBB, 0xA5, 0x31, 0x2C, 0x39, 0x30, 0x33,
        0x2C, 0x37, 0x39, 0x38, 0xE7, 0xA5, 0xA8, 0xE7, 0xA8, 0xB3, 0xE5, 0xB1, 0x85, 0xE9, 0xA6, 0x96,
        0xE4, 0xBD, 0x8D, 0xEF, 0xBC, 0x8C, 0xE8, 0x80, 0x8C, 0xE5, 0xA7, 0x9A, 0xE6, 0x98, 0x8E, 0xE5,
        0x88, 0x99, 0xE8, 0x8E, 0xB7, 0xE5, 0xBE, 0x97, 0x31, 0x2C, 0x37, 0x35, 0x38, 0x2C, 0x34, 0x39,
        0x39, 0xE7, 0xA5, 0xA8, 0xEF, 0xBC, 0x8C, 0xE9, 0xA6, 0x96, 0xE5, 0x8F, 0x91, 0xE4, 0xB8, 0xAD,
        0xE9, 0x94, 0x8B, 0xE5, 0xBA, 0x94, 0xE5, 0xB1, 0x9E, 0xE5, 0x9B, 0x8A, 0xE4, 0xB8, 0xAD, 0xE4,
        0xB9, 0x8B, 0xE7, 0x89, 0xA9, 0xE3, 0x80, 0x82, 0xE5, 0xA4, 0x87, 0xE5, 0x8F, 0x97, 0xE5, 0x85,
        0xB3, 0xE6, 0xB3, 0xA8, 0xE7, 0x9A, 0x84, 0xE6, 0x88, 0x91, 0xE5, 0x9B, 0xBD, 0xE7, 0x90, 0x83,
        0xE5, 0x91, 0x98, 0xE6, 0x98, 0x93, 0xE5, 0xBB, 0xBA, 0xE8, 0x81, 0x94, 0xE5, 0x9C, 0xA8, 0xE6,
        0xAD, 0xA4, 0xE6, 0xAC, 0xA1, 0xE5, 0x85, 0xAC, 0xE5, 0xB8, 0x83, 0xE7, 0x9A, 0x84, 0xE7, 0xBB,
        0x93, 0xE6, 0x9E, 0x9C, 0xE5, 0xBD, 0x93, 0xE4, 0xB8, 0xAD, 0xE6, 0x8E, 0x92, 0xE5, 0x90, 0x8D,
        0xE4, 0xB8, 0x9C, 0xE9, 0x83, 0xA8, 0xE5, 0x89, 0x8D, 0xE9, 0x94, 0x8B, 0xE7, 0xAC, 0xAC, 0xE4,
        0xB8, 0x89, 0xE4, 0xBD, 0x8D, 0xEF, 0xBC, 0x8C, 0xE4, 0xB8, 0x8E, 0xE7, 0xAC, 0xAC, 0xE4, 0xBA,
        0x8C, 0xE4, 0xBD, 0x8D, 0xE7, 0x9A, 0x84, 0xE5, 0x8A, 0xA0, 0xE5, 0x86, 0x85, 0xE7, 0x89, 0xB9,
        0xE4, 0xBB, 0x8D, 0xE7, 0x84, 0xB6, 0xE4, 0xBF, 0x9D, 0xE6, 0x8C, 0x81, 0x31, 0x36, 0xE4, 0xB8,
        0x87, 0xE7, 0xA5, 0xA8, 0xE7, 0x9A, 0x84, 0xE5, 0xB7, 0xAE, 0xE8, 0xB7, 0x9D, 0xEF, 0xBC, 0x8C,
        0xE8, 0x80, 0x83, 0xE8, 0x99, 0x91, 0xE5, 0x88, 0xB0, 0xE7, 0xBB, 0xBF, 0xE8, 0xA1, 0xAB, 0xE5,
        0x86, 0x9B, 0xE6, 0x9C, 0x80, 0xE8, 0xBF, 0x91, 0xE8, 0xAE, 0xA9, 0xE4, 0xBA, 0xBA, 0xE5, 0xA4,
        0xA7, 0xE8, 0xB7, 0x8C, 0xE7, 0x9C, 0xBC, 0xE9, 0x95, 0x9C, 0xE7, 0x9A, 0x84, 0xE6, 0x88, 0x98,
        0xE7, 0xBB, 0xA9, 0xE4, 0xBB, 0xA5, 0xE5, 0x8F, 0x8A, 0xE9, 0x98, 0xBF, 0xE8, 0x81, 0x94, 0xE7,
        0x9A, 0x84, 0xE7, 0x88, 0x86, 0xE5, 0x8F, 0x91, 0xE8, 0xB6, 0x8B, 0xE5, 0x8A, 0xBF, 0xEF, 0xBC,
        0x8C, 0xE5, 0x90, 0x8E, 0xE8, 0x80, 0x85, 0xE5, 0x9C, 0xA8, 0xE6, 0x9C, 0x80, 0xE5, 0x90, 0x8E,
        0x31, 0x30, 0xE5, 0xA4, 0xA9, 0xE7, 0x9A, 0x84, 0xE5, 0x9C, 0xA8, 0xE7, 0xBA, 0xBF, 0xE6, 0x8A,
        0x95, 0xE7, 0xA5, 0xA8, 0xE6, 0xB4, 0xBB, 0xE5, 0x8A, 0xA8, 0xE5, 0xBD, 0x93, 0xE4, 0xB8, 0xAD,
        0xE4, 0xBB, 0x8D, 0xE6, 0x9C, 0x89, 0xE5, 0xB8, 0x8C, 0xE6, 0x9C, 0x9B, 0xE8, 0xBF, 0x8E, 0xE5,
        0xA4, 0xB4, 0xE8, 0xB5, 0xB6, 0xE4, 0xB8, 0x8A, 0xE3, 0x80, 0x82
    });

    EXPECT_THAT(utf8_data, ::testing::ContainerEq(decoded_text));
}
