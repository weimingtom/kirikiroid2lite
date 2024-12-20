#pragma once
#include "WaveIntf.h"

class VorbisWaveDecoderCreator : public tTVPWaveDecoderCreator
{
public:
    //VorbisWaveDecoderCreator() { TVPRegisterWaveDecoderCreator(this); }
    tTVPWaveDecoder * Create(const ttstr & storagename, const ttstr & extension);
};

#ifndef USE_OPUS
#define USE_OPUS 0
#endif

#if USE_OPUS
class OpusWaveDecoderCreator : public tTVPWaveDecoderCreator
{
public:
    //VorbisWaveDecoderCreator() { TVPRegisterWaveDecoderCreator(this); }
    tTVPWaveDecoder * Create(const ttstr & storagename, const ttstr & extension);
};
#endif
