/* FreeSat Huffman decoder for VDR
 *
 * Insert GPL license
 */

#include "freesat.hpp"
#include <cstdio>
#include <string>
#include <array>

struct hufftab {
    char from;
    unsigned int value;
    short bits;
    char next;
};

#include "huffman.t.hpp"

constexpr char START = '\0';
constexpr char STOP = '\0';
constexpr char ESCAPE = '\1';

constexpr int HUFFMAN_TABLE_SIZE = 256;
static std::array<const hufftab *, HUFFMAN_TABLE_SIZE> tables[2]{ {},{} };
static std::array<int, HUFFMAN_TABLE_SIZE> table_size[2]{ {},{} };

void freesat_table_init()
{
    static bool runonce = true;
    // Initialization to be done once only
    if (!runonce)
        return;
    runonce = false;

    int this_table = -1;
    unsigned char this_char = 0xff;
    for (size_t i = 0; i < memtable.size(); ++i) {
        if (memtable[i].from != this_char) {
            this_char = memtable[i].from;
            if (!this_char) { // Jumping to next table
                this_table++;
            }
            tables[this_table][this_char] = &memtable[i];
        }
        table_size[this_table][this_char]++;
    }
}
namespace dvb_utf8
{
namespace freesat
{

/** \brief Decode an EPG string as necessary
 *
 *  \param[in] stream Possibly encoded string
 *
 *  \retval NULL - Can't decode
 *  \return A decoded string
 */
std::string decode(const dvb_utf8::stream_span &stream)
{
    std::string uncompressed;
    int uncompressed_len = 30;

    uint8_t tableid = stream.read<uint8_t>();
    if (tableid != 1 && tableid != 2) {
        printf("error, invalid freesat huffman tableid\n");
        return "";
    }

    tableid -= 1;
    auto size = stream.size() - stream.tell();
    auto src = &stream.data()[stream.tell()];

    uncompressed.resize(uncompressed_len + 1);
    unsigned int value = 0, bit = 0;
    int p = 0, byte = 0;

    // 'stream' the first 4 bytes into a 32 bit int
    while (byte < 4u && byte < size) {
        value |= src[byte] << ((3 - byte) * 8);
        byte++;
    }

    freesat_table_init();   /**< Load the tables as necessary */

    int lastch = START;
    do {
        int found = 0;
        unsigned int bitShift = 0;
        if (lastch == ESCAPE) {
            const char nextCh = (value >> 24) & 0xff;
            found = 1;
            // Encoded in the next 8 bits.
            // Terminated by the first ASCII character.
            bitShift = 8;
            if ((nextCh & 0x80) == 0)
                lastch = nextCh;
            if (p >= uncompressed_len) {
                uncompressed_len += 10;
                uncompressed.resize(uncompressed_len + 1);
            }
            uncompressed[p++] = nextCh;
            uncompressed[p] = 0;
        }
        else
        {
            int j;
            for (j = 0; j < table_size[tableid][lastch]; j++) {
                unsigned int mask = 0u, maskbit = 0x80000000;
                for (short kk = 0; kk < tables[tableid][lastch][j].bits; kk++) {
                    mask |= maskbit;
                    maskbit >>= 1;
                }
                if ((value & mask) == tables[tableid][lastch][j].value) {
                    const char nextCh = tables[tableid][lastch][j].next;
                    bitShift = tables[tableid][lastch][j].bits;
                    if (nextCh != STOP && nextCh != ESCAPE) {
                        if (p >= uncompressed_len) {
                            uncompressed_len += 10;
                            uncompressed.resize(uncompressed_len + 1);
                        }
                        uncompressed[p++] = nextCh;
                        uncompressed[p] = 0;
                    }
                    found = 1;
                    lastch = nextCh;
                    break;
                }
            }
        }
        if (found) {
            // Shift up by the number of bits.
            for (unsigned int b = 0; b < bitShift; b++)
            {
                value = (value << 1) & 0xfffffffe;
                if (byte < size)
                    value |= (src[byte] >> (7 - bit)) & 1;
                if (bit == 7)
                {
                    bit = 0;
                    byte++;
                }
                else
                {
                    bit++;
                }
            }
        }
        else
        {
            printf("Missing table %d entry: <%s>\n", tableid + 1, uncompressed.c_str());
            // Entry missing in table.

            stream.seek(0, SEEK_END);
            return uncompressed;
        }
    } while (lastch != STOP && value != 0);


    stream.seek(size, SEEK_CUR);
    uncompressed.resize(p);
    return uncompressed;
}

// bitstreamnode
struct bitnode
{
    uint32_t value;
    int bits;
    std::vector<uint8_t> escape;
};

hufftab find_node(char ch, int tableid, char lastch)
{
    // if we have to escape this character find the first escape node
    if (ch < 0)
        ch = ESCAPE;

    for (int i = 0; i < table_size[tableid][lastch]; ++i)
    {
        auto &node = tables[tableid][lastch][i];
        if (node.next == ch)
            return node;
    }
    __debugbreak();
}

#if DVB_UTF8_ENABLE_ENCODE
dvb_utf8::stream_buffer encode(const std::string &text, const int tableid /*= 1 */)
{
    std::vector<bitnode> bitstream;

    // when not found and not escaping yet, enable escaping the bytes.
    char lastch = 0;
    dvb_utf8::stream_buffer result;

    freesat_table_init();   /**< Load the tables if necessary */

    result.write(static_cast<uint8_t>(tableid + 1));
    for (size_t i = 0; i < text.size(); ++i)
    {
        auto currch = text[i];

        auto node = find_node(currch, tableid, lastch);
        bitnode bit;
        bit.value = node.value;
        bit.bits = node.bits;

        if (currch < 0)
        {
            for (; text[i] < 0 && i < text.size(); ++i)
                bit.escape.emplace_back(text[i]);

            // the first ascii character will break the escape sequence
            if (i < text.size())
            {
                // if the escaped sequence did not start on the first char
                char firstascii = text[i];
                if (lastch != 0)
                    lastch = firstascii;
                bit.escape.emplace_back(firstascii);
            }
        }
        else
            lastch = currch;
        bitstream.emplace_back(bit);
    }

    uint32_t value = 0;
    int bits = 0;
    for (auto &node : bitstream)
    {
        value |= node.value >> bits;
        bits += node.bits;
        while (bits >= 8)
        {
            uint8_t byte = (value >> 24) & 0xFF;
            value <<= 8;
            bits -= 8;

            result.write(byte);
        }
        for (size_t i = 0; i < node.escape.size(); ++i)
        {
            uint32_t temp = static_cast<uint32_t>(node.escape[i]) << 24;
            value |= temp >> bits;
            bits += 8;

            while (bits >= 8)
            {
                uint8_t byte = (value >> 24) & 0xFF;
                value <<= 8;
                bits -= 8;

                result.write(byte);
            }
        }
    }

    // serialize the left over bits
    while (bits > 0)
    {
        uint8_t byte = (value >> 24) & 0xFF;
        value <<= 8;
        bits -= 8;

        result.write(byte);
    }
    return result;
}
#endif // DVB_UTF8_ENABLE_ENCODE

} // namespace freesat
} // namespace dvb_utf8
