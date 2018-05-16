#include <time.h>

#include <msgpuck.h>

#include "msgpack_data.h"

#define CHECK if (buf > end) return NULL

#define ENCODE_MAP(name, len, size) \
    buf = mp_encode_str(buf, name, len); \
    CHECK; \
    buf = mp_encode_map(buf, size); \
    CHECK;

#define ENCODE_INT(name, val) \
    buf = mp_encode_str(buf, name, 1); \
    CHECK; \
    if (val < 0) \
        buf = mp_encode_int(buf, val); \
    else \
        buf = mp_encode_uint(buf, val); \
    CHECK;

#define ENCODE_UINT(name, val) \
    buf = mp_encode_str(buf, name, 1); \
    CHECK; \
    buf = mp_encode_uint(buf, val); \
    CHECK;

#define ENCODE_BIN(name, val, size) \
    buf = mp_encode_str(buf, name, 1); \
    CHECK; \
    buf = mp_encode_bin(buf, (const char*) val, size); \
    CHECK;

char* serialize(struct Data* data, char* buf, size_t size)
{
    char* end = buf + size;

    buf = mp_encode_array(buf, StructsNum);
    CHECK;

    char* start = buf;
    for (int i = 0; i < StructsNum; ++i)
    {
        ENCODE_MAP("first", 5, 11);

        ENCODE_INT("a", data->third[i].first.a)
        ENCODE_UINT("b", data->third[i].first.b)
        ENCODE_INT("c", data->third[i].first.c)
        ENCODE_INT("d", data->third[i].first.d)
        ENCODE_UINT("e", data->third[i].first.e)
        ENCODE_INT("f", data->third[i].first.f)
        ENCODE_BIN("g", data->third[i].first.g, Small * sizeof(uint64_t))
        ENCODE_BIN("h", data->third[i].first.h, Medium * sizeof(int32_t))
        ENCODE_BIN("i", data->third[i].first.i, Large)
        ENCODE_INT("k", data->third[i].first.k)
        ENCODE_UINT("l", data->third[i].first.l)

        ENCODE_MAP("second", 6, 11);

        ENCODE_INT("a", data->third[i].second.a)
        ENCODE_UINT("b", data->third[i].second.b)
        ENCODE_INT("c", data->third[i].second.c)
        ENCODE_INT("d", data->third[i].second.d)
        ENCODE_UINT("e", data->third[i].second.e)
        ENCODE_INT("f", data->third[i].second.f)
        ENCODE_UINT("g", data->third[i].second.g)
        ENCODE_INT("h", data->third[i].second.h)
        ENCODE_INT("i", data->third[i].second.i)
        ENCODE_INT("k", data->third[i].second.k)
        ENCODE_UINT("l", data->third[i].second.l)
    }

    return buf;
}

#define DECODE_MAP(name, expect1, expect2) \
    mp_decode_str(&buf, &len); \
    if (len != expect1) return NULL; \
    CHECK; \
    len = mp_decode_map(&buf); \
    if (len != expect2) return NULL; \
    CHECK;

#define DECODE_INT(name, val) \
    mp_decode_str(&buf, &len); \
    if (len != 1) return NULL; \
    CHECK; \
    if (mp_typeof(*buf) == MP_INT) \
        val = mp_decode_int(&buf); \
    else \
        val = mp_decode_uint(&buf); \
    CHECK;

#define DECODE_UINT(name, val) \
    mp_decode_str(&buf, &len); \
    if (len != 1) return NULL; \
    CHECK; \
    val = mp_decode_uint(&buf); \
    CHECK;

#define DECODE_BIN(name, val, expect) \
    mp_decode_str(&buf, &len); \
    if (len != 1) return NULL; \
    CHECK; \
    val = mp_decode_bin(&buf, &len); \
    if (len != expect) return NULL; \
    CHECK;

char* deserialize(struct Data* data, char* buf, size_t size)
{
    char* end = buf + size;

    uint32_t arrSize = mp_decode_array(&buf);
    CHECK;

    if (arrSize != StructsNum) return NULL;

    for (uint32_t  i = 0; i < arrSize; ++i)
    {
        uint32_t len = 0;

        DECODE_MAP("first", 5, 11);

        struct First f;

        DECODE_INT("a", f.a)
        DECODE_UINT("b", f.b)
        DECODE_INT("c", f.c)
        DECODE_INT("d", f.d)
        DECODE_UINT("e", f.e)
        DECODE_INT("f", f.f)
        DECODE_BIN("g", f.g, Small * sizeof(uint64_t))
        DECODE_BIN("h", f.h, Medium * sizeof(int32_t))
        DECODE_BIN("i", f.i, Large)
        DECODE_INT("k", f.k)
        DECODE_UINT("l", f.l)

        data->third[i].first = f;

        DECODE_MAP("second", 6, 11);

        struct Second s;

        DECODE_INT("a", s.a)
        DECODE_UINT("b", s.b)
        DECODE_INT("c", s.c)
        DECODE_INT("d", s.d)
        DECODE_UINT("e", s.e)
        DECODE_INT("f", s.f)
        DECODE_UINT("g", s.g)
        DECODE_INT("h", s.h)
        DECODE_INT("i", s.i)
        DECODE_INT("k", s.k)
        DECODE_UINT("l", s.l)

        data->third[i].second = s;
    }

    return buf;
}

void printSummary(size_t n, size_t size, clock_t start)
{
    clock_t diff = clock() - start;

    const size_t mb = 1024 * 1024;
    printf("    total: %d Mb\n", n * size / mb);

    int ms = diff * 1000 / CLOCKS_PER_SEC;
    printf("    total time: %d ms\n", ms);

    if (ms == 0)
    {
        ms = 1;
    }
    const size_t speed = n * size * 1000 / mb / ms;
    printf("    speed: %d Mb/s\n", speed);
}

int main()
{
    struct Data data;
    init(&data);

    char* buf = (char*) malloc(MaxSerializedSize);

    printf("trying to serialize... ");
    char* end = serialize(&data, buf, MaxSerializedSize);

    if (!end)
    {
        printf("FAILURE\n");
        return 1;
    }

    size_t size = end - buf;

    printf("ok\n");
    printf("archive size: %d bytes\n", size);

    printf("serialize %d times\n", Repeats);

    clock_t start = clock();

    for (int i = 0; i < Repeats; ++i)
    {
        if (!serialize(&data, buf, MaxSerializedSize))
        {
            printf("FAILURE\n");
            return 1;
        }
    }

    printSummary(Repeats, size, start);

    destroy(&data);

    printf("deserialize %d times\n", Repeats);

    start = clock();

    for (int i = 0; i < Repeats; ++i)
    {
        if (!deserialize(&data, buf, MaxSerializedSize))
        {
            printf("FAILURE\n");
            return 1;
        }
    }

    printSummary(Repeats, size, start);

    free(buf);

    return 0;
}
