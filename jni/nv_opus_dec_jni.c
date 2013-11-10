#include "nv_opus_dec.h"

#include <jni.h>

// This function must be called before
// any other decoding functions
JNIEXPORT jint JNICALL
Java_com_limelight_nvstream_av_audio_OpusDecoder_init(JNIEnv *env, jobject this) {
	return nv_opus_init();
}

// This function must be called after
// decoding is finished
JNIEXPORT void JNICALL
Java_com_limelight_nvstream_av_audio_OpusDecoder_destroy(JNIEnv *env, jobject this) {
	nv_opus_destroy();
}

// The Opus stream is stereo
JNIEXPORT jint JNICALL
Java_com_limelight_nvstream_av_audio_OpusDecoder_getChannelCount(JNIEnv *env, jobject this) {
	return nv_opus_get_channel_count();
}

// This number assumes 2 channels at 48 KHz
JNIEXPORT jint JNICALL
Java_com_limelight_nvstream_av_audio_OpusDecoder_getMaxOutputShorts(JNIEnv *env, jobject this) {
	return nv_opus_get_max_out_shorts();
}

// The Opus stream is 48 KHz
JNIEXPORT jint JNICALL
Java_com_limelight_nvstream_av_audio_OpusDecoder_getSampleRate(JNIEnv *env, jobject this) {
	return nv_opus_get_sample_rate();
}

// outpcmdata must be 5760*2 shorts in length
// packets must be decoded in order
// a packet loss must call this function with NULL indata and 0 inlen
// returns the number of decoded samples
JNIEXPORT jint JNICALL
Java_com_limelight_nvstream_av_audio_OpusDecoder_decode(
	JNIEnv *env, jobject this, // JNI parameters
	jbyteArray indata, jint inoff, jint inlen, // Input parameters
	jshortArray outpcmdata) // Output parameter
{
	jint ret;
	jbyte* jni_input_data;
	jshort* jni_pcm_data;

	jni_input_data = (*env)->GetByteArrayElements(env, indata, 0);
	jni_pcm_data = (*env)->GetShortArrayElements(env, outpcmdata, 0); 

	ret = nv_opus_decode(&jni_input_data[inoff], inlen, jni_pcm_data);

	// The input data isn't changed so it can be safely aborted
	(*env)->ReleaseByteArrayElements(env, indata, jni_input_data, JNI_ABORT);
	(*env)->ReleaseShortArrayElements(env, outpcmdata, jni_pcm_data, 0);

	return ret;
}
