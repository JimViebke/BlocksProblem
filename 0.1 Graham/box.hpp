#include <cstddef>

namespace uva
{
    class box
    {
    private:
        size_t _index;
        size_t _bucket;

    public:
        box(const size_t &i, const size_t &b) : _index(i), _bucket(b) { }

        size_t index() { return _index; }
        void index(const size_t &i) { _index = i; }

        size_t bucket() { return _bucket; }
        void bucket(const size_t &b) { _bucket = b; }
    };
}