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
public:
    stream_buffer() = default;

    explicit stream_buffer(const std::vector<uint8_t> &data)
        : data_(data)
    {
    }

    explicit stream_buffer(std::vector<uint8_t> &&data)
        : data_(std::move(data))
    {
    }

    stream_buffer(stream_buffer &&other) = default;
    stream_buffer &operator=(stream_buffer &&other) = default;

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

    void prep_write_(const int len)
    {
        // overflow protection?
        const int64_t total_length = data_.size() + len;
        if (total_length >= std::numeric_limits<size_type>::max())
            throw std::runtime_error("Unable to resize stream buffer beyond 2^32");

        if (total_length > data_.size())
            data_.resize(static_cast<size_type>(total_length));
    }

    /* cover floats and integers
     * \todo fix incorrect handling of different endianness. As atm the
     * endianness of the target platform is used.
     */
    template<typename T>
    auto read() const -> std::enable_if_t<std::is_arithmetic<T>::value, T>
    {
        if (sizeof(T) + index_ > data_.size())
            throw std::runtime_error("Unable to read beyond the buffer size");

        auto result = static_cast<const T*>(&data_[index_]);
        index_ += sizeof(T);
        return *result;
    }

    bool empty() const noexcept
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
        default:
            throw std::runtime_error("Invalid seek origin value");
            break;
        }
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

    auto begin() noexcept
    {
        return data_.begin();
    }

    auto end() noexcept
    {
        return data_.end();
    }

    auto begin() const noexcept
    {
        return data_.begin();
    }

    auto end() const noexcept
    {
        return data_.end();
    }

    value_type data_ = {};
    mutable int32_t index_ = 0;
};

} // namespace dvb_utf8

#endif // dvb_utf8_stream_buffer_h__
