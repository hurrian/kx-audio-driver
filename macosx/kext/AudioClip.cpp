//  kX Project audio driver for Mac OS X
//  Created by Eugene Gavrilov.
//	Copyright 2008-2014 Eugene Gavrilov. All rights reserved.
//  www.kxproject.com

/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */


#include "AudioDevice.h"
#include "AudioEngine.h"

#include "IOAudioBlitterLib.h"

#undef debug
#include "cedebug.h"



#ifndef KX_INTERNAL
	#error invalid configuration
#endif

IOReturn kXAudioEngine::clipOutputSamples(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream)
{
	// just some checks
    
	#ifdef DEBUGGING
	int i;
	int n_voice=-1;
	int sz=-1;
	for(i=0;i<KX_NUMBER_OF_VOICES;i++)
	{
		if(hw->voicetable[i].buffer.addr==sampleBuf)
		{
			sz=hw->voicetable[i].buffer.size;
			n_voice=i;
			break;
		}
	}
	if(i==KX_NUMBER_OF_VOICES)
	{
		debug("!! kXAudioEngine[%p]::clipOutputSamples: invalid buffer [%p]\n",this,sampleBuf);
		return 0;
	}
	if(firstSampleFrame+numSampleFrames > (UInt )n_frames || (firstSampleFrame+numSampleFrames)*bps/8 > (UInt )sz)
	{
		debug("!! kXAudioEngine[%p]::clipOutputSamples: invalid offset [buf: %d, %p first: %d num: %d [%04x, %04x]\n",this,i,
			  sampleBuf,
			  firstSampleFrame,numSampleFrames,
			  firstSampleFrame*1*bps/8,
			  (firstSampleFrame+numSampleFrames)*1*bps/8);
		return 0;
	}
	#endif
    
	int frsamples=firstSampleFrame * streamFormat->fNumChannels;
    UInt8 *outputBuf = &(((UInt8 *)sampleBuf)[frsamples * streamFormat->fBitWidth / 8]);
	float *fMixBuf = ((float *)mixBuf) + frsamples;
    
    // Calculate the number of actual samples to convert
    int num_samples = numSampleFrames * streamFormat->fNumChannels;
	#if 1
    //returnSamples = num_samples;
    if(bps==32){
		Float32ToNativeInt32_X86(fMixBuf, (SInt32 *)outputBuf, num_samples);
    }
    else{
		if(bps==16)
			Float32ToNativeInt16_X86(fMixBuf, (SInt16 *)outputBuf, num_samples);
    }
    //returnBuffer = outputBuf;
	#else
	{
		SInt32 *out=(SInt32 *)outputBuf;
		for(int i=0;i<frsamples;i++)
		{
			*out=(SInt32)(*fMixBuf*(float)0x7fffffff);
		}
	}
	#endif
	
    
    /*
    if(!streamFormat)
    {
        return kIOReturnBadArgument;
    }
    UInt32 firstSample = firstSampleFrame * streamFormat->fNumChannels;
    UInt32 numSamples = numSampleFrames * streamFormat->fNumChannels;
    Float32 *floatMixBuf = ((Float32*)mixBuf) + firstSample;
    
    UInt8 *sourceBuf = (UInt8 *) sampleBuf;
    
    // figure out what sort of blit we need to do
    if ((streamFormat->fSampleFormat == kIOAudioStreamSampleFormatLinearPCM) && streamFormat->fIsMixable) {
        // it's mixable linear PCM, which means we will be calling a blitter, which works in samples
        // not frames
        
        if (streamFormat->fNumericRepresentation == kIOAudioStreamNumericRepresentationSignedInt) {
            // it's some kind of signed integer, which we handle as some kind of even byte length
            bool nativeEndianInts;
            nativeEndianInts = (streamFormat->fByteOrder == kIOAudioStreamByteOrderLittleEndian);
            
            SInt16* theOutputBufferSInt16;
            UInt8* theOutputBufferSInt24;
            //SInt8* theOutputBufferSInt8;
            SInt32* theOutputBufferSInt32;
            
            switch (streamFormat->fBitWidth) {
                    
                case 8:
                case 16:
                    theOutputBufferSInt16 = ((SInt16*)sampleBuf) + firstSample;
                    if (nativeEndianInts) {
                            Float32ToNativeInt16_X86(floatMixBuf, theOutputBufferSInt16, numSamples);
                    }
                    else
                        Float32ToSwapInt16_X86(floatMixBuf, (SInt16 *) &sourceBuf[2 * firstSample],
                                           numSamples);
                    break;
                    
                case 20:
                case 24:
                    theOutputBufferSInt24 = (((UInt8*)sampleBuf) + (firstSample * 3));
                    if (nativeEndianInts) {
                        Float32ToNativeInt24_X86(floatMixBuf, theOutputBufferSInt24, numSamples);
                    }
                    else
                        Float32ToSwapInt24_X86(floatMixBuf, &sourceBuf[3 * firstSample], numSamples);
                    break;
                    
                case 32:
                    theOutputBufferSInt32 = ((SInt32*)sampleBuf) + firstSample;
                    if (nativeEndianInts) {
                            Float32ToNativeInt32_X86(floatMixBuf, theOutputBufferSInt32, numSamples);                    }
                    else
                        Float32ToSwapInt32_X86(floatMixBuf, (SInt32 *) &sourceBuf[4 * firstSample],
                                           numSamples);
                    break;
                    
                default:
                    IOLog("clipOutputSamples: can't handle signed integers with a bit width of %d",
                             streamFormat->fBitWidth);
                    break;
                    
            }
        } else if (streamFormat->fNumericRepresentation == kIOAudioStreamNumericRepresentationIEEE754Float) {
            // it is some kind of floating point format
            if ((streamFormat->fBitWidth == 32) && (streamFormat->fBitDepth == 32) &&
                (streamFormat->fByteOrder == kIOAudioStreamByteOrderLittleEndian)) {
                // it's Float32, so we are just going to copy the data
                memcpy(&((Float32 *) sampleBuf)[firstSample], &floatMixBuf[firstSample],
                       numSamples * sizeof (Float32));
            } else
                IOLog("clipOutputSamples: can't handle floats with a bit width of %d, bit depth of %d, "
                         "and/or the given byte order", streamFormat->fBitWidth, streamFormat->fBitDepth);
        }
    } else {
        // it's not linear PCM or it's not mixable, so just copy the data into the target buffer
        UInt32 offset = firstSampleFrame * (streamFormat->fBitWidth / 8) * streamFormat->fNumChannels;
        UInt32 size = numSampleFrames * (streamFormat->fBitWidth / 8) * streamFormat->fNumChannels;
        memcpy(&((SInt8 *) sampleBuf)[offset], &((SInt8 *) mixBuf)[offset], size);
    }
    */
    
    IOLog("Output frames converted");
	return kIOReturnSuccess;
}

IOReturn kXAudioEngine::convertInputSamples(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream)
{
	#ifdef DEBUGGING
	 // just some checks
	int i;
	int sz=-1;
	for(i=0;i<KX_NUMBER_OF_VOICES;i++)
	{
		if(hw->voicetable[i].buffer.addr==sampleBuf)
		{
			sz=hw->voicetable[i].buffer.size;
			break;
		}
	}
	if(i==KX_NUMBER_OF_VOICES)
	{
		debug("!! kXAudioEngine[%p]::convertInputSamples: invalid buffer [%p] audio_stream: %p stream_format: %p\n",this,sampleBuf,audioStream,streamFormat);
		return 0;
	}
	
	if(firstSampleFrame+numSampleFrames>(UInt)n_frames || (firstSampleFrame+numSampleFrames)*bps/8>(UInt)sz)
	{
		debug("!! kXAudioEngine[%p]::convertInputSamples: invalid offset [%p first: %d num: %d [%04x, %04x]\n",this,
			  sampleBuf,
			  firstSampleFrame,numSampleFrames,
			  firstSampleFrame*1*bps/8,
			  (firstSampleFrame+numSampleFrames)*1*bps/8);
		return 0;
	}
	#endif
	
    

	#if 1
   // if (returnBuffer == 0){
        int num_samples = numSampleFrames * streamFormat->fNumChannels;
        UInt8 *inputBuf = &(((UInt8 *)sampleBuf)[firstSampleFrame * streamFormat->fNumChannels * streamFormat->fBitWidth / 8]);
        
        
        if(bps==32)
            NativeInt32ToFloat32_X86((SInt32 *)inputBuf,(Float32 *)destBuf,num_samples);
        else
            if(bps==16)
                NativeInt16ToFloat32_X86((SInt16 *)inputBuf,(Float32 *)destBuf,num_samples);
   /* }else{
            //try to got input recording
            if(bps==32)
                NativeInt32ToFloat32_X86((SInt32 *)returnBuffer,(Float32 *)destBuf,returnSamples);
            else
                if(bps==16)
                    NativeInt16ToFloat32_X86((SInt16 *)returnBuffer,(Float32 *)destBuf,returnSamples);
            
        //returnBuffer = 0;
        //returnSamples = 0;
    }*/
	#endif
    
    IOLog("Input frames converted");
    return kIOReturnSuccess;
}
