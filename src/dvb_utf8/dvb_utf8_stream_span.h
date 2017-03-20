#ifndef dvb_utf8_stream_span_h__
#define dvb_utf8_stream_span_h__

#include "dvb_utf8_stream_span.h"

namespace dvb_utf8
{
// experiment to work around the buffer extraction + memory alloc in a fancy way
// \note read only all the way
class stream_span
{
public:
    stream_span() = delete;
    explicit stream_span(const uint8_t *begin, const uint8_t *end)
        : begin_(begin)
        , end_(end)
        , index_(0)
    {
    }

    /* cover floats and integers
    * \todo fix incorrect handling of different endianness. As atm the
    * endianness of the target platform is used.
    */
    template<typename T>
    auto read() const -> std::enable_if_t<std::is_arithmetic<T>::value, T>
    {
        if (begin_ +  index_ + sizeof(T) > end_)
            throw std::runtime_error("Unable to read beyond the span size");

        // \todo fix cast
        auto result = (const T*)(&begin_[index_]);
        index_ += sizeof(T);
        return *result;
    }

    // \todo check for length limits... and fix unsigned / signed undefined behavior.
    stream_span read_buffer(const int length) const
    {
        if (length <= 0)
            return stream_span(nullptr, nullptr);

        if (begin_ + length > end_)
            throw std::runtime_error("Unable to read buffer beyond the span size");

        auto result = stream_span(
            &begin_[index_],
            &begin_[index_ + length]
            );

        index_ += length;
        return result;
    }

    bool empty() const noexcept
    {
        return begin_ == nullptr;
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
            new_index = (size() - 1) - offset;
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
        return begin_;
    }

    const uint8_t *data() const noexcept
    {
        return begin_;
    }

    size_type tell() const noexcept
    {
        return index_;
    }

    size_type size() const noexcept
    {
        return end_ - begin_;
    }

    bool eos() const noexcept
    {
        // \todo use static cast
        return (size_type)index_ >= size();
    }

    const uint8_t *begin_;
    const uint8_t *end_;
    mutable int32_t index_; // read index
};

} // namespace dvb_utf8

#endif // dvb_utf8_stream_span_h__
