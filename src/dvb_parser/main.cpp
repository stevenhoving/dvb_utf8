#include "dvb_parser.hpp"
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
    //auto stream = dvb_utf8::stream_buffer(hex_packet_to_data(hex_packet_pid18_3));
    auto stream = dvb_utf8::stream_buffer(read_test_data("D:/dev/dvb_utf8/pid18.raw"));


    bool fail = false;
    while (!payload.eos() && fail == false)
    {
        auto table_id = payload.peek<uint8_t>();

        if (table_id >= 0x4E && table_id <= 0x6F)
            auto section = dvb_parse::event_information_section(payload);
        else if (table_id == 0x42 || table_id == 0x46)
            auto section = dvb_parse::service_description_section(payload);
        else if (table_id == 0x40)
            auto section = dvb_parse::network_information_section(payload);
        else
        {
            printf("unsupported table id: 0x%X(%u)\n", table_id, table_id);
            fail = true;
        }
    }


    if (payload.eos())
        printf("parsed everything\n");
    else
        printf("parsed incomplete\n");

    return 0;
}