#ifdef __cplusplus
    #include <pods/pods.h>
#else
    #include <stdint.h>
    #include <stdlib.h>
#endif

#define Small 15
#define Medium 256
#define Large 32768

#define StructsNum 1024

#define Repeats 128

#define MaxSerializedSize 40 * 1024 * 1024

struct First
{
    int32_t a;
    uint64_t b;
    int64_t c;
    int8_t d;
    uint16_t e;
    int64_t f;
    uint64_t* g;
    int32_t* h;
    int8_t* i;
    int32_t k;
    uint16_t l;

#ifdef __cplusplus
    PODS_SERIALIZABLE(1, PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d),
        PODS_MDR(e), PODS_MDR(f), PODS_MDR_BIN_2(g, Small), PODS_MDR_BIN_2(h, Medium),
        PODS_MDR_BIN_2(i, Large), PODS_MDR(k), PODS_MDR(l))
#endif
};

void initFirst(struct First* data)
{
    data->a = -20;
    data->b = 1024 * 1024;
    data->c = 1024 * 1024 * 1024;
    data->d = 100;
    data->e = 512;
    data->f = -(1024 * 1024);
    data->g = (uint64_t*) malloc(Small * sizeof(uint64_t));
    data->h = (int32_t*) malloc(Medium * sizeof(int32_t));
    data->i = (int8_t*) malloc(Large);
    data->k = -(60 * 1024);
    data->l = 0;
}

void destroyFirst(struct First* data)
{
    free(data->g);
    free(data->h);
    free(data->i);
}

struct Second
{
    int32_t a;
    uint64_t b;
    int64_t c;
    int8_t d;
    uint16_t e;
    int64_t f;
    uint64_t g;
    int32_t h;
    int8_t i;
    int32_t k;
    uint16_t l;

#ifdef __cplusplus
    PODS_SERIALIZABLE(1, PODS_MDR(a), PODS_MDR(b), PODS_MDR(c), PODS_MDR(d),
        PODS_MDR(e), PODS_MDR(f), PODS_MDR(g), PODS_MDR(h), PODS_MDR(i),
        PODS_MDR(k), PODS_MDR(l))
#endif
};

void initSecond(struct Second* data)
{
    data->a = -20;
    data->b = 1024 * 1024;
    data->c = 100l * 1024 * 1024 * 1024;
    data->d = 100;
    data->e = 512;
    data->f = -(1024 * 1024);
    data->g = 64;
    data->h = 8000;
    data->i = -4;
    data->k = -(60 * 1024);
    data->l = 0;
}

void destroySecond(struct Second* data)
{
}

struct Third
{
    struct First first;
    struct Second second;

    #ifdef __cplusplus
        PODS_SERIALIZABLE(1, PODS_MDR(first), PODS_MDR(second))
    #endif
};

void initThird(struct Third* data)
{
    initFirst(&data->first);
    initSecond(&data->second);
}

void destroyThird(struct Third* data)
{
    destroySecond(&data->second);
    destroyFirst(&data->first);
}

struct Data
{
    struct Third third[StructsNum];

    #ifdef __cplusplus
        PODS_SERIALIZABLE(1, PODS_MDR(third))
    #endif
};

void init(struct Data* data)
{
    for (int i = 0; i < StructsNum; ++i)
    {
        initThird(&data->third[i]);
    }
}

void destroy(struct Data* data)
{
    for (int i = 0; i < StructsNum; ++i)
    {
        destroyThird(&data->third[i]);
    }
}
