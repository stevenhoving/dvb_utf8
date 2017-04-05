#ifndef dvb_utf8_stream_buffer_h__
#define dvb_utf8_stream_buffer_h__

#if DVB_UTF8_ENABLE_ENCODE

#include <cstdint>
#include <cstring>
#include <vector>
#include <limits>
#include <stdexcept>

namespace dvb_utf8
{

class stream_buffer
{
    using value_type = std::vector<uint8_t>;
    using size_type = value_type::size_type;
public:
    stream_buffer() = default;

    explicit stream_buffer(const std::vector<uint8_t> &data)
        : data_(data)
    {
    }

    explicit stream_buffer(std::vector<uint8_t> &&data) noexcept
        : data_(std::move(data))
    {
    }

    stream_buffer(stream_buffer &&other) = default;
    stream_buffer &operator=(stream_buffer &&other) = default;

    uint8_t peek_u8() const
    {
        return data_[index_];
    }

    void write(const uint8_t value) const
    {
        prep_write_(sizeof(value));
        data_[index_++] = value;
    }

    void write(const uint16_t value) const
    {
        prep_write_(sizeof(value));
        data_[index_++] = value & 0xFF;
        data_[index_++] = (value >> 8) & 0xFF;
    }

    void write(const value_type &data) const
    {
        // \todo should we throw a exception, or is it a warning?
        if (data.empty())
        {
            printf("Warning, we are attempting to write a empty buffer to the stream\n");
            return;
        }

        prep_write_(data.size());
        auto dst = &data_[index_];
        memmove(dst, data.data(), data.size());
        index_ += data.size();
    }

    void prep_write_(const int len) const
    {
        // \todo overflow/underflow protection.
        const auto total_length = data_.size() + len;
        if (total_length >= std::numeric_limits<size_type>::max())
            throw std::runtime_error("Unable to resize stream buffer beyond 2^32");

        if (total_length > data_.size())
        {
            // \todo when we write a string/vector or buffer we should use insert
            // instead of doing this + memmove/memcpy
            data_.resize(static_cast<size_type>(total_length));
        }
    }

    bool empty() const noexcept
    {
        return data_.empty();
    }

    // \todo fix integer math issues (int = int + int).
    void seek(const int offset, const int origin) const
    {
        int32_t new_index = 0;
        switch (origin)
        {
        case SEEK_SET:
            new_index = offset;
            break;
        case SEEK_CUR:
            new_index = index_ + offset;
            break;
        case SEEK_END:
            new_index = (data_.size() - 1) - offset;
            break;
        default:
            throw std::runtime_error("Invalid seek origin value");
            break;
        }

        if (new_index < 0)
            throw std::runtime_error("Invalid seek new index, negative");

        if (static_cast<size_type>(new_index) > data_.size())
            throw std::runtime_error("Invalid seek new index, to big");

        index_ = new_index;
    }

    uint8_t *data() noexcept
    {
        return data_.data();
    }

    const uint8_t *data() const noexcept
    {
        return data_.data();
    }

    size_type tell() const noexcept
    {
        return index_;
    }

    size_type size() const noexcept
    {
        return data_.size();
    }

    mutable value_type data_ = {};
    mutable int32_t index_ = 0; // write index
};

} // namespace dvb_utf8

#endif // DVB_UTF8_ENABLE_ENCODE
#endif // dvb_utf8_stream_buffer_h__
