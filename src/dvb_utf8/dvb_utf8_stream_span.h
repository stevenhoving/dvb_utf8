#ifndef dvb_utf8_stream_span_h__
#define dvb_utf8_stream_span_h__

namespace dvb_utf8
{
// experiment to work around the buffer extraction + memory alloc in a fancy way
// \note read only all the way
class stream_span
{
    using size_type = std::ptrdiff_t;
public:
    stream_span() = default;
    explicit stream_span(const uint8_t *begin, const uint8_t *end) noexcept
        : begin_(begin)
        , end_(end)
        , index_(0)
    {
    }
    ~stream_span() noexcept
    {
        // \note debugging check... in production mode this check is not needed.
        if (begin_ != nullptr && end_ != nullptr && index_ != size())
            printf("Warning, stream_span was not completely parsed\n");
    }

    stream_span(stream_span &&other) noexcept
        : begin_(std::move(other.begin_))
        , end_(std::move(other.end_))
        , index_(std::move(other.index_))
    {
    }

    stream_span &operator=(stream_span &&other) noexcept
    {
        begin_ = std::move(other.begin_);
        end_ = std::move(other.end_);
        index_ = std::move(other.index_);

        // \note debugging... in production mode nulling the parameters is not needed.
        other.begin_ = nullptr;
        other.end_ = nullptr;
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
        if (begin_ +  index_ + sizeof(T) > end_)
            throw std::runtime_error("Unable to read beyond the span size");

        // \todo fix cast
        auto result = (const T*)(&begin_[index_]);
        index_ += sizeof(T);
        return *result;
    }

    template<typename T>
    auto peek() const -> std::enable_if_t<std::is_arithmetic<T>::value, T>
    {
        if (begin_ + index_ + sizeof(T) > end_)
            throw std::runtime_error("Unable to peek beyond the buffer size");

        // \todo fix cast
        return *(const T*)(&begin_[index_]);
    }

    // \todo check for length limits... and fix unsigned / signed undefined behavior.
    stream_span read_buffer(const int length) const
    {
        if (length <= 0)
            return stream_span(nullptr, nullptr);

        if (begin_ + length > end_)
            throw std::runtime_error("Unable to read buffer beyond the span size");

        const auto begin = &begin_[index_];
        const auto end = &begin_[index_ + length];
        index_ += length;

        return stream_span(begin, end);
    }

    bool empty() const noexcept
    {
        return begin_ == nullptr;
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
            new_index = (size() - 1) - offset;
            break;
        default:
            throw std::runtime_error("Invalid seek origin value");
            break;
        }

        if (new_index < 0)
            throw std::runtime_error("Invalid seek new index, negative");

        if (new_index > size())
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
        return end_ - begin_;
    }

    bool eos() const noexcept
    {
        return index_ >= size();
    }
private:
    const uint8_t *begin_ = nullptr;
    const uint8_t *end_ = nullptr;
    mutable size_type index_ = 0;
};

} // namespace dvb_utf8

#endif // dvb_utf8_stream_span_h__
