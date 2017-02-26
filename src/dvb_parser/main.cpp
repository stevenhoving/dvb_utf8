#include "dvb_parser.h"
#include "dvb_table_id.hpp"
#include "dvb_test_data.hpp"

void write_test_data(const std::string &path, std::vector<uint8_t> data)
{
    FILE * fp = fopen(path.c_str(), "wb");
    if (!fp)
    {
        printf("unable to open: %s\n", path.c_str());
        exit(1);
    }

    fwrite(data.data(), data.size(), 1, fp);
    fclose(fp);
}

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

int main()
{
    auto stream = dvb_utf8::stream_buffer(hex_packet_to_data(hex_packet_pid18_2));

    bool fail = false;
    while (!stream.eos() && fail == false)
    {
        auto table_id = stream.peek<uint8_t>();

        switch (table_id)
        {
        case 0x42: {
            auto section = dvb_parse::service_description_section(stream);
        } break;

        case 0x4E: case 0x4F:
        case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
        case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E: case 0x6F: {
            auto section = dvb_parse::event_information_section(stream);
        } break;
        default:
            printf("unsupported table id: 0x%X(%u)\n", table_id, table_id);
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