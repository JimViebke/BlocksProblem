#include <cstddef>

namespace uva
{
    class bucket
    {
    private:
        size_t _begin;
        size_t _end;
        size_t _next;
    public:
        bucket(const size_t &b, const size_t &e, const size_t &n) : _begin(b), _end(e), _next(n) { }

        size_t begin() { return _begin; }
        void begin(const size_t &b) { _begin = b; }

        size_t end() { return _end; }
        void end(const size_t &e) { _end = e; }

        size_t next() { return _next; }
        void next(const size_t &n) { _next = n; }
    };
}