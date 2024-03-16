
#include "nds_include.h" 

#define f32 float
#define u32 uint32_t
#define s32 int32_t
#define u64 uint64_t

#ifdef TARGET_NDS
#define NDS_ITCM_CODE __attribute__((section(".itcm")))
#else
#define NDS_ITCM_CODE
#endif

NDS_ITCM_CODE f32 fsqrt(f32 x){
    union{f32 f; u32 i;}xu;
    xu.f=x;
    //grab exponent
    s32 exponent= (xu.i & (0xff<<23));
    if(exponent==0)return 0.0;
    exponent=exponent-(127<<23);
    exponent=exponent>>1; //right shift on negative number depends on compiler
    u64 mantissa=xu.i & ((1<<23)-1);
    mantissa=(mantissa+(1<<23))<<23;
    if ((exponent & (1<<22))>0){
    mantissa=mantissa<<1;
    }
    //printf("exponent: %d\n", exponent);
    //printf("mantissa: %ld\n", mantissa);
    u32 new_mantissa= (u32) sqrt64(mantissa); //modify this line to use hardware sqrt

    xu.i= ((exponent+(127<<23))& (0xff<<23) ) | (new_mantissa & ((1<<23)-1));
    return xu.f;
}
