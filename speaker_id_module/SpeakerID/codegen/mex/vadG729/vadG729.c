/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * vadG729.c
 *
 * Code generation for function 'vadG729'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "vadG729.h"
#include "BiquadFilter.h"
#include "Autocorrelator.h"
#include "LevinsonSolver.h"
#include "ZeroCrossingDetector.h"
#include "power.h"
#include "mod.h"
#include "SystemCore.h"
#include "sum.h"
#include "log10.h"
#include "LPCToLSF.h"

/* Type Definitions */
#ifndef typedef_struct_T
#define typedef_struct_T

typedef struct {
  real32_T window_buffer[320];
  real32_T frm_count;
  real32_T MeanLSF[10];
  real32_T MeanSE;
  real32_T MeanSLE;
  real32_T MeanE;
  real32_T MeanSZC;
  real32_T count_sil;
  real32_T count_update;
  real32_T count_ext;
  real32_T less_count;
  real32_T flag;
  real32_T prev_markers[2];
  real32_T prev_energy;
  real32_T Prev_Min;
  real32_T Next_Min;
  real32_T Min_buffer[128];
} struct_T;

#endif                                 /*typedef_struct_T*/

/* Variable Definitions */
static dspcodegen_BiquadFilter HPF;
static boolean_T HPF_not_empty;
static dspcodegen_Autocorrelator AC;
static dspcodegen_LevinsonSolver LEV1;
static dspcodegen_LevinsonSolver_1 LEV2;
static dspcodegen_LPCToLSF LPC2LSF;
static dspcodegen_ZeroCrossingDetector ZCD;
static struct_T VAD_var_param;
static emlrtRSInfo emlrtRSI = { 17, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo b_emlrtRSI = { 21, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo c_emlrtRSI = { 28, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo d_emlrtRSI = { 31, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo e_emlrtRSI = { 36, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo f_emlrtRSI = { 39, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo g_emlrtRSI = { 70, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo h_emlrtRSI = { 81, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo i_emlrtRSI = { 84, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo j_emlrtRSI = { 85, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo k_emlrtRSI = { 88, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo l_emlrtRSI = { 95, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo m_emlrtRSI = { 99, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo n_emlrtRSI = { 102, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo o_emlrtRSI = { 106, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

static emlrtRSInfo p_emlrtRSI = { 113, "vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\vadG729.m" };

/* Function Declarations */
static real32_T vad_decision(real32_T dSLE, real32_T dSE, real32_T SD, real32_T
  dSZC);

/* Function Definitions */
static real32_T vad_decision(real32_T dSLE, real32_T dSE, real32_T SD, real32_T
  dSZC)
{
  real32_T dec;

  /*  Active voice decision using multi-boundary decision regions in the space */
  /*  of the 4 difference measures */
  dec = 0.0F;
  if (SD > 0.00175F * dSZC + 0.00085F) {
    dec = 1.0F;
  } else if (SD > -0.00454545487F * dSZC + 0.00115909101F) {
    dec = 1.0F;
  } else if (dSE < -25.0F * dSZC + -5.0F) {
    dec = 1.0F;
  } else if (dSE < 20.0F * dSZC + -6.0F) {
    dec = 1.0F;
  } else if (dSE < -4.7F) {
    dec = 1.0F;
  } else if (dSE < 8800.0F * SD + -12.2F) {
    dec = 1.0F;
  } else if (SD > 0.0009F) {
    dec = 1.0F;
  } else if (dSLE < 25.0F * dSZC + -7.0F) {
    dec = 1.0F;
  } else if (dSLE < -29.09091F * dSZC + -4.8182F) {
    dec = 1.0F;
  } else if (dSLE < -5.3F) {
    dec = 1.0F;
  } else if (dSLE < 14000.0F * SD + -15.5F) {
    dec = 1.0F;
  } else if (dSLE > 0.928571F * dSE + 1.14285F) {
    dec = 1.0F;
  } else if (dSLE < -1.5F * dSE + -9.0F) {
    dec = 1.0F;
  } else {
    if (dSLE < 0.714285F * dSE + -2.14285707F) {
      dec = 1.0F;
    }
  }

  return dec;
}

void AC_not_empty_init(void)
{
}

void HPF_not_empty_init(void)
{
  HPF_not_empty = false;
}

void LEV1_not_empty_init(void)
{
}

void LEV2_not_empty_init(void)
{
}

void LPC2LSF_not_empty_init(void)
{
}

void VAD_var_param_not_empty_init(void)
{
}

void ZCD_not_empty_init(void)
{
}

real32_T vadG729(const emlrtStack *sp, const real_T speech[160])
{
  real_T dv0[160];
  int32_T ixstart;
  real_T speech_hp[160];
  real32_T speech_buf[480];
  real32_T fv0[480];
  real32_T r[13];
  static const real32_T fv1[480] = { 0.0799999908F, 0.0800117627F, 0.080046989F,
    0.0801057592F, 0.0801880136F, 0.0802937523F, 0.0804230049F, 0.080575712F,
    0.0807519F, 0.0809515789F, 0.0811747089F, 0.0814212337F, 0.0816912428F,
    0.0819846466F, 0.0823014453F, 0.0826416686F, 0.0830052271F, 0.0833921507F,
    0.0838023797F, 0.0842359439F, 0.0846927837F, 0.0851728693F, 0.0856762305F,
    0.0862027779F, 0.0867525414F, 0.0873254314F, 0.0879214779F, 0.0885406211F,
    0.0891828F, 0.089848049F, 0.0905363038F, 0.0912475064F, 0.091981627F,
    0.0927386358F, 0.0935185F, 0.0943212F, 0.0951466635F, 0.0959948674F,
    0.096865721F, 0.0977592543F, 0.0986753777F, 0.0996140316F, 0.100575186F,
    0.101558812F, 0.102564819F, 0.103593208F, 0.104643889F, 0.105716772F,
    0.106811889F, 0.107929118F, 0.109068461F, 0.110229798F, 0.111413069F,
    0.112618245F, 0.113845296F, 0.115094103F, 0.116364606F, 0.117656775F,
    0.118970521F, 0.120305754F, 0.121662475F, 0.123040535F, 0.124439932F,
    0.125860527F, 0.127302304F, 0.128765151F, 0.130249009F, 0.131753817F,
    0.133279487F, 0.13482593F, 0.136393085F, 0.137980863F, 0.139589146F,
    0.141217932F, 0.142867073F, 0.14453651F, 0.146226183F, 0.147935942F,
    0.149665728F, 0.151415482F, 0.153185084F, 0.154974446F, 0.156783447F,
    0.158612087F, 0.160460159F, 0.162327692F, 0.164214477F, 0.166120455F,
    0.168045565F, 0.16998969F, 0.17195268F, 0.173934475F, 0.175934955F,
    0.177954093F, 0.179991767F, 0.18204774F, 0.184122011F, 0.186214522F,
    0.188325092F, 0.190453604F, 0.1926F, 0.194764152F, 0.196945921F,
    0.199145243F, 0.201361969F, 0.203595981F, 0.205847219F, 0.208115503F,
    0.210400745F, 0.212702855F, 0.215021625F, 0.217357084F, 0.219709F,
    0.222077236F, 0.224461749F, 0.226862386F, 0.229279F, 0.231711552F,
    0.234159783F, 0.23662366F, 0.239103094F, 0.241597846F, 0.244107857F,
    0.246632978F, 0.24917312F, 0.251728088F, 0.254297763F, 0.256882101F,
    0.259480894F, 0.262093961F, 0.264721304F, 0.267362654F, 0.270017922F,
    0.272687048F, 0.275369823F, 0.278066099F, 0.280775726F, 0.283498675F,
    0.286234677F, 0.288983643F, 0.291745454F, 0.294519931F, 0.297306985F,
    0.300106376F, 0.302918077F, 0.305741906F, 0.308577657F, 0.311425269F,
    0.314284563F, 0.317155391F, 0.320037544F, 0.322931F, 0.325835526F,
    0.328750968F, 0.331677258F, 0.334614187F, 0.337561548F, 0.340519309F,
    0.343487203F, 0.3464652F, 0.349453092F, 0.352450639F, 0.355457872F,
    0.358474433F, 0.361500353F, 0.364535362F, 0.367579371F, 0.370632172F,
    0.373693526F, 0.376763463F, 0.379841775F, 0.382928252F, 0.386022747F,
    0.389125109F, 0.39223516F, 0.395352721F, 0.398477763F, 0.401610017F,
    0.404749334F, 0.407895505F, 0.411048502F, 0.414208084F, 0.417374F,
    0.420546293F, 0.423724622F, 0.42690891F, 0.430099F, 0.433294713F,
    0.43649587F, 0.439702213F, 0.442913771F, 0.446130306F, 0.449351639F,
    0.452577591F, 0.455807954F, 0.459042698F, 0.462281495F, 0.465524346F,
    0.468770981F, 0.472021192F, 0.47527495F, 0.478532016F, 0.481792212F,
    0.485055327F, 0.488321275F, 0.491589874F, 0.494860977F, 0.498134375F,
    0.501409948F, 0.504687309F, 0.507966638F, 0.511247635F, 0.514530063F,
    0.517813742F, 0.521098614F, 0.524384379F, 0.527671F, 0.530958235F,
    0.534245908F, 0.537533939F, 0.540822F, 0.544110119F, 0.547398F, 0.550685465F,
    0.553972423F, 0.557258606F, 0.560543954F, 0.56382829F, 0.567111373F,
    0.570393085F, 0.573673129F, 0.576951563F, 0.58022809F, 0.583502591F,
    0.586774886F, 0.590044737F, 0.593312F, 0.596576571F, 0.599838257F,
    0.603096962F, 0.606352389F, 0.609604359F, 0.612852812F, 0.616097569F,
    0.619338393F, 0.622575223F, 0.625807762F, 0.629036F, 0.632259667F,
    0.635478616F, 0.638692677F, 0.641901612F, 0.645105422F, 0.648303866F,
    0.651496768F, 0.654684F, 0.657865345F, 0.661040604F, 0.664209723F,
    0.667372525F, 0.670528829F, 0.673678398F, 0.676821232F, 0.679957032F,
    0.68308562F, 0.686207F, 0.689320803F, 0.692427039F, 0.695525527F, 0.698616F,
    0.701698422F, 0.704772472F, 0.707838178F, 0.71089536F, 0.713943779F,
    0.716983259F, 0.720013738F, 0.723035F, 0.72604686F, 0.729049325F, 0.732042F,
    0.735025048F, 0.737998F, 0.740960896F, 0.743913472F, 0.746855617F,
    0.749787211F, 0.752708137F, 0.755618095F, 0.758517206F, 0.761405F,
    0.764281452F, 0.767146468F, 0.770000041F, 0.772841692F, 0.775671482F,
    0.778489292F, 0.781294823F, 0.784088135F, 0.78686887F, 0.789637F,
    0.792392433F, 0.795134962F, 0.797864437F, 0.8005808F, 0.803283691F,
    0.805973232F, 0.808649182F, 0.811311483F, 0.813959777F, 0.816594183F,
    0.819214404F, 0.821820319F, 0.824411869F, 0.826988935F, 0.82955128F,
    0.832098782F, 0.834631503F, 0.837149F, 0.839651465F, 0.842138588F,
    0.844610214F, 0.847066283F, 0.849506736F, 0.851931274F, 0.85434F,
    0.856732488F, 0.859108925F, 0.86146903F, 0.863812745F, 0.866139829F,
    0.868450284F, 0.87074405F, 0.873020768F, 0.875280619F, 0.877523243F,
    0.879748523F, 0.881956577F, 0.884147167F, 0.886320114F, 0.888475418F,
    0.890612841F, 0.892732441F, 0.894834F, 0.896917462F, 0.898982584F,
    0.901029468F, 0.903057814F, 0.905067623F, 0.907058835F, 0.909031153F,
    0.910984695F, 0.912919402F, 0.914834917F, 0.916731358F, 0.918608487F,
    0.920466363F, 0.92230469F, 0.924123466F, 0.925922751F, 0.927702248F,
    0.929461956F, 0.931201696F, 0.932921469F, 0.934621215F, 0.936300695F,
    0.93796F, 0.939599F, 0.941217542F, 0.942815602F, 0.944393039F, 0.945949912F,
    0.947486F, 0.949001193F, 0.950495601F, 0.951968908F, 0.953421295F,
    0.954852402F, 0.95626241F, 0.957651138F, 0.959018528F, 0.96036458F,
    0.961689055F, 0.962992F, 0.964273393F, 0.965533F, 0.966771F, 0.967987061F,
    0.969181299F, 0.970353663F, 0.971504F, 0.972632229F, 0.973738432F,
    0.974822402F, 0.975884259F, 0.976923764F, 0.977941F, 0.978935778F,
    0.979908168F, 0.980858147F, 0.981785536F, 0.982690334F, 0.983572543F,
    0.984432101F, 0.98526895F, 0.986083F, 0.986874282F, 0.987642705F, 0.9883883F,
    0.989111F, 0.989810705F, 0.990487456F, 0.9911412F, 0.991771877F,
    0.992379427F, 0.99296391F, 0.993525267F, 0.994063377F, 0.994578362F,
    0.9950701F, 0.995538533F, 0.995983779F, 0.996405661F, 0.996804237F,
    0.997179449F, 0.997531354F, 0.997859895F, 0.998165F, 0.998446703F,
    0.99870497F, 0.998939812F, 0.99915117F, 0.999339104F, 0.999503553F,
    0.999644578F, 0.999762058F, 0.999856055F, 0.999926567F, 0.999973595F,
    0.999997079F, 1.0F, 0.999219298F, 0.996878445F, 0.992981076F, 0.987533331F,
    0.980543613F, 0.972022891F, 0.961984515F, 0.950444102F, 0.937419653F,
    0.922931552F, 0.907002449F, 0.88965708F, 0.870922685F, 0.850828409F,
    0.829405665F, 0.806687951F, 0.782710671F, 0.757511258F, 0.73112911F,
    0.703605354F, 0.674983F, 0.645306766F, 0.614622951F, 0.5829795F,
    0.550425708F, 0.517012596F, 0.482792258F, 0.447818F, 0.412144542F,
    0.375827551F, 0.338923752F, 0.301490873F, 0.263587236F, 0.225271955F,
    0.186604932F, 0.147646546F, 0.108457617F, 0.069099471F, 0.0296334308F };

  static const real32_T fv2[13] = { 1.0001F, 0.999722481F, 0.998890281F,
    0.99750489F, 0.995568514F, 0.993084431F, 0.990056813F, 0.986490548F,
    0.982391596F, 0.977766693F, 0.972623467F, 0.966970325F, 0.960816443F };

  real32_T a[11];
  real32_T LSF[10];
  real32_T rc[2];
  real32_T mtmp;
  real32_T Ef;
  real32_T b_r[12];
  static const real32_T fv3[12] = { 0.21398823F, 0.14767693F, 0.07018812F,
    0.00980856456F, -0.020159347F, -0.0238827F, -0.0148007618F, -0.00503292168F,
    0.000121413665F, 0.0011935425F, 0.000659087207F, 0.000150157823F };

  real32_T El;
  real32_T b_LSF[10];
  real32_T fv4[10];
  real32_T SD;
  uint32_T A;
  real32_T ZC;
  real32_T Min;
  real32_T fv5[128];
  real32_T hoistedGlobal_Min_buffer[128];
  real32_T marker;
  real32_T NE;
  int32_T v_flag;
  int32_T ix;
  boolean_T exitg1;
  real32_T COEF;
  real32_T COEFZC;
  real32_T COEFSD;
  emlrtStack st;
  st.prev = sp;
  st.tls = sp->tls;

  /*  VADG729 Implement the Voice Activity Detection Algorithm. */
  /*  Note that although G.729 VAD operates on pre-processed speech data, this */
  /*  function is a standalone version, i.e. the pre-processing (highpass */
  /*  filtering and linear predictive analysis) is also included. */
  /*  */
  /*  This function is in support of the 'G.729 Voice Activity Detection' */
  /*  example and may change in a future release */
  /*  Copyright 2015 The MathWorks, Inc. */
  /* % Algorithm Components Initialization */
  if (!HPF_not_empty) {
    /*  Create a IIR digital filter used for pre-processing */
    st.site = &emlrtRSI;
    BiquadFilter_BiquadFilter(&HPF);
    HPF_not_empty = true;

    /*  Create an autocorrelator and set its properties to compute the lags  */
    /*  in the range [0:NP]. */
    st.site = &b_emlrtRSI;
    Autocorrelator_Autocorrelator(&AC);

    /*  Create a Levinson solver which compute the reflection coefficients  */
    /*  from auto-correlation function using the Levinson-Durbin recursion.  */
    /*  The first object is configured to output polynomial coefficients and  */
    /*  the second object is configured to output reflection coefficients. */
    st.site = &c_emlrtRSI;
    LevinsonSolver_LevinsonSolver(&LEV1);
    st.site = &d_emlrtRSI;
    b_LevinsonSolver_LevinsonSolver(&LEV2);

    /*  Create a converter from linear prediction coefficients (LPC) to line  */
    /*  spectral frequencies (LSF) */
    st.site = &e_emlrtRSI;
    LPCToLSF_LPCToLSF(&st, &LPC2LSF);

    /*  Create a zero crossing detector */
    st.site = &f_emlrtRSI;
    c_ZeroCrossingDetector_ZeroCros(&ZCD);

    /*  initialize variable parameters */
    memset(&VAD_var_param.window_buffer[0], 0, 320U * sizeof(real32_T));
    VAD_var_param.frm_count = 0.0F;
    for (ixstart = 0; ixstart < 10; ixstart++) {
      VAD_var_param.MeanLSF[ixstart] = 0.0F;
    }

    VAD_var_param.MeanSE = 0.0F;
    VAD_var_param.MeanSLE = 0.0F;
    VAD_var_param.MeanE = 0.0F;
    VAD_var_param.MeanSZC = 0.0F;
    VAD_var_param.count_sil = 0.0F;
    VAD_var_param.count_update = 0.0F;
    VAD_var_param.count_ext = 0.0F;
    VAD_var_param.less_count = 0.0F;
    VAD_var_param.flag = 1.0F;
    for (ixstart = 0; ixstart < 2; ixstart++) {
      VAD_var_param.prev_markers[ixstart] = 1.0F;
    }

    VAD_var_param.prev_energy = 0.0F;
    VAD_var_param.Prev_Min = ((real32_T)rtInf);
    VAD_var_param.Next_Min = ((real32_T)rtInf);
    for (ixstart = 0; ixstart < 128; ixstart++) {
      VAD_var_param.Min_buffer[ixstart] = ((real32_T)rtInf);
    }
  }

  /* % Constants Initialization */
  VAD_var_param.frm_count++;

  /* % Pre-processing */
  /*  Filter the speech frame: this high-pass filter serves as a precaution  */
  /*  against undesired low-frequency components. */
  for (ixstart = 0; ixstart < 160; ixstart++) {
    dv0[ixstart] = 32768.0 * speech[ixstart];
  }

  st.site = &g_emlrtRSI;
  SystemCore_step(&st, &HPF, dv0, speech_hp);

  /*  Store filtered data to the pre-processed speech buffer */
  memcpy(&speech_buf[0], &VAD_var_param.window_buffer[0], 320U * sizeof(real32_T));
  for (ixstart = 0; ixstart < 160; ixstart++) {
    speech_buf[ixstart + 320] = (real32_T)speech_hp[ixstart];
  }

  /*  LPC analysis */
  /*  Windowing of signal */
  /*  Autocorrelation */
  for (ixstart = 0; ixstart < 480; ixstart++) {
    fv0[ixstart] = fv1[ixstart] * speech_buf[ixstart];
  }

  st.site = &h_emlrtRSI;
  b_SystemCore_step(&st, &AC, fv0, r);
  for (ixstart = 0; ixstart < 13; ixstart++) {
    r[ixstart] *= fv2[ixstart];
  }

  /*  LSF */
  st.site = &i_emlrtRSI;
  c_SystemCore_step(&st, &LEV1, *(real32_T (*)[11])&r[0], a);
  st.site = &j_emlrtRSI;
  d_SystemCore_step(&st, &LPC2LSF, a, LSF);
  for (ixstart = 0; ixstart < 10; ixstart++) {
    LSF[ixstart] /= 6.28318548F;
  }

  /*  Reflection coefficients */
  st.site = &k_emlrtRSI;
  e_SystemCore_step(&st, &LEV2, *(real32_T (*)[3])&r[0], rc);

  /* % VAD starts here */
  /* % Parameters extraction */
  /*  Full-band energy */
  mtmp = r[0] / 480.0F;
  st.site = &l_emlrtRSI;
  b_log10(&st, &mtmp);
  Ef = 10.0F * mtmp;

  /*  Low-band energy */
  for (ixstart = 0; ixstart < 12; ixstart++) {
    b_r[ixstart] = r[ixstart + 1] * fv3[ixstart];
  }

  mtmp = (r[0] * 0.24017939F + 2.0F * sum(b_r)) / 480.0F;
  st.site = &m_emlrtRSI;
  b_log10(&st, &mtmp);
  El = 10.0F * mtmp;

  /*  Spectral Distorsion */
  for (ixstart = 0; ixstart < 10; ixstart++) {
    b_LSF[ixstart] = LSF[ixstart] - VAD_var_param.MeanLSF[ixstart];
  }

  st.site = &n_emlrtRSI;
  power(b_LSF, fv4);
  SD = b_sum(fv4);

  /*  Zero-crossing rate */
  st.site = &o_emlrtRSI;
  A = f_SystemCore_step(&st, &ZCD, *(real32_T (*)[161])&speech_buf[280]);
  ZC = (real32_T)A / 160.0F;

  /*  Long-term minimum energy */
  VAD_var_param.Next_Min = muSingleScalarMin(Ef, VAD_var_param.Next_Min);
  Min = muSingleScalarMin(VAD_var_param.Prev_Min, VAD_var_param.Next_Min);
  if (b_mod(VAD_var_param.frm_count, 8.0F) == 0.0F) {
    memcpy(&fv5[0], &VAD_var_param.Min_buffer[1], 127U * sizeof(real32_T));
    fv5[127] = VAD_var_param.Next_Min;
    for (ixstart = 0; ixstart < 128; ixstart++) {
      VAD_var_param.Min_buffer[ixstart] = fv5[ixstart];
      hoistedGlobal_Min_buffer[ixstart] = VAD_var_param.Min_buffer[ixstart];
    }

    st.site = &p_emlrtRSI;
    ixstart = 1;
    mtmp = hoistedGlobal_Min_buffer[0];
    if (muSingleScalarIsNaN(hoistedGlobal_Min_buffer[0])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix < 129)) {
        ixstart = ix;
        if (!muSingleScalarIsNaN(hoistedGlobal_Min_buffer[ix - 1])) {
          mtmp = hoistedGlobal_Min_buffer[ix - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < 128) {
      while (ixstart + 1 < 129) {
        if (hoistedGlobal_Min_buffer[ixstart] < mtmp) {
          mtmp = hoistedGlobal_Min_buffer[ixstart];
        }

        ixstart++;
      }
    }

    VAD_var_param.Prev_Min = mtmp;
    VAD_var_param.Next_Min = ((real32_T)rtInf);
  }

  if (VAD_var_param.frm_count < 32.0F) {
    /*     %% Initialization of running averages if frame number is less than 32 */
    if (Ef < 21.0F) {
      VAD_var_param.less_count++;
      marker = 0.0F;
    } else {
      /*  include only the frames that have an energy Ef greater than 21 */
      marker = 1.0F;
      NE = (VAD_var_param.frm_count - 1.0F) - VAD_var_param.less_count;
      VAD_var_param.MeanE = (VAD_var_param.MeanE * NE + Ef) / (NE + 1.0F);
      VAD_var_param.MeanSZC = (VAD_var_param.MeanSZC * NE + ZC) / (NE + 1.0F);
      for (ixstart = 0; ixstart < 10; ixstart++) {
        VAD_var_param.MeanLSF[ixstart] = (VAD_var_param.MeanLSF[ixstart] * NE +
          LSF[ixstart]) / (NE + 1.0F);
      }
    }
  } else {
    /*     %% Start calculating the chararcteristic energies of background noise */
    if (VAD_var_param.frm_count == 32.0F) {
      VAD_var_param.MeanSE = VAD_var_param.MeanE - 10.0F;
      VAD_var_param.MeanSLE = VAD_var_param.MeanE - 12.0F;
    }

    /*  Difference measures between current frame parameters and running */
    /*  averages of background noise characteristics */
    /*     %% Initial VAD decision */
    if (Ef < 21.0F) {
      marker = 0.0F;
    } else {
      marker = vad_decision(VAD_var_param.MeanSLE - El, VAD_var_param.MeanSE -
                            Ef, SD, VAD_var_param.MeanSZC - ZC);
    }

    v_flag = 0;

    /*     %% Voice activity decision smoothing */
    /*  from energy considerations and neighbouring past frame decisions */
    /*  Step 1 */
    if ((VAD_var_param.prev_markers[0] == 1.0F) && (marker == 0.0F) && (Ef >
         VAD_var_param.MeanSE + 2.0F) && (Ef > 21.0F)) {
      marker = 1.0F;
      v_flag = 1;
    }

    /*  Step 2 */
    if (VAD_var_param.flag == 1.0F) {
      if ((VAD_var_param.prev_markers[1] == 1.0F) &&
          (VAD_var_param.prev_markers[0] == 1.0F) && (marker == 0.0F) &&
          (muSingleScalarAbs(Ef - VAD_var_param.prev_energy) <= 3.0F)) {
        VAD_var_param.count_ext++;
        marker = 1.0F;
        v_flag = 1;
        if (VAD_var_param.count_ext <= 4.0F) {
          VAD_var_param.flag = 1.0F;
        } else {
          VAD_var_param.count_ext = 0.0F;
          VAD_var_param.flag = 0.0F;
        }
      }
    } else {
      VAD_var_param.flag = 1.0F;
    }

    if (marker == 0.0F) {
      VAD_var_param.count_sil++;
    }

    /*  Step 3     */
    if ((marker == 1.0F) && (VAD_var_param.count_sil > 10.0F) && (Ef -
         VAD_var_param.prev_energy <= 3.0F)) {
      marker = 0.0F;
      VAD_var_param.count_sil = 0.0F;
    }

    if (marker == 1.0F) {
      VAD_var_param.count_sil = 0.0F;
    }

    /*  Step 4 */
    if ((Ef < VAD_var_param.MeanSE + 3.0F) && (VAD_var_param.frm_count > 128.0F)
        && (v_flag == 0) && (rc[1] < 0.6)) {
      marker = 0.0F;
    }

    /*     %% Update running averages only in the presence of background noise */
    if ((Ef < VAD_var_param.MeanSE + 3.0F) && (rc[1] < 0.75F) && (SD <
         0.002532959)) {
      VAD_var_param.count_update++;

      /*  Modify update speed coefficients */
      if (VAD_var_param.count_update < 20.0F) {
        COEF = 0.75F;
        COEFZC = 0.8F;
        COEFSD = 0.6F;
      } else if (VAD_var_param.count_update < 30.0F) {
        COEF = 0.95F;
        COEFZC = 0.92F;
        COEFSD = 0.65F;
      } else if (VAD_var_param.count_update < 40.0F) {
        COEF = 0.97F;
        COEFZC = 0.94F;
        COEFSD = 0.7F;
      } else if (VAD_var_param.count_update < 50.0F) {
        COEF = 0.99F;
        COEFZC = 0.96F;
        COEFSD = 0.75F;
      } else if (VAD_var_param.count_update < 60.0F) {
        COEF = 0.995F;
        COEFZC = 0.99F;
        COEFSD = 0.75F;
      } else {
        COEF = 0.995F;
        COEFZC = 0.998F;
        COEFSD = 0.75F;
      }

      /*  Update mean of parameters LSF, SE, SLE, SZC */
      VAD_var_param.MeanSE = COEF * VAD_var_param.MeanSE + (1.0F - COEF) * Ef;
      VAD_var_param.MeanSLE = COEF * VAD_var_param.MeanSLE + (1.0F - COEF) * El;
      VAD_var_param.MeanSZC = COEFZC * VAD_var_param.MeanSZC + (1.0F - COEFZC) *
        ZC;
      for (ixstart = 0; ixstart < 10; ixstart++) {
        VAD_var_param.MeanLSF[ixstart] = COEFSD * VAD_var_param.MeanLSF[ixstart]
          + (1.0F - COEFSD) * LSF[ixstart];
      }
    }

    if (((VAD_var_param.frm_count > 128.0F) && (VAD_var_param.MeanSE < Min) &&
         (SD < 0.002532959)) || (VAD_var_param.MeanSE > Min + 10.0F)) {
      VAD_var_param.MeanSE = Min;
      VAD_var_param.count_update = 0.0F;
    }
  }

  /* % Update parameters for next frame */
  VAD_var_param.prev_energy = Ef;
  mtmp = VAD_var_param.prev_markers[0];
  VAD_var_param.prev_markers[0] = marker;
  VAD_var_param.prev_markers[1] = mtmp;
  memcpy(&VAD_var_param.window_buffer[0], &speech_buf[160], 320U * sizeof
         (real32_T));

  /* % Return final decision */
  return marker;
}

/* End of code generation (vadG729.c) */
