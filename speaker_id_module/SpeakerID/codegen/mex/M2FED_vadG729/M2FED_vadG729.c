/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * M2FED_vadG729.c
 *
 * Code generation for function 'M2FED_vadG729'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "M2FED_vadG729.h"
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
  real32_T window_buffer[882];
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
static emlrtRSInfo emlrtRSI = { 17, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo b_emlrtRSI = { 21, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo c_emlrtRSI = { 28, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo d_emlrtRSI = { 31, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo e_emlrtRSI = { 36, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo f_emlrtRSI = { 39, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo g_emlrtRSI = { 70, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo h_emlrtRSI = { 81, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo i_emlrtRSI = { 84, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo j_emlrtRSI = { 85, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo k_emlrtRSI = { 88, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo l_emlrtRSI = { 95, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo m_emlrtRSI = { 99, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo n_emlrtRSI = { 102, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo o_emlrtRSI = { 106, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

static emlrtRSInfo p_emlrtRSI = { 113, "M2FED_vadG729",
  "C:\\Dropbox\\UVa_lab\\SpeakerIDMatlab\\SpeakerID\\G729\\M2FED_vadG729.m" };

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

real32_T M2FED_vadG729(const emlrtStack *sp, const real_T speech[441])
{
  real_T dv0[441];
  int32_T ixstart;
  real_T speech_hp[441];
  real32_T speech_buf[1323];
  real32_T fv0[1323];
  real32_T r[13];
  static const real32_T fv1[1323] = { 0.0799999908F, 0.0800018683F,
    0.0800074711F, 0.0800168F, 0.0800298825F, 0.080046691F, 0.0800672248F,
    0.0800914839F, 0.0801195279F, 0.0801512673F, 0.0801867321F, 0.080225952F,
    0.0802689F, 0.0803155676F, 0.0803659633F, 0.0804201141F, 0.08047802F,
    0.0805396214F, 0.0806049481F, 0.0806740299F, 0.0807468072F, 0.0808233395F,
    0.0809036F, 0.0809875801F, 0.0810752884F, 0.0811667219F, 0.0812618807F,
    0.0813607648F, 0.0814633742F, 0.0815697089F, 0.0816797689F, 0.0817935243F,
    0.0819110349F, 0.0820322707F, 0.0821571723F, 0.0822858587F, 0.0824182108F,
    0.0825542882F, 0.0826940909F, 0.0828376189F, 0.0829848126F, 0.0831357613F,
    0.0832904354F, 0.0834487751F, 0.0836108401F, 0.0837766F, 0.0839460865F,
    0.084119238F, 0.0842961147F, 0.084476687F, 0.0846609846F, 0.084848918F,
    0.0850406066F, 0.0852359608F, 0.0854350105F, 0.0856377855F, 0.0858442F,
    0.0860543F, 0.0862681046F, 0.0864856318F, 0.086706765F, 0.0869316235F,
    0.0871601775F, 0.0873924F, 0.0876282826F, 0.0878678337F, 0.0881110504F,
    0.0883579627F, 0.0886085406F, 0.0888627544F, 0.0891206339F, 0.0893822089F,
    0.0896474198F, 0.0899163F, 0.0901888385F, 0.0904650167F, 0.0907448307F,
    0.0910283104F, 0.0913154259F, 0.0916062072F, 0.0919005945F, 0.0921986476F,
    0.0925003067F, 0.0928056315F, 0.0931145921F, 0.0934271589F, 0.0937433615F,
    0.0940631703F, 0.0943866149F, 0.0947136953F, 0.0950443521F, 0.0953786448F,
    0.0957165435F, 0.0960580483F, 0.0964031592F, 0.0967518762F, 0.0971042F,
    0.0974601284F, 0.0978196338F, 0.0981827155F, 0.0985493734F, 0.0989196673F,
    0.0992935076F, 0.0996709242F, 0.100051917F, 0.100436486F, 0.100824662F,
    0.101216383F, 0.101611622F, 0.102010466F, 0.102412857F, 0.102818795F,
    0.103228278F, 0.103641309F, 0.104057916F, 0.104478009F, 0.104901679F,
    0.105328865F, 0.105759598F, 0.106193848F, 0.106631584F, 0.107072897F,
    0.107517667F, 0.107965983F, 0.108417787F, 0.108873136F, 0.109331943F,
    0.109794267F, 0.110260107F, 0.110729404F, 0.111202188F, 0.111678489F,
    0.112158217F, 0.112641431F, 0.113128133F, 0.113618292F, 0.114111908F,
    0.114608981F, 0.115109541F, 0.115613498F, 0.116120912F, 0.116631813F,
    0.117146112F, 0.117663868F, 0.118185051F, 0.118709631F, 0.119237669F,
    0.119769104F, 0.120303936F, 0.120842196F, 0.121383823F, 0.121928908F,
    0.12247736F, 0.12302921F, 0.123584427F, 0.124143042F, 0.124705024F,
    0.125270382F, 0.125839099F, 0.126411214F, 0.126986638F, 0.127565458F,
    0.128147587F, 0.128733084F, 0.129321918F, 0.12991409F, 0.1305096F,
    0.131108388F, 0.131710544F, 0.132316008F, 0.13292478F, 0.13353686F,
    0.134152219F, 0.134770885F, 0.13539283F, 0.136018082F, 0.136646584F,
    0.137278423F, 0.13791348F, 0.138551816F, 0.139193401F, 0.139838263F,
    0.140486345F, 0.141137674F, 0.141792253F, 0.142450079F, 0.143111125F,
    0.143775389F, 0.144442871F, 0.145113543F, 0.145787463F, 0.146464571F,
    0.147144869F, 0.147828326F, 0.148515031F, 0.149204865F, 0.149897888F,
    0.150594071F, 0.151293412F, 0.151995912F, 0.152701572F, 0.15341039F,
    0.154122338F, 0.154837385F, 0.155555621F, 0.156276926F, 0.157001361F,
    0.157728925F, 0.158459589F, 0.159193322F, 0.159930155F, 0.160670087F,
    0.161413088F, 0.16215919F, 0.162908331F, 0.163660541F, 0.164415821F,
    0.165174142F, 0.165935501F, 0.166699901F, 0.167467341F, 0.16823779F,
    0.16901128F, 0.16978775F, 0.170567229F, 0.171349749F, 0.172135219F,
    0.172923699F, 0.173715129F, 0.17450957F, 0.175306961F, 0.176107332F,
    0.176910654F, 0.177716896F, 0.178526089F, 0.179338232F, 0.180153295F,
    0.18097128F, 0.181792155F, 0.18261601F, 0.183442727F, 0.184272334F,
    0.185104832F, 0.185940191F, 0.186778441F, 0.187619552F, 0.188463554F,
    0.189310387F, 0.190160051F, 0.191012606F, 0.191867962F, 0.19272612F,
    0.193587109F, 0.19445096F, 0.195317551F, 0.196187F, 0.197059199F,
    0.197934195F, 0.198811963F, 0.199692532F, 0.200575843F, 0.201461926F,
    0.202350691F, 0.203242287F, 0.204136565F, 0.205033556F, 0.205933288F,
    0.206835762F, 0.207740918F, 0.208648786F, 0.209559307F, 0.210472539F,
    0.211388424F, 0.212306991F, 0.213228211F, 0.214152113F, 0.215078667F,
    0.216007844F, 0.216939613F, 0.217874035F, 0.21881108F, 0.219750717F,
    0.220692977F, 0.2216378F, 0.222585216F, 0.223535225F, 0.224487796F,
    0.225442901F, 0.226400629F, 0.22736086F, 0.228323624F, 0.229288921F,
    0.230256721F, 0.231227055F, 0.232199952F, 0.233175293F, 0.234153137F,
    0.235133454F, 0.236116245F, 0.23710148F, 0.238089189F, 0.239079371F,
    0.240072F, 0.241067097F, 0.242064551F, 0.243064448F, 0.24406676F,
    0.245071486F, 0.246078566F, 0.24708809F, 0.248099938F, 0.2491142F,
    0.250130802F, 0.251149744F, 0.252171069F, 0.25319469F, 0.254220635F,
    0.255248934F, 0.256279558F, 0.257312447F, 0.25834766F, 0.259385139F,
    0.260424942F, 0.26146695F, 0.262511253F, 0.263557822F, 0.264606565F,
    0.265657634F, 0.266710907F, 0.267766356F, 0.268824071F, 0.26988396F,
    0.270946056F, 0.272010326F, 0.273076773F, 0.274145365F, 0.275216103F,
    0.276289046F, 0.277364105F, 0.27844134F, 0.279520661F, 0.280602068F,
    0.281685621F, 0.28277126F, 0.283858985F, 0.284948796F, 0.286040694F,
    0.287134618F, 0.288230598F, 0.289328665F, 0.290428728F, 0.291530818F,
    0.292634875F, 0.293741047F, 0.294849217F, 0.295959294F, 0.297071427F,
    0.298185468F, 0.299301535F, 0.30041948F, 0.301539451F, 0.3026613F,
    0.303785115F, 0.304910839F, 0.30603838F, 0.307167947F, 0.308299333F,
    0.309432656F, 0.310567796F, 0.311704844F, 0.31284371F, 0.313984454F,
    0.315127045F, 0.316271424F, 0.317417651F, 0.318565637F, 0.31971547F,
    0.320867F, 0.322020411F, 0.32317552F, 0.324332446F, 0.32549113F,
    0.326651514F, 0.327813685F, 0.328977525F, 0.330143124F, 0.331310362F,
    0.332479358F, 0.33365F, 0.334822327F, 0.33599636F, 0.337171942F,
    0.338349283F, 0.339528173F, 0.340708762F, 0.341890931F, 0.343074739F,
    0.344260126F, 0.345447123F, 0.346635699F, 0.347825825F, 0.349017501F,
    0.350210726F, 0.351405561F, 0.352601826F, 0.353799641F, 0.354998946F,
    0.356199831F, 0.357402205F, 0.358606F, 0.359811336F, 0.361018062F,
    0.362226307F, 0.363435924F, 0.364647061F, 0.365859538F, 0.367073506F,
    0.368288845F, 0.369505495F, 0.370723635F, 0.371943086F, 0.373163968F,
    0.374386162F, 0.375609726F, 0.376834571F, 0.378060818F, 0.379288375F,
    0.380517185F, 0.381747335F, 0.382978708F, 0.384211421F, 0.385445356F,
    0.386680514F, 0.387916952F, 0.389154643F, 0.390393585F, 0.391633689F,
    0.392875046F, 0.394117534F, 0.395361274F, 0.396606117F, 0.397852182F,
    0.39909938F, 0.400347739F, 0.401597261F, 0.402847826F, 0.404099524F,
    0.405352294F, 0.406606227F, 0.407861203F, 0.409117311F, 0.410374403F,
    0.411632627F, 0.412891865F, 0.414152116F, 0.415413409F, 0.416675687F,
    0.417939F, 0.419203252F, 0.420468539F, 0.42173472F, 0.423001945F,
    0.424270093F, 0.425539166F, 0.426809192F, 0.428080112F, 0.429352F,
    0.430624694F, 0.431898355F, 0.433172822F, 0.434448242F, 0.435724497F,
    0.437001526F, 0.43827942F, 0.439558148F, 0.440837711F, 0.442118049F,
    0.443399251F, 0.444681138F, 0.445963889F, 0.447247416F, 0.448531598F,
    0.449816614F, 0.451102287F, 0.452388734F, 0.453675836F, 0.454963714F,
    0.456252158F, 0.457541406F, 0.45883131F, 0.46012181F, 0.461413026F,
    0.462704837F, 0.463997304F, 0.465290368F, 0.466584057F, 0.467878312F,
    0.469173193F, 0.47046867F, 0.471764624F, 0.473061204F, 0.474358231F,
    0.475655884F, 0.476954F, 0.478252679F, 0.479551822F, 0.480851501F,
    0.482151687F, 0.48345226F, 0.48475334F, 0.486054838F, 0.487356812F,
    0.488659173F, 0.489961982F, 0.491265118F, 0.492568731F, 0.493872732F,
    0.49517706F, 0.496481836F, 0.49778688F, 0.499092311F, 0.50039804F,
    0.501704156F, 0.503010511F, 0.504317224F, 0.505624175F, 0.506931424F,
    0.508239F, 0.509546638F, 0.510854721F, 0.512163F, 0.513471484F, 0.514780164F,
    0.516089082F, 0.517398238F, 0.518707454F, 0.520016968F, 0.521326542F,
    0.522636354F, 0.523946226F, 0.525256276F, 0.526566446F, 0.527876675F,
    0.529187083F, 0.530497491F, 0.531808078F, 0.533118665F, 0.534429312F,
    0.53574F, 0.537050784F, 0.53836149F, 0.539672315F, 0.54098314F, 0.542293847F,
    0.543604672F, 0.544915259F, 0.546226F, 0.547536612F, 0.548847198F,
    0.550157666F, 0.551468134F, 0.552778482F, 0.554088652F, 0.555398762F,
    0.556708753F, 0.558018625F, 0.559328258F, 0.560637832F, 0.561947227F,
    0.563256383F, 0.56456542F, 0.565874159F, 0.567182779F, 0.568491161F,
    0.569799304F, 0.571107209F, 0.572414875F, 0.573722243F, 0.575029373F,
    0.576336205F, 0.577642739F, 0.578949F, 0.580254853F, 0.581560433F,
    0.582865655F, 0.58417058F, 0.585475087F, 0.586779296F, 0.588083088F,
    0.589386463F, 0.59068954F, 0.59199208F, 0.593294263F, 0.594595969F,
    0.595897317F, 0.597198129F, 0.598498523F, 0.599798381F, 0.601097763F,
    0.602396727F, 0.603695095F, 0.604993F, 0.60629034F, 0.607587218F,
    0.60888344F, 0.610179186F, 0.611474335F, 0.612768888F, 0.614062905F,
    0.615356266F, 0.616649F, 0.617941082F, 0.619232655F, 0.620523453F,
    0.621813715F, 0.623103321F, 0.624392152F, 0.625680327F, 0.626967788F,
    0.628254652F, 0.629540682F, 0.630826056F, 0.632110596F, 0.63339448F,
    0.634677589F, 0.635959864F, 0.637241423F, 0.638522148F, 0.639802158F,
    0.641081274F, 0.642359614F, 0.643637121F, 0.644913793F, 0.64618957F,
    0.647464514F, 0.648738623F, 0.650011778F, 0.651284099F, 0.652555406F,
    0.653825939F, 0.655095458F, 0.656364083F, 0.657631814F, 0.658898473F,
    0.660164237F, 0.661429F, 0.662692845F, 0.663955569F, 0.6652174F,
    0.666478157F, 0.667737901F, 0.668996572F, 0.67025423F, 0.671510875F,
    0.672766387F, 0.674020827F, 0.675274193F, 0.676526487F, 0.677777588F,
    0.679027677F, 0.680276573F, 0.681524396F, 0.682771F, 0.684016466F,
    0.685260773F, 0.686503887F, 0.68774581F, 0.68898654F, 0.690226078F,
    0.691464365F, 0.692701459F, 0.693937302F, 0.695171714F, 0.696405053F,
    0.697637081F, 0.698867917F, 0.700097322F, 0.701325536F, 0.702552438F,
    0.703777969F, 0.705002189F, 0.706225038F, 0.707446635F, 0.708666742F,
    0.709885597F, 0.711102962F, 0.712319F, 0.71353364F, 0.714746892F,
    0.715958714F, 0.717169046F, 0.718378F, 0.719585478F, 0.720791519F,
    0.721996069F, 0.723199189F, 0.724400759F, 0.725600898F, 0.726799488F,
    0.727996469F, 0.729192F, 0.730386F, 0.731578529F, 0.73276943F, 0.733958721F,
    0.735146523F, 0.736332715F, 0.737517357F, 0.73870033F, 0.739881694F,
    0.741061449F, 0.742239594F, 0.743416071F, 0.744590819F, 0.745764F,
    0.746935546F, 0.748105407F, 0.749273539F, 0.750439882F, 0.751604617F,
    0.752767622F, 0.753928959F, 0.755088389F, 0.756246209F, 0.757402241F,
    0.758556545F, 0.75970906F, 0.760859609F, 0.762008548F, 0.763155699F,
    0.764301F, 0.765444398F, 0.766586065F, 0.767725885F, 0.768863916F,
    0.770000041F, 0.771134198F, 0.772266567F, 0.773397F, 0.774525642F,
    0.77565223F, 0.776777F, 0.777899921F, 0.779020786F, 0.780139804F,
    0.781256735F, 0.782371819F, 0.783484936F, 0.784596086F, 0.78570509F,
    0.786812246F, 0.787917435F, 0.789020538F, 0.790121675F, 0.791220546F,
    0.792317629F, 0.793412626F, 0.794505537F, 0.795596302F, 0.7966851F,
    0.797771811F, 0.798856437F, 0.799938917F, 0.801019251F, 0.802097499F,
    0.803173661F, 0.804247677F, 0.805319488F, 0.806389213F, 0.807456732F,
    0.808522105F, 0.809585333F, 0.810646236F, 0.811705053F, 0.812761664F,
    0.813816071F, 0.814868152F, 0.815918088F, 0.816965818F, 0.818011224F,
    0.819054425F, 0.820095301F, 0.821133852F, 0.822170198F, 0.823204279F,
    0.824236F, 0.825265408F, 0.826292574F, 0.827317417F, 0.828339934F, 0.82936F,
    0.830377758F, 0.831393242F, 0.832406342F, 0.833416939F, 0.834425271F,
    0.835431218F, 0.836434722F, 0.837435901F, 0.838434517F, 0.839430809F,
    0.840424657F, 0.841416061F, 0.842404962F, 0.843391478F, 0.844375491F,
    0.84535712F, 0.846336186F, 0.847312748F, 0.848286748F, 0.849258363F,
    0.850227475F, 0.851193964F, 0.85215807F, 0.853119552F, 0.854078591F,
    0.855035F, 0.855988801F, 0.856940091F, 0.857888818F, 0.858835F, 0.859778464F,
    0.860719442F, 0.861657798F, 0.862593532F, 0.863526642F, 0.86445713F,
    0.865384936F, 0.866310179F, 0.8672328F, 0.868152618F, 0.869069874F,
    0.869984448F, 0.870896339F, 0.871805549F, 0.872712F, 0.873615801F,
    0.874516845F, 0.875415266F, 0.876310885F, 0.877203822F, 0.878094F,
    0.878981531F, 0.879866183F, 0.880748034F, 0.881627262F, 0.882503688F,
    0.883377254F, 0.884248F, 0.8851161F, 0.885981321F, 0.886843741F,
    0.887703359F, 0.888560116F, 0.889414072F, 0.890265167F, 0.89111346F,
    0.891958833F, 0.892801344F, 0.893641055F, 0.894477904F, 0.895311832F,
    0.8961429F, 0.896971047F, 0.897796273F, 0.898618639F, 0.899438F,
    0.900254607F, 0.901068211F, 0.901878893F, 0.902686596F, 0.903491318F,
    0.904293239F, 0.90509212F, 0.905888F, 0.906680942F, 0.907470882F,
    0.908257902F, 0.909041882F, 0.909822881F, 0.910600841F, 0.91137588F,
    0.91214788F, 0.91291678F, 0.91368264F, 0.914445579F, 0.915205419F,
    0.915962219F, 0.916716F, 0.91746664F, 0.918214262F, 0.918958783F,
    0.919700265F, 0.920438588F, 0.92117393F, 0.921906114F, 0.922635257F,
    0.923361242F, 0.924084067F, 0.924803853F, 0.925520539F, 0.926234066F,
    0.926944375F, 0.927651644F, 0.928355753F, 0.929056704F, 0.929754496F,
    0.930449F, 0.931140482F, 0.931828737F, 0.932513833F, 0.93319571F,
    0.933874369F, 0.934549928F, 0.935222208F, 0.93589133F, 0.936557174F,
    0.937219799F, 0.937879264F, 0.938535452F, 0.939188361F, 0.939838111F,
    0.940484583F, 0.941127837F, 0.941767752F, 0.942404449F, 0.943037868F,
    0.943668067F, 0.944295F, 0.944918513F, 0.945538878F, 0.946155906F,
    0.946769655F, 0.947380066F, 0.947987139F, 0.948590934F, 0.949191451F,
    0.94978863F, 0.950382411F, 0.950972915F, 0.95156014F, 0.952143967F,
    0.952724397F, 0.953301489F, 0.953875303F, 0.95444572F, 0.955012739F,
    0.95557636F, 0.956136644F, 0.95669359F, 0.957247138F, 0.957797289F,
    0.958344042F, 0.958887398F, 0.959427357F, 0.959963918F, 0.960497F,
    0.961026788F, 0.961553097F, 0.962076F, 0.962595463F, 0.96311146F,
    0.96362406F, 0.964133203F, 0.964638948F, 0.965141177F, 0.96564F,
    0.966135323F, 0.96662724F, 0.9671157F, 0.967600644F, 0.96808213F,
    0.968560159F, 0.969034672F, 0.969505668F, 0.969973207F, 0.970437288F,
    0.970897913F, 0.971354961F, 0.971808493F, 0.972258627F, 0.972705185F,
    0.973148227F, 0.973587751F, 0.974023759F, 0.974456251F, 0.974885166F,
    0.975310624F, 0.975732505F, 0.976150811F, 0.976565659F, 0.976976931F,
    0.977384627F, 0.977788806F, 0.978189409F, 0.978586495F, 0.978979945F,
    0.979369879F, 0.979756236F, 0.980139077F, 0.980518281F, 0.98089385F,
    0.981265903F, 0.981634378F, 0.981999278F, 0.982360601F, 0.982718289F,
    0.9830724F, 0.983422935F, 0.983769834F, 0.984113157F, 0.984452844F,
    0.984788954F, 0.985121429F, 0.985450327F, 0.98577553F, 0.986097217F,
    0.986415207F, 0.986729622F, 0.987040341F, 0.987347484F, 0.987651F,
    0.987950861F, 0.988247097F, 0.988539636F, 0.988828599F, 0.989113927F,
    0.989395559F, 0.989673555F, 0.989947915F, 0.99021858F, 0.990485668F,
    0.990749061F, 0.991008759F, 0.99126482F, 0.991517246F, 0.991766F, 0.992011F,
    0.992252409F, 0.992490113F, 0.99272418F, 0.992954552F, 0.993181229F,
    0.993404269F, 0.993623614F, 0.993839264F, 0.994051218F, 0.994259477F,
    0.994464099F, 0.994664967F, 0.994862199F, 0.995055676F, 0.995245516F,
    0.995431662F, 0.995614111F, 0.995792806F, 0.995967805F, 0.996139109F,
    0.996306777F, 0.99647069F, 0.996630847F, 0.996787369F, 0.996940196F,
    0.997089267F, 0.997234643F, 0.997376263F, 0.997514248F, 0.997648418F,
    0.997778952F, 0.997905731F, 0.998028815F, 0.998148203F, 0.998263836F,
    0.998375714F, 0.998483896F, 0.998588383F, 0.998689175F, 0.998786151F,
    0.998879492F, 0.998969078F, 0.999054909F, 0.999137044F, 0.999215424F,
    0.999290049F, 0.999361F, 0.999428213F, 0.999491692F, 0.999551415F,
    0.999607384F, 0.999659717F, 0.999708235F, 0.999753058F, 0.999794126F,
    0.999831438F, 0.999865055F, 0.999895F, 0.999921083F, 0.999943554F,
    0.999962211F, 0.999977112F, 0.999988317F, 0.999995828F, 0.999999523F, 1.0F,
    0.99997443F, 0.999897838F, 0.999770105F, 0.999591291F, 0.999361396F,
    0.99908042F, 0.998748422F, 0.998365402F, 0.997931361F, 0.997446299F,
    0.996910334F, 0.996323407F, 0.995685518F, 0.994996846F, 0.994257271F,
    0.993466914F, 0.992625773F, 0.991733968F, 0.99079144F, 0.989798307F,
    0.98875463F, 0.987660348F, 0.986515701F, 0.985320568F, 0.984075129F,
    0.982779384F, 0.981433451F, 0.980037391F, 0.978591204F, 0.977095F,
    0.975548923F, 0.973953F, 0.972307324F, 0.97061193F, 0.968866944F,
    0.967072487F, 0.965228558F, 0.963335395F, 0.961392939F, 0.959401369F,
    0.957360804F, 0.955271304F, 0.953133F, 0.950946033F, 0.948710442F,
    0.946426332F, 0.944093943F, 0.941713274F, 0.939284503F, 0.936807692F,
    0.934283078F, 0.93171072F, 0.929090738F, 0.926423252F, 0.923708498F,
    0.920946479F, 0.918137431F, 0.915281475F, 0.912378788F, 0.909429431F,
    0.906433642F, 0.90339148F, 0.900303245F, 0.897168934F, 0.893988788F,
    0.890763044F, 0.887491703F, 0.884175062F, 0.880813181F, 0.877406359F,
    0.873954654F, 0.870458364F, 0.866917551F, 0.86333245F, 0.859703243F,
    0.856030047F, 0.852313161F, 0.848552763F, 0.844749F, 0.84090203F,
    0.837012112F, 0.833079457F, 0.829104185F, 0.825086594F, 0.821026802F,
    0.816925049F, 0.812781632F, 0.808596611F, 0.804370284F, 0.80010289F,
    0.795794606F, 0.791445732F, 0.787056327F, 0.782626629F, 0.778157115F,
    0.773647785F, 0.769098878F, 0.764510691F, 0.759883463F, 0.755217433F,
    0.750512719F, 0.745769739F, 0.740988612F, 0.736169636F, 0.731313109F,
    0.726419091F, 0.721488059F, 0.716520131F, 0.711515546F, 0.706474721F,
    0.701397657F, 0.696284831F, 0.691136479F, 0.685952723F, 0.680734F,
    0.675480425F, 0.670192301F, 0.66487F, 0.659513772F, 0.654123724F,
    0.648700356F, 0.64324379F, 0.63775444F, 0.632232368F, 0.62667805F,
    0.621091783F, 0.615473688F, 0.609824121F, 0.60414356F, 0.598431945F,
    0.592689872F, 0.58691752F, 0.581115127F, 0.575283F, 0.56942153F, 0.563531F,
    0.557611585F, 0.551663697F, 0.545687735F, 0.539683819F, 0.533652306F,
    0.527593493F, 0.521507859F, 0.515395463F, 0.509256721F, 0.503092051F,
    0.496901602F, 0.490685761F, 0.484444827F, 0.478179246F, 0.471889138F,
    0.46557492F, 0.459237F, 0.452875525F, 0.446490914F, 0.440083593F,
    0.433653682F, 0.427201718F, 0.420727819F, 0.414232433F, 0.407715976F,
    0.401178569F, 0.394620776F, 0.388042718F, 0.381444842F, 0.374827474F,
    0.368190944F, 0.361535817F, 0.354862F, 0.348170072F, 0.341460317F,
    0.334733129F, 0.327988863F, 0.321227938F, 0.314450592F, 0.307657063F,
    0.300847799F, 0.294023186F, 0.287183672F, 0.280329347F, 0.273460716F,
    0.266578227F, 0.259682F, 0.252772599F, 0.245850191F, 0.23891522F,
    0.23196803F, 0.225009F, 0.218038693F, 0.211057022F, 0.204064563F,
    0.197061673F, 0.190048724F, 0.18302618F, 0.175994158F, 0.16895327F,
    0.161903635F, 0.154845715F, 0.14778F, 0.140706629F, 0.133626059F,
    0.126538664F, 0.119444929F, 0.112345085F, 0.105239376F, 0.0981283F,
    0.0910122F, 0.0838914588F, 0.0767666623F, 0.0696377084F, 0.0625052F,
    0.0553694926F, 0.0482309572F, 0.0410899594F, 0.0339469798F, 0.026802266F,
    0.019656064F, 0.0125088571F, 0.00536101079F };

  static const real32_T fv2[13] = { 1.0001F, 0.999963462F, 0.999853849F,
    0.999671221F, 0.999415576F, 0.999086916F, 0.998685479F, 0.998211205F,
    0.997664273F, 0.997044742F, 0.996352792F, 0.995588541F, 0.994752228F };

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
    memset(&VAD_var_param.window_buffer[0], 0, 882U * sizeof(real32_T));
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
  for (ixstart = 0; ixstart < 441; ixstart++) {
    dv0[ixstart] = 32768.0 * speech[ixstart];
  }

  st.site = &g_emlrtRSI;
  SystemCore_step(&st, &HPF, dv0, speech_hp);

  /*  Store filtered data to the pre-processed speech buffer */
  memcpy(&speech_buf[0], &VAD_var_param.window_buffer[0], 882U * sizeof(real32_T));
  for (ixstart = 0; ixstart < 441; ixstart++) {
    speech_buf[ixstart + 882] = (real32_T)speech_hp[ixstart];
  }

  /*  LPC analysis */
  /*  Windowing of signal */
  /*  Autocorrelation */
  for (ixstart = 0; ixstart < 1323; ixstart++) {
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
  mtmp = r[0] / 1323.0F;
  st.site = &l_emlrtRSI;
  b_log10(&st, &mtmp);
  Ef = 10.0F * mtmp;

  /*  Low-band energy */
  for (ixstart = 0; ixstart < 12; ixstart++) {
    b_r[ixstart] = r[ixstart + 1] * fv3[ixstart];
  }

  mtmp = (r[0] * 0.24017939F + 2.0F * sum(b_r)) / 1323.0F;
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
  A = f_SystemCore_step(&st, &ZCD, *(real32_T (*)[442])&speech_buf[662]);
  ZC = (real32_T)A / 441.0F;

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
  memcpy(&VAD_var_param.window_buffer[0], &speech_buf[441], 882U * sizeof
         (real32_T));

  /* % Return final decision */
  return marker;
}

void VAD_var_param_not_empty_init(void)
{
}

void ZCD_not_empty_init(void)
{
}

/* End of code generation (M2FED_vadG729.c) */
