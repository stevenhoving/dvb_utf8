#ifndef dvb_utf8_stream_buffer_h__
#define dvb_utf8_stream_buffer_h__

#include <cstdint>
#include <vector>
#include <limits>
#include <stdexcept>

namespace dvb_utf8
{

class stream_buffer
{
    using value_type = std::vector<uint8_t>;
    using size_type = value_type::size_type;
    using iterator_type = value_type::iterator;
    using const_iterator_type = value_type::const_iterator;
public:
    stream_buffer() = default;

    // \todo add move operator
    stream_buffer(const std::vector<uint8_t> & data)
        : data_(data)
    {
    }

    void write(const uint8_t value)
    {
        prep_write_(sizeof(value));
        data_[index_++] = value;
    }

    void write(const uint16_t value)
    {
        prep_write_(sizeof(value));
        data_[index_++] = value & 0xFF;
        data_[index_++] = (value >> 8) & 0xFF;
    }

    void write(const value_type &data)
    {
        prep_write_(data.size());
        auto dst = &data_[index_];
        memcpy(dst, data.data(), data.size());
        index_ += data.size();
    }

    void prep_write_(int len)
    {
        // overflow protection?
        int64_t total_length = data_.size() + len;
        if (total_length >= std::numeric_limits<size_type>::max())
            throw std::runtime_error("Unable to resize stream buffer beyond 2^32");

        if (total_length > data_.size())
            data_.resize(static_cast<size_type>(total_length));
    }

    // todo implement specializations
    template<typename T>
    T read(const int len)
    {
        if (len > sizeof(T))
            throw std::runtime_error("panic");

        T result;
        memcpy(&result, &data_[index_], len);
        index_ += len;
        return result;
    }

    template<>
    uint8_t read<uint8_t>(const int len)
    {
        if (len > sizeof(uint8_t))
            throw std::runtime_error("panic");

        uint8_t result = (uint8_t)data_[index_];
        index_ += len;
        return result;
    }

    bool empty() const
    {
        return data_.empty();
    }

    // \todo check limits...
    void seek(int offset, int origin)
    {
        switch (origin)
        {
        case SEEK_SET:
            index_ = offset;
            break;
        case SEEK_CUR:
            index_ += offset;
            break;
        case SEEK_END:
            index_ = data_.size() - offset;
            break;
        }
    }

    uint8_t *data()
    {
        return data_.data();
    }

    size_type tell() const
    {
        return index_;
    }

    size_type size() const
    {
        return data_.size();
    }

    iterator_type begin()
    {
        return data_.begin();
    }

    iterator_type end()
    {
        return data_.end();
    }

    const_iterator_type begin() const
    {
        return data_.begin();
    }

    const_iterator_type end() const
    {
        return data_.end();
    }

    value_type data_ = {};
    int32_t index_ = 0;
};

} // namespace dvb_utf8

#endif // dvb_utf8_stream_buffer_h__
