#ifndef dvb_utf8_stream_span_h__
#define dvb_utf8_stream_span_h__

#include <vector> // for std::ptrdiff_t

namespace dvb_utf8
{
// experiment to work around the buffer extraction + memory alloc in a fancy way
// \note read only all the way
class stream_span
{
public:
    using size_type = std::ptrdiff_t;

    stream_span() = default;

    // construct from nullptr (empty span)
    explicit stream_span(std::nullptr_t) noexcept
        : stream_span(nullptr, 0)
    {
    }

    // construct from pointer + length
    explicit stream_span(uint8_t *begin, size_type size) noexcept
        : begin_(begin)
        , size_(size)
        , index_(0)
    {
    }

    // \todo make this std::enable_if ...
    explicit stream_span(std::vector<uint8_t> &container)
        : stream_span(container.data(), static_cast<size_type>(container.size()))
    {
    }

    ~stream_span() noexcept
    {
        // \note debugging check... in production mode this check is not needed.
        if (begin_ != nullptr && index_ != size_)
            DVB_DBG("Warning, stream_span was not completely parsed\n");
    }

    stream_span(stream_span &&other) noexcept
        : begin_(std::move(other.begin_))
        , size_(std::move(other.size_))
        , index_(std::move(other.index_))
    {
    }

    stream_span &operator=(stream_span &&other) noexcept
    {
        begin_ = std::move(other.begin_);
        size_ = std::move(other.size_);
        index_ = std::move(other.index_);

        // \note debugging... in production mode nulling the parameters is not needed.
        other.begin_ = nullptr;
        other.size_ = 0;
        other.index_ = 0;

        return *this;
    }

    stream_span(const stream_span &other) = delete;
    stream_span &operator=(const stream_span &other) = delete;

    /* cover floats and integers
    * \todo fix incorrect handling of different endianness. As atm the
    * endianness of the target platform is used.
    */
    template<typename T>
    auto read() const -> std::enable_if_t<std::is_arithmetic<T>::value, T>
    {
        if (index_ + static_cast<size_type>(sizeof(T)) > size_)
            throw std::runtime_error("Unable to read beyond the span size");

        // \todo fix cast
        auto result = (const T*)(&begin_[index_]);
        index_ += sizeof(T);
        return *result;
    }

    template<typename T>
    auto peek() const -> std::enable_if_t<std::is_arithmetic<T>::value, T>
    {
        if (index_ + static_cast<size_type>(sizeof(T)) > size_)
            throw std::runtime_error("Unable to peek beyond the buffer size");

        // \todo fix cast
        return *(const T*)(&begin_[index_]);
    }

    // \todo check for length limits... and fix unsigned / signed undefined behavior.
    stream_span read_buffer(const int length) const
    {
        if (length <= 0)
            return stream_span(nullptr);

        if (index_ + length > size_)
            throw std::runtime_error("Unable to read buffer beyond the span size");

        const auto begin = &begin_[index_];
        index_ += length;

        return stream_span(begin, length);
    }

    bool empty() const noexcept
    {
        return begin_ == nullptr || size_ == 0;
    }

    // \todo fix integer math issues (int = int + int).
    void seek(const int offset, const int origin) const
    {
        size_type new_index = 0;
        switch (origin)
        {
        case SEEK_SET:
            new_index = offset;
            break;
        case SEEK_CUR:
            new_index = index_ + offset;
            break;
        case SEEK_END:
            new_index = size_ - offset;
            break;
        default:
            throw std::runtime_error("Invalid seek origin value");
            break;
        }

        if (new_index < 0)
            throw std::runtime_error("Invalid seek new index, negative");

        if (new_index > size_)
            throw std::runtime_error("Invalid seek new index, to big");

        index_ = new_index;
    }

    uint8_t *data() noexcept
    {
        return const_cast<uint8_t *>(begin_);
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
        return size_;
    }

    bool eos() const noexcept
    {
        return index_ >= size_;
    }
private:
    uint8_t *begin_ = nullptr;
    size_type size_ = 0;
    mutable size_type index_ = 0;
};

} // namespace dvb_utf8

#endif // dvb_utf8_stream_span_h__
