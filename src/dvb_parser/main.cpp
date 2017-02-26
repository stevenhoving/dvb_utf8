#include "dvb_parser.h"

std::vector<uint8_t> read_test_data(const std::string &path)
{
    std::vector<uint8_t> result;
    FILE * fp = fopen(path.c_str(), "rb");
    if (!fp)
    {
        printf("unable to open: %s\n", path.c_str());
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    auto fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    result.resize(fsize);
    fread(&result[0], fsize, 1, fp);
    fclose(fp);

    return result;
}

void print_hexview(const std::vector<uint8_t> &data)
{
    for (size_t i = 0; i < data.size(); ++i)
        printf("%02X ", data[i]);
}

static const std::string hex_packet =
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
    if (first.size() != 2)
        abort();

    auto sections = string_split(first[0], "  ");
    auto hex_view1 = string_split(sections[2], " ");
    auto hex_view2 = string_split(sections[3], " ");

    if (hex_view1[0] == "") hex_view1.erase(hex_view1.begin());
    if (hex_view2[0] == "") hex_view2.erase(hex_view2.begin());

    static int hex_to_nibble[255];
    hex_to_nibble['0'] = 0;
    hex_to_nibble['1'] = 1;
    hex_to_nibble['2'] = 2;
    hex_to_nibble['3'] = 3;
    hex_to_nibble['4'] = 4;
    hex_to_nibble['5'] = 5;
    hex_to_nibble['6'] = 6;
    hex_to_nibble['7'] = 7;
    hex_to_nibble['8'] = 8;
    hex_to_nibble['9'] = 9;
    hex_to_nibble['a'] = 0xA;
    hex_to_nibble['b'] = 0xB;
    hex_to_nibble['c'] = 0xC;
    hex_to_nibble['d'] = 0xD;
    hex_to_nibble['e'] = 0xE;
    hex_to_nibble['f'] = 0xF;

    auto result = std::vector<uint8_t>();
    for (auto itr : hex_view1)
        result.emplace_back(hex_to_nibble[itr[0]] << 4 | hex_to_nibble[itr[1]]);

    for (auto itr : hex_view2)
        result.emplace_back(hex_to_nibble[itr[0]] << 4 | hex_to_nibble[itr[1]]);

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

int main()
{
    //auto stream = dvb_utf8::stream_buffer(hex_packet_to_data(hex_packet));


    auto stream = dvb_utf8::stream_buffer(read_test_data("D:/dev/dvb_utf8/pid18.raw"));
    //auto stream = dvb_utf8::stream_buffer(read_test_data("D:/dev/dvb_utf8/pid17.raw"));

    bool fail = false;
    while (!stream.eos() && fail == false)
    {
        auto table_id = stream.peek<uint8_t>();

        switch (table_id)
        {
        case 0x42: {
            auto section = dvb_parse::service_description_section(stream);
        } break;

        case 0x4e:
        case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
        case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E: case 0x6F: {
            auto section = dvb_parse::event_information_section(stream);
        } break;
        default:
            printf("unsupported table id: %u\n", table_id);
            fail = true;
            break;
        }
    }

    if (stream.eos())
        printf("parsed everything\n");
    else
        printf("parsed incomplete\n");




    return 0;
}