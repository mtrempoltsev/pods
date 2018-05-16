#pragma once

#include <pods/pods.h>

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
    PODS_SERIALIZABLE(1,
        PODS_MDR(us0),
        PODS_MDR(i0),
        PODS_MDR(b0),
        PODS_MDR(ui0),
        PODS_MDR(by0),
        PODS_MDR(l0),
        PODS_MDR(ub0),
        PODS_MDR(ul0),
        PODS_MDR(s0),
        PODS_MDR(f0),
        PODS_MDR(d0),
        PODS_MDR_BIN(ba0),
        PODS_MDR_BIN(bya0),
        PODS_MDR_BIN(uba0),
        PODS_MDR_BIN(sa0),
        PODS_MDR_BIN(usa0),
        PODS_MDR_BIN(ia0),
        PODS_MDR_BIN(uia0),
        PODS_MDR_BIN(la0),
        PODS_MDR_BIN(ula0),
        PODS_MDR_BIN(ca0),
        PODS_MDR_BIN(fa0),
        PODS_MDR_BIN(da0),
        PODS_MDR(us1),
        PODS_MDR(i1),
        PODS_MDR(b1),
        PODS_MDR(ui1),
        PODS_MDR(by1),
        PODS_MDR(l1),
        PODS_MDR(ub1),
        PODS_MDR(ul1),
        PODS_MDR(s1),
        PODS_MDR(f1),
        PODS_MDR(d1),
        PODS_MDR_BIN(ba1),
        PODS_MDR_BIN(bya1),
        PODS_MDR_BIN(uba1),
        PODS_MDR_BIN(sa1),
        PODS_MDR_BIN(usa1),
        PODS_MDR_BIN(ia1),
        PODS_MDR_BIN(uia1),
        PODS_MDR_BIN(la1),
        PODS_MDR_BIN(ula1),
        PODS_MDR_BIN(ca1),
        PODS_MDR_BIN(fa1),
        PODS_MDR_BIN(da1),
        PODS_MDR(us2),
        PODS_MDR(i2),
        PODS_MDR(b2),
        PODS_MDR(ui2),
        PODS_MDR(by2),
        PODS_MDR(l2),
        PODS_MDR(ub2),
        PODS_MDR(ul2),
        PODS_MDR(s2),
        PODS_MDR(f2),
        PODS_MDR(d2),
        PODS_MDR_BIN(ba2),
        PODS_MDR_BIN(bya2),
        PODS_MDR_BIN(uba2),
        PODS_MDR_BIN(sa2),
        PODS_MDR_BIN(usa2),
        PODS_MDR_BIN(ia2),
        PODS_MDR_BIN(uia2),
        PODS_MDR_BIN(la2),
        PODS_MDR_BIN(ula2),
        PODS_MDR_BIN(ca2),
        PODS_MDR_BIN(fa2),
        PODS_MDR_BIN(da2),
        PODS_MDR(us3),
        PODS_MDR(i3),
        PODS_MDR(b3),
        PODS_MDR(ui3),
        PODS_MDR(by3),
        PODS_MDR(l3),
        PODS_MDR(ub3),
        PODS_MDR(ul3),
        PODS_MDR(s3),
        PODS_MDR(f3),
        PODS_MDR(d3),
        PODS_MDR_BIN(ba3),
        PODS_MDR_BIN(bya3),
        PODS_MDR_BIN(uba3),
        PODS_MDR_BIN(sa3),
        PODS_MDR_BIN(usa3),
        PODS_MDR_BIN(ia3),
        PODS_MDR_BIN(uia3),
        PODS_MDR_BIN(la3),
        PODS_MDR_BIN(ula3),
        PODS_MDR_BIN(ca3),
        PODS_MDR_BIN(fa3),
        PODS_MDR_BIN(da3))
};
