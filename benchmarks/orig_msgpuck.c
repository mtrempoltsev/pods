#include <time.h>

#include <msgpuck.h>

#include "msgpack_data.h"

#define CHECK

#define CHECK_BUF_CAPACITY(n) \
    if (end - buf < n) return NULL

#define RET_IF_NULL(ptr) \
    if (ptr == NULL) return NULL

char* encodeName(char* buf, char* end, const char* name, uint32_t nameLen)
{
    CHECK_BUF_CAPACITY(1 + sizeof(uint32_t) + nameLen);
    return mp_encode_str(buf, name, nameLen);
}

char* encodeArray(char* buf, char* end, uint32_t size)
{
    CHECK_BUF_CAPACITY(1 + sizeof(uint32_t));
    return mp_encode_array(buf, size);
}

char* encodeMap(char* buf, char* end, uint32_t size)
{
    CHECK_BUF_CAPACITY(1 + sizeof(uint32_t));
    return mp_encode_map(buf, size);
}

char* encodeInt(char* buf, char* end, int64_t val)
{
    CHECK_BUF_CAPACITY(1 + sizeof(int64_t));
    if (val < 0)
        return mp_encode_int(buf, val);
    else
        return mp_encode_uint(buf, (uint64_t) val);
}

char* encodeUInt(char* buf, char* end, uint64_t val)
{
    CHECK_BUF_CAPACITY(1 + sizeof(uint64_t));
    return mp_encode_uint(buf, val);
}

char* encodeBin(char* buf, char* end, const char* data, uint32_t size)
{
    CHECK_BUF_CAPACITY(1 + sizeof(uint32_t) + size);
    return mp_encode_bin(buf, data, size);
}

char* serialize(struct Data* data, char* buf, size_t size)
{
    char* end = buf + size;

    buf = encodeArray(buf, end, StructsNum);
    RET_IF_NULL(buf);

    for (int i = 0; i < StructsNum; ++i)
    {
        buf = encodeName(buf, end, "first", 5);
        RET_IF_NULL(buf);
        buf = encodeMap(buf, end, 11);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "a", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].first.a);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "b", 1);
        RET_IF_NULL(buf);
        buf = encodeUInt(buf, end, data->third[i].first.b);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "c", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].first.c);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "d", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].first.d);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "e", 1);
        RET_IF_NULL(buf);
        buf = encodeUInt(buf, end, data->third[i].first.e);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "f", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].first.f);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "g", 1);
        RET_IF_NULL(buf);
        buf = encodeBin(buf, end, (const char*) data->third[i].first.g, Small * sizeof(uint64_t));
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "h", 1);
        RET_IF_NULL(buf);
        buf = encodeBin(buf, end, (const char*) data->third[i].first.h, Medium * sizeof(int32_t));
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "i", 1);
        RET_IF_NULL(buf);
        buf = encodeBin(buf, end, data->third[i].first.i, Large);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "k", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].first.k);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "l", 1);
        RET_IF_NULL(buf);
        buf = encodeUInt(buf, end, data->third[i].first.l);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "second", 6);
        RET_IF_NULL(buf);
        buf = encodeMap(buf, end, 11);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "a", 1);
        RET_IF_NULL(buf);
        buf = encodeUInt(buf, end, data->third[i].second.a);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "b", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].second.b);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "c", 1);
        RET_IF_NULL(buf);
        buf = encodeUInt(buf, end, data->third[i].second.c);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "d", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].second.d);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "e", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].second.e);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "f", 1);
        RET_IF_NULL(buf);
        buf = encodeUInt(buf, end, data->third[i].second.f);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "g", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].second.g);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "h", 1);
        RET_IF_NULL(buf);
        buf = encodeUInt(buf, end, data->third[i].second.h);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "i", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].second.i);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "k", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].second.k);
        RET_IF_NULL(buf);

        buf = encodeName(buf, end, "l", 1);
        RET_IF_NULL(buf);
        buf = encodeInt(buf, end, data->third[i].second.k);
        RET_IF_NULL(buf);
    }

    return buf;
}

const char* decodeName(const char* buf, const char* end, const char* name, uint32_t nameLen)
{
    CHECK_BUF_CAPACITY(1 + sizeof(uint32_t) + nameLen);

    uint32_t len = 0;
    const char* ptr = mp_decode_str(&buf, &len);

    if (len != nameLen)
        return NULL;

    if (memcmp(name, ptr, len) != 0)
        return NULL;

    return buf;
}

const char* decodeArray(const char* buf, const char* end, uint32_t size)
{
    CHECK_BUF_CAPACITY(1 + sizeof(uint32_t));

    uint32_t len = mp_decode_array(&buf);
    if (len != size)
        return NULL;

    return buf;
}

const char* decodeMap(const char* buf, const char* end, uint32_t size)
{
    CHECK_BUF_CAPACITY(1 + sizeof(uint32_t));

    uint32_t len = mp_decode_map(&buf);
    if (len != size)
        return NULL;

    return buf;
}

int64_t decodeInt(const char** buf, const char* end)
{
    if (end - *buf < 1 + sizeof(int64_t))
    {
        buf = NULL;
        return 0;
    }

    bool isInt = (mp_typeof(**buf) == MP_INT);
    if (isInt)
        return mp_decode_int(buf);
    else
        return (int64_t) mp_decode_uint(buf);
}

uint64_t decodeUInt(const char** buf, const char* end)
{
    if (end - *buf < 1 + sizeof(uint64_t))
    {
        buf = NULL;
        return 0;
    }

    return mp_decode_uint(buf);
}

const char* decodeBin(const char* buf, const char* end, char* data, uint32_t size)
{
    CHECK_BUF_CAPACITY(1 + sizeof(uint32_t) + size);

    uint32_t len = 0;
    const char* ptr = mp_decode_bin(&buf, &len);
    if (len != size)
        return NULL;

    memcpy(data, ptr, len);
    return buf;
}

const char* deserialize(struct Data* data, const char* buf, size_t size)
{
    const char* end = buf + size;

    buf = decodeArray(buf, end, StructsNum);
    RET_IF_NULL(buf);

    for (uint32_t  i = 0; i < StructsNum; ++i)
    {
        buf = decodeName(buf, end, "first", 5);
        RET_IF_NULL(buf);
        buf = decodeMap(buf, end, 11);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "a", 1);
        RET_IF_NULL(buf);
        data->third[i].first.a = decodeInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "b", 1);
        RET_IF_NULL(buf);
        data->third[i].first.b = decodeUInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "c", 1);
        RET_IF_NULL(buf);
        data->third[i].first.c = decodeInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "d", 1);
        RET_IF_NULL(buf);
        data->third[i].first.d = decodeInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "e", 1);
        RET_IF_NULL(buf);
        data->third[i].first.e = decodeUInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "f", 1);
        RET_IF_NULL(buf);
        data->third[i].first.f = decodeInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "g", 1);
        RET_IF_NULL(buf);
        buf = decodeBin(buf, end, (char*) data->third[i].first.g, Small * sizeof(uint64_t));
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "h", 1);
        RET_IF_NULL(buf);
        buf = decodeBin(buf, end, (char*) data->third[i].first.h, Medium * sizeof(int32_t));
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "i", 1);
        RET_IF_NULL(buf);
        buf = decodeBin(buf, end, data->third[i].first.i, Large);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "k", 1);
        RET_IF_NULL(buf);
        data->third[i].first.k = decodeInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "l", 1);
        RET_IF_NULL(buf);
        data->third[i].first.l = decodeUInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "second", 6);
        RET_IF_NULL(buf);
        buf = decodeMap(buf, end, 11);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "a", 1);
        RET_IF_NULL(buf);
        data->third[i].second.a = decodeUInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "b", 1);
        RET_IF_NULL(buf);
        data->third[i].second.b = decodeInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "c", 1);
        RET_IF_NULL(buf);
        data->third[i].second.c = decodeUInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "d", 1);
        RET_IF_NULL(buf);
        data->third[i].second.d = decodeInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "e", 1);
        RET_IF_NULL(buf);
        data->third[i].second.e = decodeInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "f", 1);
        RET_IF_NULL(buf);
        data->third[i].second.f = decodeUInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "g", 1);
        RET_IF_NULL(buf);
        data->third[i].second.g = decodeInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "h", 1);
        RET_IF_NULL(buf);
        data->third[i].second.h = decodeUInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "i", 1);
        RET_IF_NULL(buf);
        data->third[i].second.i = decodeInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "k", 1);
        RET_IF_NULL(buf);
        data->third[i].second.k = decodeInt(&buf, end);
        RET_IF_NULL(buf);

        buf = decodeName(buf, end, "l", 1);
        RET_IF_NULL(buf);
        data->third[i].second.l = decodeInt(&buf, end);
        RET_IF_NULL(buf);
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

    destroy(&data);

    free(buf);

    return 0;
}
