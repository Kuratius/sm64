

#include "nds.h" 

//#include "floatheader.h"

#define f32 float
#define u32 uint32_t
#define s32 int32_t
#define u64 uint64_t



#ifdef TARGET_NDS
#define NDS_ITCM_CODE __attribute__((section(".itcm")))
#else
#define NDS_ITCM_CODE
#endif




NDS_ITCM_CODE f32 __wrap___aeabi_fdiv(f32 x, f32 y ) {
    union {
    u32 i;
    f32 f;
    }xu;
    xu.f=x;
    union {
    u32 j;
    f32 f;

    }yu;
    yu.f=y;
    u64 i=((xu.i & ((1<<23)-1))|(1<<23));    
    u32 j=((yu.j & ((1<<23)-1))|(1<<23));
    REG_DIVCNT = DIV_64_32; 
    REG_DIV_NUMER = (i<<23);
    REG_DIV_DENOM_L = j;
    s16 shift= i<j ? -1 : 0; 
    u64 m=(xu.i) & (0xff<<23);
    u64 n=(yu.j) & (0xff<<23);
    s32 exponent=m-n+(shift<<23)+(127<<23);
    u32 a =xu.i & (1<<31);//get sign of x
    u32 b =yu.j & (1<<31);//get sign of y
    u32 sign=a^b;
    if ((exponent<=(1<<23))){
    while(REG_DIVCNT & DIV_BUSY);
    return 0.0;
    }
    while(REG_DIVCNT & DIV_BUSY);
    u32 mantissa=REG_DIV_RESULT_L;
    mantissa=shift>=0 ? mantissa>>(shift) : mantissa<<-shift;
    mantissa=mantissa & ((1<<23)-1);
    xu.i= sign | exponent | mantissa;
    return xu.f;
}