#ifndef dvb_utf8_stream_buffer_h__
#define dvb_utf8_stream_buffer_h__

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

    /* cover floats and integers
     * \todo fix incorrect handling of different endianness. As atm the
     * endianness of the target platform is used.
     */
    template<typename T>
    auto read() const -> std::enable_if_t<std::is_arithmetic<T>::value, T>
    {
        if (sizeof(T) + index_ > data_.size())
            throw std::runtime_error("Unable to read beyond the buffer size");

        // \todo fix cast
        auto result = (const T*)(&data_[index_]);
        index_ += sizeof(T);
        return *result;
    }

    template<typename T>
    auto peek() const -> std::enable_if_t<std::is_arithmetic<T>::value, T>
    {
        if (sizeof(T) + index_ > data_.size())
            throw std::runtime_error("Unable to peek beyond the buffer size");

        // \todo fix cast
        return *(const T*)(&data_[index_]);
    }

    // \todo check for length limits... and fix unsigned / signed undefined behavior.
    std::vector<uint8_t> read_buffer(const int length) const
    {
        if (length <=0)
            return std::vector<uint8_t>();

        if (static_cast<uint64_t>(length + index_) > data_.size())
            throw std::runtime_error("Unable to read buffer beyond the buffer size");

        auto result = std::vector<uint8_t>(
            &data_[index_],
            &data_[index_ + length]
        );

        index_ += length;
        return result;
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

    bool eos() const noexcept
    {
        // \todo use static cast
        return (size_type)index_ >= data_.size();
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

    bool has_range() const noexcept
    {
        return range_begin_ != -1;
    }

    void range_set(const int32_t length) const noexcept
    {
        range_begin_ = index_;
        range_end_ = index_ + length;
    }

    void range_mark_end() const
    {
        if (index_ != range_end_)
            throw std::runtime_error("stream buffer 'sub' range not satisfied");

        range_begin_ = -1;
        range_end_ = -1;
    }

    bool range_eos() const noexcept
    {
        return index_ == range_end_;
    }

    int range_size() const noexcept
    {
        return range_end_ - range_begin_;
    }

    mutable value_type data_ = {};
    mutable int32_t index_ = 0; // read index

    mutable int32_t range_begin_ = -1;
    mutable int32_t range_end_ = -1;
};

} // namespace dvb_utf8

#endif // dvb_utf8_stream_buffer_h__
