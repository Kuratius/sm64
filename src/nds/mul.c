
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





NDS_ITCM_CODE f32 __wrap___aeabi_fmul( f32 x, f32 y){
    union {u32 i;f32 f;}xu;
    xu.f=x;
    union {u32 j;f32 f;}yu;
    yu.f=y;

    u32 a =xu.i & (1<<31);//get sign of x
    u32 b =yu.j & (1<<31);//get sign of y

    s32 exponentx=(xu.i ) &( 0xff<<23);
    s32 exponenty=(yu.j) &(0xff<<23);
    if(exponentx==0 || exponenty==0) return 0.0;
    s32 combined_exponent =(exponentx- (127<<23)) +exponenty;

    u32 i=(xu.i & 0x7fffff); //should be explicit bits of mantissa
    u32 j=(yu.j & 0x7fffff);
    //i=i<<4;
    //j=j<<5;
    u32 mantissa=i+j+(((u64)(i<<4)*(u64)(j<<5))>>32);
    mantissa =mantissa +  (1<< 23) ;
    s32 clz=__builtin_clz(mantissa);

    s32 shift=8-clz;

    mantissa= shift>=0 ? mantissa>>shift: mantissa<<-shift;

    mantissa= mantissa & ((1<<23)-1);
    u32 exponent=(combined_exponent+(shift<<23));
    u32 sign= a^b;
    union{
    f32 f;
    u32 k;
    }result;
    result.k= sign |exponent | mantissa;

    return combined_exponent<=(0<<23)? 0.0:result.f ;

}




