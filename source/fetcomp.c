#include <stdint.h>
#include <c6x.h>
#include "sh_params.h"

/* FETCOMP: clean-room 1176-style core, arranged as a leaf/frame-free C674x kernel. */
#define RECIP(x,r) { float _x=(x),_r=_rcpsp(_x); (r)=_r*(2.0f-_x*_r); }
#define EXP2F(x,r) { int _n=(int)(x); float _t; union{uint32_t u;float f;} _s; \
 if((x)<0.0f&&(float)_n>(x))--_n; _t=(x)-(float)_n; \
 _s.u=(uint32_t)(_n+127)<<23; \
 (r)=_s.f*(1.0f+_t*(.69314718f+_t*(.24022651f+_t*(.05550411f+_t*.00961813f)))); }
#define LOG2F(x,r) { union{uint32_t u;float f;} _b; int _e; float _m,_z,_z2,_q; \
 _b.f=(x);_e=(int)((_b.u>>23)&255u)-127;_b.u=(_b.u&0x007fffffu)|0x3f800000u; \
 _m=_b.f;RECIP(_m+1.0f,_q);_z=(_m-1.0f)*_q;_z2=_z*_z; \
 (r)=(float)_e+2.8853900818f*_z*(1.0f+_z2*(.3333333333f+_z2*(.2f+_z2*.1428571429f))); }
#define BIQUAD(x,c,si,y) { float _x=(x),_y=(c)[0]*_x+state[(si)]; \
 state[(si)]=(c)[1]*_x-(c)[3]*_y+state[(si)+1]; \
 state[(si)+1]=(c)[2]*_x-(c)[4]*_y;(y)=_y; }
#define BIQUAD_HP(x,si,y) { float _x=(x),_y=state[20]*_x+state[(si)]; \
 state[(si)]=state[21]*_x-state[23]*_y+state[(si)+1]; \
 state[(si)+1]=state[22]*_x-state[24]*_y;(y)=_y; }

/* Two-biquad fit of the settled detector helper plus its default HF shelf.
   Across 20 Hz..20 kHz: .052 dB RMS and .249 dB worst-case magnitude error. */
/* This fit also absorbs the measured default 10 kHz/-6 dB detector shelf. */
static const float kModelHP[5]={.9978796581f,-1.9957593162f,.9978796581f,-1.9957582643f,.9957603680f};
static const float kModelShelf[5]={1.1389811956f,-1.7857377290f,.6905193673f,-1.8037526861f,.8114856058f};

void SH_AUDIO_FN(void **instance,void **ctx)
{
 const float *coef=(const float*)instance[1];
 float *eff=(float*)ctx[SH_CTX_EFF],*state=(float*)instance[2];
 uint32_t *words=(uint32_t*)state;
 float fade;
 int i;
 unsigned ok=1;

 if(words[SH_STATE_GUARD_WORD]!=SH_STATE_GUARD)ok=0;
 fade=coef[SH_COEFF_BYPASS];if(!(fade>=0&&fade<=1))ok=0;
 for(i=SH_PARAM_IN;i<=SH_PARAM_HPF;i++)if(!(coef[i]>=0&&coef[i]<=1))ok=0;
 if(!ok){for(i=0;i<SH_FRAMES;i++){eff[i]=0;eff[i+SH_CH_B_OFFSET]=0;}return;}

 /* Block constants live in state memory to keep the callback leaf/frame-free.
    state 16/17=time coefficients,18=OUT target,19=ratio slope,
    20..24=the live detector HPF coefficients,25=private init marker,
    26=smoothed HPF knob position. */
 {float x=(-30.0f+60.0f*coef[SH_PARAM_OUT])*.1660964047f;EXP2F(x,state[18]);}
 {float q,x;RECIP((2.0f+78.0f*coef[SH_PARAM_ATACK])*44.1f,q);
  x=-6.6438561898f*q;EXP2F(x,state[16]);}
 {float q,x;RECIP((50.0f+1050.0f*coef[SH_PARAM_RELSE])*44.1f,q);
  x=-6.6438561898f*q;EXP2F(x,state[17]);}
 {float r=coef[SH_PARAM_RATIO];state[19]=r<.16666667f?.75f:(r<.5f?.875f:(r<.8333333f?.91666667f:.95f));}
 if(words[25]!=0x46455432u){state[0]=0;state[1]=state[18];
  state[2]=coef[SH_PARAM_IN];state[3]=coef[SH_PARAM_DRY];
  state[26]=coef[SH_PARAM_HPF];words[25]=0x46455432u;}
 /* Smooth HPF at block rate.  0.05417117 reaches 99% of a knob move in
    approximately 30 ms at 44.1 kHz/16-frame blocks.  Only the detector is
    affected; the settled 20..500 Hz mapping remains exact. */
 state[26]+=.0541711710f*(coef[SH_PARAM_HPF]-state[26]);
 {float k=.00007123792865f*(20.0f+480.0f*state[26]);
  float k2,kq,n;k=k+k*k*k*.3333333333f;k2=k*k;kq=k*1.4142135624f;
  RECIP(1.0f+kq+k2,n);state[20]=n;state[21]=-2.0f*n;state[22]=n;
  state[23]=2.0f*(k2-1.0f)*n;state[24]=(1.0f-kq+k2)*n;}

 for(i=0;i<SH_FRAMES;i++){
  float left=eff[i],right=eff[i+SH_CH_B_OFFSET],x,env,level,threshold,width,gr,gain;
  state[1]+=.002f*(state[18]-state[1]);
  state[2]+=.002f*(coef[SH_PARAM_IN]-state[2]);
  state[3]+=.002f*(coef[SH_PARAM_DRY]-state[3]);

  x=.5f*(left+right);BIQUAD(x,kModelHP,4,x);BIQUAD(x,kModelShelf,6,x);
  x*=.3013507845f;BIQUAD_HP(x,8,x);
  if(x<0)x=-x;env=state[0];
  if(x>env)env=x+state[16]*(env-x);else env=x+state[17]*(env-x);
  state[0]=env;if(env>1.0e-12f){LOG2F(env,level);level*=6.0205999133f;}else level=-240.0f;
  threshold=-60.0f*state[2];width=-.2f*threshold;
  if(width>1.0e-5f&&level>threshold-.5f*width&&level<threshold+.5f*width){
   float d=level-(threshold-.5f*width),q;RECIP(2.0f*width,q);gr=-state[19]*d*d*q;
  }else{gr=(threshold-level)*state[19];if(gr>0)gr=0;}
  x=gr*.1660964047f;EXP2F(x,gain);

  /* The omitted oversampling-off polynomial changes |x|<=1 by at most .01%;
     keeping the linear wet path saves a large fraction of pedal cycles. */
  x=left*gain*state[1]+state[3]*left;
  left+=fade*(x-left);
  x=right*gain*state[1]+state[3]*right;
  right+=fade*(x-right);
  if(!(left>=-7.9f&&left<=7.9f))left=left<0?-7.9f:(left>0?7.9f:0);
  if(!(right>=-7.9f&&right<=7.9f))right=right<0?-7.9f:(right>0?7.9f:0);
  eff[i]=left;eff[i+SH_CH_B_OFFSET]=right;
 }
}
