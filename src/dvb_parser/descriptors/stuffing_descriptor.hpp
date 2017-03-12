#ifndef stuffing_descriptor_hpp__
#define stuffing_descriptor_hpp__

struct stuffing_descriptor : descriptor
{
    explicit stuffing_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        // we can simply ignore this, although it is also used as a way to
        // invalidate previously coded descriptors.
        stream.seek(length, SEEK_CUR);
    }
};
#endif // stuffing_descriptor_hpp__
