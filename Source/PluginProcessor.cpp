#include "PluginProcessor.h"
#include "PluginEditor.h"

Supreme808AudioProcessor::Supreme808AudioProcessor()
: AudioProcessor(BusesProperties().withOutput("Output",juce::AudioChannelSet::stereo(),true)),
  parameters(*this,nullptr,"PARAMETERS",createParameterLayout()) {}

juce::AudioProcessorValueTreeState::ParameterLayout Supreme808AudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout l;
    l.add(std::make_unique<juce::AudioParameterFloat>("tune","Tune",juce::NormalisableRange<float>(35,90,.1f),55));
    l.add(std::make_unique<juce::AudioParameterFloat>("decay","Decay",juce::NormalisableRange<float>(80,2000,1,.45f),650));
    l.add(std::make_unique<juce::AudioParameterFloat>("level","Level",juce::NormalisableRange<float>(0,1,.001f),.8f));
    return l;
}
void Supreme808AudioProcessor::prepareToPlay(double sr,int){ kick.prepare(sr); drums.prepare(sr); }
bool Supreme808AudioProcessor::isBusesLayoutSupported(const BusesLayout& l) const {
    auto o=l.getMainOutputChannelSet(); return o==juce::AudioChannelSet::mono()||o==juce::AudioChannelSet::stereo();
}
void Supreme808AudioProcessor::triggerNote(int n,float v) {
    using V=Drum808::Voice;
    switch(n) {
        case 36: kick.trigger(v); break; case 38: drums.trigger(V::Snare,v); break;
        case 39: drums.trigger(V::Clap,v); break; case 42: drums.trigger(V::ClosedHat,v); break;
        case 46: drums.trigger(V::OpenHat,v); break; case 41: drums.trigger(V::LowTom,v); break;
        case 45: drums.trigger(V::MidTom,v); break; case 48: drums.trigger(V::HighTom,v); break;
        case 64: drums.trigger(V::LowConga,v); break; case 63: drums.trigger(V::MidConga,v); break;
        case 62: drums.trigger(V::HighConga,v); break; case 56: drums.trigger(V::Cowbell,v); break;
        case 37: drums.trigger(V::Rimshot,v); break; case 75: drums.trigger(V::Claves,v); break;
        case 70: drums.trigger(V::Maracas,v); break; case 49: drums.trigger(V::Cymbal,v); break;
        default: break;
    }
}
void Supreme808AudioProcessor::processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer& m) {
    juce::ScopedNoDenormals nd; b.clear();
    kick.setTune(parameters.getRawParameterValue("tune")->load());
    kick.setDecay(parameters.getRawParameterValue("decay")->load());
    kick.setLevel(parameters.getRawParameterValue("level")->load());
    auto it=m.begin();
    for(int s=0;s<b.getNumSamples();++s) {
        while(it!=m.end()&&it->samplePosition<=s) { auto msg=it->getMessage(); if(msg.isNoteOn()) triggerNote(msg.getNoteNumber(),msg.getFloatVelocity()); ++it; }
        float x=juce::jlimit(-1.0f,1.0f,kick.processSample()+drums.processSample());
        for(int c=0;c<b.getNumChannels();++c) b.setSample(c,s,x);
    }
}
void Supreme808AudioProcessor::getStateInformation(juce::MemoryBlock& d){ if(auto x=parameters.copyState().createXml()) copyXmlToBinary(*x,d); }
void Supreme808AudioProcessor::setStateInformation(const void* d,int n){ if(auto x=getXmlFromBinary(d,n)) if(x->hasTagName(parameters.state.getType())) parameters.replaceState(juce::ValueTree::fromXml(*x)); }
juce::AudioProcessorEditor* Supreme808AudioProcessor::createEditor(){ return new Supreme808AudioProcessorEditor(*this); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){ return new Supreme808AudioProcessor(); }
