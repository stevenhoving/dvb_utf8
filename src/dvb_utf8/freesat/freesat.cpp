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

#define HUFFMAN_TABLE_SIZE 256

static std::array<const hufftab *, HUFFMAN_TABLE_SIZE> tables[2];
static std::array<int, HUFFMAN_TABLE_SIZE> table_size[2];

void freesat_table_init()
{
    int this_table = -1;
    unsigned char this_char = 0xff;
    static bool runonce = true;

    // Initialization to be done once only
    if (!runonce)
        return;
    runonce = false;

    for (int i = 0; i < HUFFMAN_TABLE_SIZE; ++i) {
        tables[0][i] = nullptr;
        tables[1][i] = nullptr;
        table_size[0][i] = 0;
        table_size[1][i] = 0;
    }

    for (int i = 0; i < MEMTABLE_SIZE; ++i) {
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

/** \brief Decode an EPG string as necessary
 *
 *  \param[in] stream Possibly encoded string
 *
 *  \retval NULL - Can't decode
 *  \return A decoded string
 */
std::string freesat_huffman_decode(const dvb_utf8::stream_span &stream)
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
    unsigned int value = 0, byte = 0, bit = 0;
    int p = 0;
    int lastch = START;

    // 'stream' the first 4 bytes into a 32 bit int
    while (byte < 4 && byte < size) {
        value |= src[byte] << ((3 - byte) * 8);
        byte++;
    }

    freesat_table_init();   /**< Load the tables as necessary */

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
                unsigned int mask = 0, maskbit = 0x80000000;
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
            DVB_DBG("Missing table %d entry: <%s>\n", tableid + 1, uncompressed.c_str());
            // Entry missing in table.

            // \todo error situation, is skipping over the bytes we have parsed the correct thing todo?
            stream.seek(byte, SEEK_CUR);
            return uncompressed;
        }
    } while (lastch != STOP && value != 0);

    stream.seek(byte, SEEK_CUR);
    return uncompressed;
}
