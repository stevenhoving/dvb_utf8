#ifndef test_data_hpp__
#define test_data_hpp__

#include <string>
#include <vector>

#include <cstdint>

std::vector<std::string> string_split(const std::string &data, const std::string needle)
{
    std::vector<std::string> result;
    size_t pos = std::string::npos;
    size_t prev_pos = 0;
    while ((pos = data.find(needle, prev_pos)) != std::string::npos)
    {
        result.emplace_back(data.substr(prev_pos, pos - prev_pos));
        prev_pos = pos + needle.size();
    }
    if (prev_pos != std::string::npos)
        result.emplace_back(data.substr(prev_pos, data.size() - prev_pos));

    return result;
}

std::vector<uint8_t> hex_line_to_data(const std::string &data)
{
    std::vector<std::string> split_result;
    auto first = string_split(data, "   ");
    if (first.size() < 2)
        abort();

    auto sections = string_split(first[0], "  ");
    auto hex_view1 = string_split(sections[2], " ");
    auto hex_view2 = string_split(sections[3], " ");

    if (hex_view1[0] == "") hex_view1.erase(hex_view1.begin());
    if (hex_view2[0] == "") hex_view2.erase(hex_view2.begin());

    static int hex2nibble[255];
    hex2nibble[static_cast<int>('0')] = 0;
    hex2nibble[static_cast<int>('1')] = 1;
    hex2nibble[static_cast<int>('2')] = 2;
    hex2nibble[static_cast<int>('3')] = 3;
    hex2nibble[static_cast<int>('4')] = 4;
    hex2nibble[static_cast<int>('5')] = 5;
    hex2nibble[static_cast<int>('6')] = 6;
    hex2nibble[static_cast<int>('7')] = 7;
    hex2nibble[static_cast<int>('8')] = 8;
    hex2nibble[static_cast<int>('9')] = 9;
    hex2nibble[static_cast<int>('a')] = hex2nibble[static_cast<int>('A')] = 0xA;
    hex2nibble[static_cast<int>('b')] = hex2nibble[static_cast<int>('B')] = 0xB;
    hex2nibble[static_cast<int>('c')] = hex2nibble[static_cast<int>('C')] = 0xC;
    hex2nibble[static_cast<int>('d')] = hex2nibble[static_cast<int>('D')] = 0xD;
    hex2nibble[static_cast<int>('e')] = hex2nibble[static_cast<int>('E')] = 0xE;
    hex2nibble[static_cast<int>('f')] = hex2nibble[static_cast<int>('F')] = 0xF;

    auto result = std::vector<uint8_t>();
    for (auto itr : hex_view1)
        result.emplace_back(hex2nibble[static_cast<int>(itr[0])] << 4 | hex2nibble[static_cast<int>(itr[1])]);

    for (auto itr : hex_view2)
        result.emplace_back(hex2nibble[static_cast<int>(itr[0])] << 4 | hex2nibble[static_cast<int>(itr[1])]);

    return result;
}

std::vector<uint8_t> hex_packet_to_data(const std::string &data)
{
    auto result = std::vector<uint8_t>();
    auto lines = string_split(data, "\n");
    for (auto &line : lines)
    {
        auto line_data = hex_line_to_data(line);
        result.insert(result.end(), line_data.begin(), line_data.end());
    }

    return result;
}

static const std::string hex_packet_pid18_1 =
R"(  0000:  4e f1 8d 52 21 d1 00 01  0c 99 00 03 01 4e 4e ed   N..R!........NN.
  0010:  e1 cb 13 50 00 02 40 00  91 72 4d 25 64 75 74 11   ...P..@..rM%dut.
  0020:  05 53 70 6f 72 7a 61 20  6f 70 20 7a 6f 6e 64 61   .Sporza op zater
  0030:  67 0f 05 28 61 66 6c 20  36 29 20 28 32 30 31 37   g..(afl 6) (2017
  0040:  29 4e ff 01 64 75 74 00  f9 05 53 70 6f 72 74 6d   )N..dut...Sportm
  0050:  61 67 61 7a 69 6e 65 2e  20 0a 4d 65 74 3a 20 42   agazine. .Met: B
  0060:  61 73 6b 65 74 62 61 6c  3a 20 6c 69 76 65 76 65   asketbal: liveve
  0070:  72 73 6c 61 67 20 76 61  6e 20 64 65 20 66 69 6e   rslag van de fin
  0080:  61 6c 65 20 76 61 6e 20  64 65 20 42 65 6b 65 72   ale van de Beker
  0090:  20 76 61 6e 20 42 65 6c  67 69 eb 2e 20 43 6f 6d    van Belgi.. Com
  00a0:  6d 65 6e 74 61 61 72 3a  20 4b 72 69 73 20 4d 65   mentaar: Henk Me
  00b0:  65 72 74 65 6e 73 20 65  6e 20 4c 6f 75 69 73 20   ertens en Loeis 
  00c0:  43 61 73 74 65 65 6c 73  2e 20 49 6e 74 65 72 76   Casteels. Interv
  00d0:  69 65 77 73 3a 20 43 68  72 69 73 74 6f 70 68 65   iews: Christophe
  00e0:  20 56 61 6e 64 65 67 6f  6f 72 2e 20 2d 20 56 65    Vandegeer. - Ve
  00f0:  6c 64 72 69 6a 64 65 6e  3a 20 73 61 6d 65 6e 76   ldrijden: samenv
  0100:  61 74 74 69 6e 67 20 76  61 6e 20 64 65 20 49 6e   atting van de In
  0110:  74 65 72 6e 61 74 69 6f  6e 61 6c 65 20 53 6c 75   ternationale Slu
  0120:  69 74 69 6e 67 73 70 72  69 6a 73 20 69 6e 20 4f   itingsprijs in O
  0130:  6f 73 74 6d 61 6c 6c 65  2e 20 43 6f 6d 6d 65 6e   ostmalle. Cimmen
  0140:  74 61 4e 2a 11 64 75 74  00 24 05 61 72 3a 20 4d   taN*.dut.$.ar: M
  0150:  69 63 68 65 6c 20 57 75  79 74 73 20 65 6e 20 50   ichal Huyts en P
  0160:  61 75 6c 20 48 65 72 79  67 65 72 73 2e 0a 55 18   aul Hirygers..U.
  0170:  4e 4c 44 00 42 45 4c 00  44 45 55 00 43 5a 45 00   NLD.BEL.DEU.CZE.
  0180:  4c 55 58 00 53 56 4b 00  54 02 40 00 72 01 54 b4   LUX.SVK.T.@.r.T.)";

static const std::string hex_packet_pid18_2 =
R"(  0000:  4f f2 86 10 1c d1 01 01  04 51 00 35 01 4f 17 1a   O........Q.5.O..
  0010:  e1 cb 15 29 00 01 53 00  32 6b 4d 12 64 75 74 0d   ...)..S.2kM.dut.
  0020:  05 42 61 6c 6c 20 6f 66  20 46 69 72 65 00 4e ff   .Ball of Fire.N.
  0030:  02 64 75 74 00 f9 05 50  72 6f 66 65 73 73 6f 72   .dut...Professor
  0040:  20 42 65 72 74 72 61 6d  20 50 6f 74 74 73 20 69    Bertram Potts i
  0050:  73 74 20 65 69 6e 20 6a  75 6e 67 65 72 20 41 6b   st ein junger Ak
  0060:  61 64 65 6d 69 6b 65 72  2c 20 64 65 72 20 61 6e   ademiker, der an
  0070:  20 65 69 6e 65 6d 20 57  f6 72 74 65 72 62 75 63    einem W.rterbuc
  0080:  68 20 66 fc 72 20 53 6c  61 6e 67 2d 41 75 73 64   h f.r Slang-Ausd
  0090:  72 fc 63 6b 65 20 61 72  62 65 69 74 65 74 2e 20   r.cke arbeitet. 
  00a0:  42 65 69 20 73 65 69 6e  65 6e 20 53 74 75 64 69   Bei seinen Studi
  00b0:  65 6e 20 68 69 6c 66 74  20 69 68 6d 20 64 69 65   en hilft ihm die
  00c0:  20 72 61 73 73 69 67 65  20 53 74 72 69 70 70 65    rassige Strippe
  00d0:  72 69 6e 20 4b 61 74 68  72 79 6e 20 22 53 75 67   rin Kathryn "Sug
  00e0:  61 72 20 50 75 73 73 22  20 4f 27 53 68 65 61 2c   ar Puss" O'Shea,
  00f0:  20 64 69 65 20 69 6d 20  48 61 75 73 20 64 65 73    die im Haus des
  0100:  20 50 72 6f 66 65 73 73  6f 72 73 20 55 6e 74 65    Professors Unte
  0110:  72 73 63 68 6c 75 70 66  20 76 6f 72 20 69 68 72   rschlupf vor ihr
  0120:  65 6d 20 47 61 6e 67 73  74 65 72 66 72 65 75 4e   em GangsterfreuN
  0130:  ff 12 64 75 74 00 f9 05  6e 64 20 66 69 6e 64 65   ..dut...nd finde
  0140:  74 2e 20 44 69 65 20 67  65 6d 65 69 6e 73 61 6d   t. Die gemeinsam
  0150:  65 20 41 72 62 65 69 74  20 61 6e 20 64 65 6d 20   e Arbeit an dem 
  0160:  50 72 6f 6a 65 6b 74 20  73 63 68 77 65 69 df 74   Projekt schwei.t
  0170:  20 64 69 65 20 62 65 69  64 65 6e 20 75 6e 74 65    die beiden unte
  0180:  72 73 63 68 69 65 64 6c  69 63 68 65 6e 20 43 68   rschiedlichen Ch
  0190:  61 72 61 6b 74 65 72 65  20 7a 75 73 61 6d 6d 65   araktere zusamme
  01a0:  6e 20 75 6e 64 20 4b 61  74 68 72 79 6e 20 62 72   n und Kathryn br
  01b0:  69 6e 67 74 20 66 72 69  73 63 68 65 6e 20 57 69   ingt frischen Wi
  01c0:  6e 64 20 69 6e 20 64 61  73 20 73 74 61 75 62 69   nd in das staubi
  01d0:  67 65 20 41 6b 61 64 65  6d 69 6b 65 72 2d 44 61   ge Akademiker-Da
  01e0:  73 65 69 6e 2e 20 50 6c  f6 74 7a 6c 69 63 68 20   sein. Pl.tzlich 
  01f0:  73 74 65 68 74 20 6a 65  64 6f 63 68 20 69 68 72   steht jedoch ihr
  0200:  20 45 78 66 72 65 75 6e  64 20 76 6f 72 20 64 65    Exfreund vor de
  0210:  72 20 54 fc 72 20 2e 2e  2e 0a 0a 52 65 67 69 65   r T.r .....Regie
  0220:  3a 20 48 6f 77 61 72 64  20 48 61 77 6b 73 0a 44   : Howard Hawks.D
  0230:  4e 35 22 64 75 74 00 2f  05 72 65 68 62 75 63 68   N5"dut./.rehbuch
  0240:  3a 20 42 69 6c 6c 79 20  57 69 6c 64 65 72 0a 4b   : Billy Wilder.K
  0250:  6f 6d 70 6f 6e 69 73 74  3a 20 41 6c 66 72 65 64   omponist: Alfred
  0260:  20 4e 65 77 6d 61 6e 55  18 4e 4c 44 00 42 45 4c    NewmanU.NLD.BEL
  0270:  00 44 45 55 00 43 5a 45  00 4c 55 58 00 53 56 4b   .DEU.CZE.LUX.SVK
  0280:  00 54 02 00 00 ae 59 14  72                        .T....Y.r)";

#endif // test_data_hpp__
