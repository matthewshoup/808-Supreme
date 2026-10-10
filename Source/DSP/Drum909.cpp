#include "Drum909.h"
#include <cmath>

void Drum909::prepare(double sr){ sampleRate=sr>0.0?sr:44100.0; states={}; }
float Drum909::noise(){ noiseState^=noiseState<<13; noiseState^=noiseState>>17; noiseState^=noiseState<<5; return (float)((noiseState&0x00ffffffu)/8388607.5-1.0); }
float Drum909::osc(double& p,float hz){ p+=juce::MathConstants<double>::twoPi*hz/sampleRate; if(p>=juce::MathConstants<double>::twoPi)p-=juce::MathConstants<double>::twoPi; return std::sin((float)p); }
float Drum909::decay(float s) const { return std::exp(-1.0f/(s*(float)sampleRate)); }
void Drum909::trigger(Voice v,float vel){ auto& s=states[(size_t)v]; s.env=s.aux=1.0f; s.velocity=juce::jlimit(0.0f,1.0f,vel); s.phase1=s.phase2=0.0; }
float Drum909::processSample(){
 float out=0.0f;
 for(size_t i=0;i<states.size();++i){ auto v=(Voice)i; auto& s=states[i]; if(s.env<0.00001f)continue; float x=0.0f,d=0.99f;
  switch(v){
   case Voice::Kick:{ float pitch=52.0f+95.0f*s.aux; x=osc(s.phase1,pitch)*s.env; s.aux*=decay(0.028f); d=decay(0.42f); break; }
   case Voice::Snare:x=(0.38f*osc(s.phase1,185.0f)+0.25f*osc(s.phase2,335.0f)+0.72f*noise())*s.env; d=decay(0.18f); break;
   case Voice::Clap:x=noise()*(s.aux>0.58f?1.0f:0.45f)*s.env; s.aux*=decay(0.05f); d=decay(0.22f); break;
   case Voice::ClosedHat:x=noise()*(0.55f+0.45f*osc(s.phase1,7200.0f))*s.env; d=decay(0.055f); break;
   case Voice::OpenHat:x=noise()*(0.55f+0.45f*osc(s.phase1,7200.0f))*s.env; d=decay(0.48f); break;
   case Voice::LowTom:x=osc(s.phase1,110.0f)*s.env; d=decay(0.30f); break;
   case Voice::MidTom:x=osc(s.phase1,155.0f)*s.env; d=decay(0.27f); break;
   case Voice::HighTom:x=osc(s.phase1,220.0f)*s.env; d=decay(0.23f); break;
   case Voice::Rimshot:x=(0.72f*osc(s.phase1,520.0f)+0.28f*noise())*s.env; d=decay(0.04f); break;
   case Voice::Cowbell:x=(osc(s.phase1,560.0f)>0?0.55f:-0.55f)+(osc(s.phase2,845.0f)>0?0.45f:-0.45f); x*=s.env; d=decay(0.22f); break;
   case Voice::Cymbal:x=(0.58f*noise()+0.42f*osc(s.phase1,5100.0f)*osc(s.phase2,7350.0f))*s.env; d=decay(0.95f); break;
   default:break;
  }
  out+=x*(0.45f+0.55f*s.velocity)*0.32f; s.env*=d;
 }
 return juce::jlimit(-1.0f,1.0f,out);
}