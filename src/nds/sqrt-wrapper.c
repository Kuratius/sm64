
#include "nds.h" 
#define f32 float
#define u32 uint32_t
#define s32 int32_t
#define u64 uint64_t



#ifdef TARGET_NDS
#define NDS_ITCM_CODE __attribute__((section(".itcm")))
#else
#define NDS_ITCM_CODE
#endif




NDS_ITCM_CODE f32 __wrap_sqrtf(f32 x){
    union{f32 f; u32 i;}xu;
    xu.f=x;
    u64 mantissa=xu.i & ((1<<23)-1);
    //check if exponent is odd
    //before subtracting 127 exponent was even if it is odd now
    //therefore check if last digit is 0 
    mantissa=(mantissa+(1<<23))<<   ( (( xu.i & (1<<23))==0  )      +23 );
    REG_SQRTCNT = SQRT_64;
    REG_SQRT_PARAM = mantissa;
    u32 raw_exponent= (xu.i & (0xff<<23));
    if (raw_exponent > 0 ) {
    s32 exponent=raw_exponent-(127<<23);
    exponent=exponent>>1; //right shift on negative number depends on compiler
    exponent=((exponent+(127<<23))& (0xff<<23) );
    //fetch async result here
    
    while(REG_SQRTCNT & SQRT_BUSY);
    xu.i=  exponent| (REG_SQRT_RESULT & ((1<<23)-1));
    return xu.f;
    } else{
    while(REG_SQRTCNT & SQRT_BUSY);
    return 0.0;
    };

}
