#include <cstdio>

#include <msgpuck.h>

#define CHECK_MEM if (p - buf > max) { exit(1); }

struct Data
{
    uint16_t us0;
    int32_t i0;
    bool b0;
    uint32_t ui0;
    int8_t by0;
    int64_t l0;
    uint8_t ub0;
    uint64_t ul0;
    int16_t s0;
    float f0;
    double d0;
    bool ba0[64];
    int8_t bya0[64];
    uint8_t uba0[64];
    int16_t sa0[64];
    uint16_t usa0[64];
    int32_t ia0[64];
    uint32_t uia0[64];
    int64_t la0[64];
    uint64_t ula0[64];
    char ca0[64];
    float fa0[64];
    double da0[64];
    uint16_t us1;
    int32_t i1;
    bool b1;
    uint32_t ui1;
    int8_t by1;
    int64_t l1;
    uint8_t ub1;
    uint64_t ul1;
    int16_t s1;
    float f1;
    double d1;
    bool ba1[64];
    int8_t bya1[64];
    uint8_t uba1[64];
    int16_t sa1[64];
    uint16_t usa1[64];
    int32_t ia1[64];
    uint32_t uia1[64];
    int64_t la1[64];
    uint64_t ula1[64];
    char ca1[64];
    float fa1[64];
    double da1[64];
    uint16_t us2;
    int32_t i2;
    bool b2;
    uint32_t ui2;
    int8_t by2;
    int64_t l2;
    uint8_t ub2;
    uint64_t ul2;
    int16_t s2;
    float f2;
    double d2;
    bool ba2[64];
    int8_t bya2[64];
    uint8_t uba2[64];
    int16_t sa2[64];
    uint16_t usa2[64];
    int32_t ia2[64];
    uint32_t uia2[64];
    int64_t la2[64];
    uint64_t ula2[64];
    char ca2[64];
    float fa2[64];
    double da2[64];
    uint16_t us3;
    int32_t i3;
    bool b3;
    uint32_t ui3;
    int8_t by3;
    int64_t l3;
    uint8_t ub3;
    uint64_t ul3;
    int16_t s3;
    float f3;
    double d3;
    bool ba3[64];
    int8_t bya3[64];
    uint8_t uba3[64];
    int16_t sa3[64];
    uint16_t usa3[64];
    int32_t ia3[64];
    uint32_t uia3[64];
    int64_t la3[64];
    uint64_t ula3[64];
    char ca3[64];
    float fa3[64];
    double da3[64];
};

int main()
{
    Data d;
    memset(&d, 0, sizeof(Data));

    char* buf = (char*) malloc(12 * 1024);

    char* p = buf;

    p = mp_encode_uint(p, d.us0);
    if (d.i0 < 0)
        p = mp_encode_int(p, d.i0);
    else
        p = mp_encode_uint(p, d.i0);
    p = mp_encode_bool(p, d.b0);
    p = mp_encode_uint(p, d.ui0);
    if (d.by0 < 0)
        p = mp_encode_int(p, d.by0);
    else
        p = mp_encode_uint(p, d.by0);
    if (d.l0 < 0)
        p = mp_encode_int(p, d.l0);
    else
        p = mp_encode_uint(p, d.l0);
    p = mp_encode_uint(p, d.ub0);
    p = mp_encode_uint(p, d.ul0);
    if (d.s0 < 0)
        p = mp_encode_int(p, d.s0);
    else
        p = mp_encode_uint(p, d.s0);
    p = mp_encode_double(p, d.f0);
    p = mp_encode_double(p, d.d0);
    p = mp_encode_bin(p, (char*) d.ba0, sizeof(d.ba0));
    p = mp_encode_bin(p, (char*) d.bya0, sizeof(d.bya0));
    p = mp_encode_bin(p, (char*) d.uba0, sizeof(d.uba0));
    p = mp_encode_bin(p, (char*) d.sa0, sizeof(d.sa0));
    p = mp_encode_bin(p, (char*) d.usa0, sizeof(d.usa0));
    p = mp_encode_bin(p, (char*) d.ia0, sizeof(d.ia0));
    p = mp_encode_bin(p, (char*) d.uia0, sizeof(d.uia0));
    p = mp_encode_bin(p, (char*) d.la0, sizeof(d.la0));
    p = mp_encode_bin(p, (char*) d.ula0, sizeof(d.ula0));
    p = mp_encode_bin(p, (char*) d.ca0, sizeof(d.ca0));
    p = mp_encode_bin(p, (char*) d.fa0, sizeof(d.fa0));
    p = mp_encode_bin(p, (char*) d.da0, sizeof(d.da0));

    p = mp_encode_uint(p, d.us1);
    if (d.i1 < 0)
        p = mp_encode_int(p, d.i1);
    else
        p = mp_encode_uint(p, d.i1);
    p = mp_encode_bool(p, d.b1);
    p = mp_encode_uint(p, d.ui1);
    if (d.by1 < 0)
        p = mp_encode_int(p, d.by1);
    else
        p = mp_encode_uint(p, d.by1);
    if (d.l1 < 0)
        p = mp_encode_int(p, d.l1);
    else
        p = mp_encode_uint(p, d.l1);
    p = mp_encode_uint(p, d.ub1);
    p = mp_encode_uint(p, d.ul1);
    if (d.s1 < 0)
        p = mp_encode_int(p, d.s1);
    else
        p = mp_encode_uint(p, d.s1);
    p = mp_encode_double(p, d.f1);
    p = mp_encode_double(p, d.d1);
    p = mp_encode_bin(p, (char*) d.ba1, sizeof(d.ba1));
    p = mp_encode_bin(p, (char*) d.bya1, sizeof(d.bya1));
    p = mp_encode_bin(p, (char*) d.uba1, sizeof(d.uba1));
    p = mp_encode_bin(p, (char*) d.sa1, sizeof(d.sa1));
    p = mp_encode_bin(p, (char*) d.usa1, sizeof(d.usa1));
    p = mp_encode_bin(p, (char*) d.ia1, sizeof(d.ia1));
    p = mp_encode_bin(p, (char*) d.uia1, sizeof(d.uia1));
    p = mp_encode_bin(p, (char*) d.la1, sizeof(d.la1));
    p = mp_encode_bin(p, (char*) d.ula1, sizeof(d.ula1));
    p = mp_encode_bin(p, (char*) d.ca1, sizeof(d.ca1));
    p = mp_encode_bin(p, (char*) d.fa1, sizeof(d.fa1));
    p = mp_encode_bin(p, (char*) d.da1, sizeof(d.da1));

    p = mp_encode_uint(p, d.us2);
    if (d.i2 < 0)
        p = mp_encode_int(p, d.i2);
    else
        p = mp_encode_uint(p, d.i2);
    p = mp_encode_bool(p, d.b2);
    p = mp_encode_uint(p, d.ui2);
    if (d.by2 < 0)
        p = mp_encode_int(p, d.by2);
    else
        p = mp_encode_uint(p, d.by2);
    if (d.l2 < 0)
        p = mp_encode_int(p, d.l2);
    else
        p = mp_encode_uint(p, d.l2);
    p = mp_encode_uint(p, d.ub2);
    p = mp_encode_uint(p, d.ul2);
    if (d.s2 < 0)
        p = mp_encode_int(p, d.s2);
    else
        p = mp_encode_uint(p, d.s2);
    p = mp_encode_double(p, d.f2);
    p = mp_encode_double(p, d.d2);
    p = mp_encode_bin(p, (char*) d.ba2, sizeof(d.ba2));
    p = mp_encode_bin(p, (char*) d.bya2, sizeof(d.bya2));
    p = mp_encode_bin(p, (char*) d.uba2, sizeof(d.uba2));
    p = mp_encode_bin(p, (char*) d.sa2, sizeof(d.sa2));
    p = mp_encode_bin(p, (char*) d.usa2, sizeof(d.usa2));
    p = mp_encode_bin(p, (char*) d.ia2, sizeof(d.ia2));
    p = mp_encode_bin(p, (char*) d.uia2, sizeof(d.uia2));
    p = mp_encode_bin(p, (char*) d.la2, sizeof(d.la2));
    p = mp_encode_bin(p, (char*) d.ula2, sizeof(d.ula2));
    p = mp_encode_bin(p, (char*) d.ca2, sizeof(d.ca2));
    p = mp_encode_bin(p, (char*) d.fa2, sizeof(d.fa2));
    p = mp_encode_bin(p, (char*) d.da2, sizeof(d.da2));

    p = mp_encode_uint(p, d.us3);
    if (d.i3 < 0)
        p = mp_encode_int(p, d.i3);
    else
        p = mp_encode_uint(p, d.i3);
    p = mp_encode_bool(p, d.b3);
    p = mp_encode_uint(p, d.ui3);
    if (d.by3 < 0)
        p = mp_encode_int(p, d.by3);
    else
        p = mp_encode_uint(p, d.by3);
    if (d.l3 < 0)
        p = mp_encode_int(p, d.l3);
    else
        p = mp_encode_uint(p, d.l3);
    p = mp_encode_uint(p, d.ub3);
    p = mp_encode_uint(p, d.ul3);
    if (d.s3 < 0)
        p = mp_encode_int(p, d.s3);
    else
        p = mp_encode_uint(p, d.s3);
    p = mp_encode_double(p, d.f3);
    p = mp_encode_double(p, d.d3);
    p = mp_encode_bin(p, (char*) d.ba3, sizeof(d.ba3));
    p = mp_encode_bin(p, (char*) d.bya3, sizeof(d.bya3));
    p = mp_encode_bin(p, (char*) d.uba3, sizeof(d.uba3));
    p = mp_encode_bin(p, (char*) d.sa3, sizeof(d.sa3));
    p = mp_encode_bin(p, (char*) d.usa3, sizeof(d.usa3));
    p = mp_encode_bin(p, (char*) d.ia3, sizeof(d.ia3));
    p = mp_encode_bin(p, (char*) d.uia3, sizeof(d.uia3));
    p = mp_encode_bin(p, (char*) d.la3, sizeof(d.la3));
    p = mp_encode_bin(p, (char*) d.ula3, sizeof(d.ula3));
    p = mp_encode_bin(p, (char*) d.ca3, sizeof(d.ca3));
    p = mp_encode_bin(p, (char*) d.fa3, sizeof(d.fa3));
    p = mp_encode_bin(p, (char*) d.da3, sizeof(d.da3));

    int size = p - buf;

    printf("size: %d\n", size);

    int N = 10000;
    int R = 50;

    free(buf);

    buf = (char*) malloc(N * size);

    int max = N * size;

    for (int i = 0; i < R; ++i)
    {
        p = buf;

        for (int j = 0; j < N; ++j)
        {
            p = mp_encode_uint(p, d.us0);
            CHECK_MEM
            if (d.i0 < 0)
                p = mp_encode_int(p, d.i0);
            else
                p = mp_encode_uint(p, d.i0);
            CHECK_MEM
            p = mp_encode_bool(p, d.b0);
            CHECK_MEM
            p = mp_encode_uint(p, d.ui0);
            CHECK_MEM
            if (d.by0 < 0)
                p = mp_encode_int(p, d.by0);
            else
                p = mp_encode_uint(p, d.by0);
            CHECK_MEM
            if (d.l0 < 0)
                p = mp_encode_int(p, d.l0);
            else
                p = mp_encode_uint(p, d.l0);
            CHECK_MEM
            p = mp_encode_uint(p, d.ub0);
            CHECK_MEM
            p = mp_encode_uint(p, d.ul0);
            CHECK_MEM
            if (d.s0 < 0)
                p = mp_encode_int(p, d.s0);
            else
                p = mp_encode_uint(p, d.s0);
            CHECK_MEM
            p = mp_encode_double(p, d.f0);
            CHECK_MEM
            p = mp_encode_double(p, d.d0);
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ba0, sizeof(d.ba0));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.bya0, sizeof(d.bya0));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.uba0, sizeof(d.uba0));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.sa0, sizeof(d.sa0));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.usa0, sizeof(d.usa0));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ia0, sizeof(d.ia0));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.uia0, sizeof(d.uia0));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.la0, sizeof(d.la0));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ula0, sizeof(d.ula0));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ca0, sizeof(d.ca0));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.fa0, sizeof(d.fa0));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.da0, sizeof(d.da0));
            CHECK_MEM

            p = mp_encode_uint(p, d.us1);
            CHECK_MEM
            if (d.i1 < 0)
                p = mp_encode_int(p, d.i1);
            else
                p = mp_encode_uint(p, d.i1);
            CHECK_MEM
            p = mp_encode_bool(p, d.b1);
            CHECK_MEM
            p = mp_encode_uint(p, d.ui1);
            CHECK_MEM
            if (d.by1 < 0)
                p = mp_encode_int(p, d.by1);
            else
                p = mp_encode_uint(p, d.by1);
            CHECK_MEM
            if (d.l1 < 0)
                p = mp_encode_int(p, d.l1);
            else
                p = mp_encode_uint(p, d.l1);
            CHECK_MEM
            p = mp_encode_uint(p, d.ub1);
            CHECK_MEM
            p = mp_encode_uint(p, d.ul1);
            CHECK_MEM
            if (d.s1 < 0)
                p = mp_encode_int(p, d.s1);
            else
                p = mp_encode_uint(p, d.s1);
            CHECK_MEM
            p = mp_encode_double(p, d.f1);
            CHECK_MEM
            p = mp_encode_double(p, d.d1);
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ba1, sizeof(d.ba1));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.bya1, sizeof(d.bya1));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.uba1, sizeof(d.uba1));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.sa1, sizeof(d.sa1));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.usa1, sizeof(d.usa1));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ia1, sizeof(d.ia1));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.uia1, sizeof(d.uia1));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.la1, sizeof(d.la1));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ula1, sizeof(d.ula1));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ca1, sizeof(d.ca1));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.fa1, sizeof(d.fa1));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.da1, sizeof(d.da1));
            CHECK_MEM

            p = mp_encode_uint(p, d.us2);
            CHECK_MEM
            if (d.i2 < 0)
                p = mp_encode_int(p, d.i2);
            else
                p = mp_encode_uint(p, d.i2);
            CHECK_MEM
            p = mp_encode_bool(p, d.b2);
            CHECK_MEM
            p = mp_encode_uint(p, d.ui2);
            CHECK_MEM
            if (d.by2 < 0)
                p = mp_encode_int(p, d.by2);
            else
                p = mp_encode_uint(p, d.by2);
            CHECK_MEM
            if (d.l2 < 0)
                p = mp_encode_int(p, d.l2);
            else
                p = mp_encode_uint(p, d.l2);
            CHECK_MEM
            p = mp_encode_uint(p, d.ub2);
            CHECK_MEM
            p = mp_encode_uint(p, d.ul2);
            CHECK_MEM
            if (d.s2 < 0)
                p = mp_encode_int(p, d.s2);
            else
                p = mp_encode_uint(p, d.s2);
            CHECK_MEM
            p = mp_encode_double(p, d.f2);
            CHECK_MEM
            p = mp_encode_double(p, d.d2);
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ba2, sizeof(d.ba2));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.bya2, sizeof(d.bya2));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.uba2, sizeof(d.uba2));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.sa2, sizeof(d.sa2));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.usa2, sizeof(d.usa2));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ia2, sizeof(d.ia2));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.uia2, sizeof(d.uia2));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.la2, sizeof(d.la2));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ula2, sizeof(d.ula2));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ca2, sizeof(d.ca2));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.fa2, sizeof(d.fa2));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.da2, sizeof(d.da2));
            CHECK_MEM

            p = mp_encode_uint(p, d.us3);
            CHECK_MEM
            if (d.i3 < 0)
                p = mp_encode_int(p, d.i3);
            else
                p = mp_encode_uint(p, d.i3);
            CHECK_MEM
            p = mp_encode_bool(p, d.b3);
            CHECK_MEM
            p = mp_encode_uint(p, d.ui3);
            CHECK_MEM
            if (d.by3 < 0)
                p = mp_encode_int(p, d.by3);
            else
                p = mp_encode_uint(p, d.by3);
            CHECK_MEM
            if (d.l3 < 0)
                p = mp_encode_int(p, d.l3);
            else
                p = mp_encode_uint(p, d.l3);
            CHECK_MEM
            p = mp_encode_uint(p, d.ub3);
            CHECK_MEM
            p = mp_encode_uint(p, d.ul3);
            CHECK_MEM
            if (d.s3 < 0)
                p = mp_encode_int(p, d.s3);
            else
                p = mp_encode_uint(p, d.s3);
            CHECK_MEM
            p = mp_encode_double(p, d.f3);
            CHECK_MEM
            p = mp_encode_double(p, d.d3);
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ba3, sizeof(d.ba3));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.bya3, sizeof(d.bya3));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.uba3, sizeof(d.uba3));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.sa3, sizeof(d.sa3));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.usa3, sizeof(d.usa3));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ia3, sizeof(d.ia3));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.uia3, sizeof(d.uia3));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.la3, sizeof(d.la3));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ula3, sizeof(d.ula3));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.ca3, sizeof(d.ca3));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.fa3, sizeof(d.fa3));
            CHECK_MEM
            p = mp_encode_bin(p, (char*) d.da3, sizeof(d.da3));
            CHECK_MEM
        }
    }

    free(buf);

    printf("%d\n", p - buf);
    return 0;
}
