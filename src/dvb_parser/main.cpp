#include "dvb_parser.hpp"
#include "dvb_test_data.hpp"

#include "stop_watch.h"

using namespace dvb_parse;


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

int main()
{
    //auto stream = read_test_data("D:/dev/dvb_utf8/pid16.raw");
    auto stream = read_test_data("D:/dev/dvb_utf8/pid18.raw");
    //auto stream = read_test_data("D:/dev/dvb_utf8/pid17.raw");
    //auto stream = read_test_data("D:/dev/dvb_utf8/pid18-2.raw");
    auto payload = dvb_utf8::stream_span(stream.data(), &stream.data()[stream.size()]);

    stop_watch stopwatch;
    stopwatch.time_start();

    bool fail = false;
    while (!payload.eos() && fail == false)
    {
        auto table_id = static_cast<table_ids>(payload.peek<uint8_t>());

        if (table_id >= table_ids::eit_actual && table_id <= table_ids::eit_other_sched_f)
            auto section = dvb_parse::event_information_section(payload);
        else if (table_id == table_ids::sdt_actual || table_id == table_ids::sdt_other)
            auto section = dvb_parse::service_description_section(payload);
        else if (table_id == table_ids::nit_actual || table_id == table_ids::nit_other)
            auto section = dvb_parse::network_information_section(payload);
        else
        {
            printf("unsupported table id: 0x%X(%u)\n", table_id, table_id);
            fail = true;
        }
    }

    printf("parse time: %f\n", stopwatch.time_since());

    if (payload.eos())
        printf("parsed everything\n");
    else
        printf("parsed incomplete\n");

    return 0;
}