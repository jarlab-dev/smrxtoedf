//header file to present SMRX to EDF convertion code
// as library to be used in third party apps

#pragma once

#include "edflib.h"
#include "ceds64int.h"

#include "windows.h"
#include "math.h"

// ----------------------------------------------------------------------------
//definition to access the CEDS64INT dll.

//MATINT_API int S64Open(const char *FileName, const int nFlag);
typedef int(*fS64Open) (const char *, int);

//MATINT_API double S64GetTimeBase(const int nFid);
typedef double(*fS64GetTimeBase) (const int);

//MATINT_API long long S64SecsToTicks(const int nFid, const double dSec);
typedef long long(*fS64SecsToTicks) (const int, const double);

//MATINT_API long long S64MaxTime(const int nFid);
typedef long long(*fS64MaxTime) (const int);

//MATINT_API int S64ChanType(const int nFid, const int nChan);
typedef int(*fS64ChanType) (const int, const int);

//MATINT_API int S64MaxChan(const int nFid);
typedef int(*fS64MaxChans) (const int);

//MATINT_API int S64GetFreeChan(const int nFid);
typedef int(*fS64GetFreeChan) (const int);

//MATINT_API int S64GetChanTitle(const int nFid, const int nChan, char *Title, const int nGetSize);
typedef int(*fS64GetChanTitle) (const int, const int, char *, const int);

//MATINT_API int S64GetChanComment(const int nFid, const int nChan, char *Comment, const int nGetSize);
typedef int(*fS64GetChanComment) (const int, const int, char *, const int);

//MATINT_API long long S64ChanMaxTime(const int nFid, const int nChan);
typedef long long(*fS64ChanMaxTime) (const int, const int);

//MATINT_API int S64GetChanUnits(const int nFid, const int nChan, char *Units, const int nGetSize);
typedef int(*fS64GetChanUnits) (const int, const int, char *, const int);

//MATINT_API double S64GetIdealRate(const int nFid, const int nChan);
typedef long long(*fS64GetIdealRate) (const int, const int);

//MATINT_API long long S64ChanDivide(const int nFid, const int nChan);
typedef long long(*fS64ChanDivide) (const int, const int);


//MATINT_API int S64GetChanOffset(const int nFid, const int nChan, double *dOffset);
//tUpto = MATINT_API long long S64ChanMaxTime(const int nFid, const int nChan);

//MATINT_API int S64ReadWave64(const int nFid, const int nChan, double* pData, const int nMax,
//								const long long tFrom, const long long tUpto, long long* tFirst, 
//								const int nMask);
typedef int(*fS64ReadWave64) (const int, const int, double*, const int,
	const long long, const long long, long long*,
	const int);


int smrx2edf(char * fin_name, char* fout_name) {

	// ------------------------------------------------------------------------------
	//smrx dll setup
	HINSTANCE hInstance = LoadLibrary(L"ceds64int.dll");
	if (hInstance == NULL)
	{
		printf("Failed to load Dll library.\n");
		return -1;
	}

	fS64Open S64Open = (fS64Open)GetProcAddress(hInstance, "S64Open");
	if (NULL == S64Open) { printf("Error to get function S64Open.\n"); return -1; }

	fS64GetTimeBase S64GetTimeBase = (fS64GetTimeBase)GetProcAddress(hInstance, "S64GetTimeBase");
	if (NULL == S64GetTimeBase) { printf("Error to get function S64GetTimeBase.\n"); return -1; }

	fS64SecsToTicks S64SecsToTicks = (fS64SecsToTicks)GetProcAddress(hInstance, "S64SecsToTicks");
	if (NULL == S64SecsToTicks) { printf("Error to get function S64SecsToTicks.\n"); return -1; }

	fS64MaxTime S64MaxTime = (fS64MaxTime)GetProcAddress(hInstance, "S64MaxTime");
	if (NULL == S64MaxTime) { printf("Error to get function S64MaxTime.\n"); return -1; }

	fS64MaxChans S64MaxChans = (fS64MaxChans)GetProcAddress(hInstance, "S64MaxChans");
	if (NULL == S64MaxChans) { printf("Error to get function S64MaxChans.\n"); return -1; }

	fS64GetFreeChan S64GetFreeChan = (fS64GetFreeChan)GetProcAddress(hInstance, "S64GetFreeChan");
	if (NULL == S64GetFreeChan) { printf("Error to get function S64GetFreeChan.\n"); return -1; }

	fS64ChanType S64ChanType = (fS64ChanType)GetProcAddress(hInstance, "S64ChanType");
	if (NULL == S64ChanType) { printf("Error to get function S64ChanType.\n"); return -1; }

	fS64ChanMaxTime S64ChanMaxTime = (fS64ChanMaxTime)GetProcAddress(hInstance, "S64ChanMaxTime");
	if (NULL == S64ChanMaxTime) { printf("Error to get function S64ChanMaxTime.\n"); return -1; }

	fS64GetChanTitle S64GetChanTitle = (fS64GetChanTitle)GetProcAddress(hInstance, "S64GetChanTitle");
	if (NULL == S64GetChanTitle) { printf("Error to get function S64GetChanTitle.\n"); return -1; }

	fS64GetChanComment S64GetChanComment = (fS64GetChanComment)GetProcAddress(hInstance, "S64GetChanComment");
	if (NULL == S64GetChanComment) { printf("Error to get function S64GetChanComment.\n"); return -1; }

	fS64ReadWave64 S64ReadWave64 = (fS64ReadWave64)GetProcAddress(hInstance, "S64ReadWave64");
	if (NULL == S64ReadWave64) { printf("Error to get function S64ReadWave64.\n"); return -1; }

	fS64GetIdealRate S64GetIdealRate = (fS64GetIdealRate)GetProcAddress(hInstance, "S64GetIdealRate");
	if (NULL == S64GetIdealRate) { printf("Error to get function S64GetIdealRate.\n"); return -1; }

	fS64ChanDivide S64ChanDivide = (fS64ChanDivide)GetProcAddress(hInstance, "S64ChanDivide");
	if (NULL == S64ChanDivide) { printf("Error to get function S64ChanDivide.\n"); return -1; }

	// ------------------------------------------------------------------------------
	//smrx file read
	int smrx_fhandle = S64Open(fin_name, 1);
	printf("file open with status %d \n", smrx_fhandle);
	if (smrx_fhandle == -1) return -1;

	//number of channels
	int smrx_maxchannels = S64MaxChans(smrx_fhandle);
	printf("max number of channels for file=  %d \n", smrx_maxchannels);
	if (smrx_maxchannels == 0)
	{
		printf("Error with input file");
		return -1;
	}
	int smrx_freechannel = S64GetFreeChan(smrx_fhandle);
	printf("first free channel=  %d \n", smrx_freechannel);
	int smrx_nchannels = smrx_freechannel - 1;
	printf("sampled channels =%d\n", smrx_nchannels);

	//sapmling and time base
	double smrx_samplefrequency = S64GetTimeBase(smrx_fhandle);
	printf("samplefrequency record =  %.9g \n", smrx_samplefrequency);
	long long sps_spike = S64SecsToTicks(smrx_fhandle, 1.0);
	printf("number of tics per second =  %lld \n", sps_spike);
	
	long long smrx_maxtime = S64MaxTime(smrx_fhandle);
	printf("max tics =  %lld \n", smrx_maxtime);

	long long *ch_maxtime = (long long*)malloc(sizeof(long long) * smrx_nchannels);
	
	int global_div = 0; 

	char ** ch_labels = (char **) malloc (sizeof(char*) * smrx_nchannels);

	for (int i = 0; i < smrx_nchannels; i++) {

		int type = S64ChanType(smrx_fhandle, i+1);
		
		int title_l = S64GetChanTitle(smrx_fhandle, i+1, &ch_labels[i], -1);
		ch_labels[i] = (char*) malloc(title_l + 1);
		int aux = S64GetChanTitle(smrx_fhandle, i+1, &ch_labels[i], title_l);

		char * buff2 = NULL;
		int comment_l = S64GetChanComment(smrx_fhandle, i+1, buff2, -1);
		buff2 = (char *)malloc(comment_l + 1);
		int aux2 = S64GetChanComment(smrx_fhandle, i+1, buff2, comment_l);

		ch_maxtime[i] = S64ChanMaxTime(smrx_fhandle, i+1);
		int ch_div = S64ChanDivide(smrx_fhandle, i+1);
		if (i == 0) global_div = ch_div;
		if (global_div != ch_div) {
			printf("** ERROR **: unhandled event, ch_div is different between channels\n");
			return -1;
		}

		printf("  channel[%d]: {type=%d, max_tics=%lld, div=%d name=%s ,comment:%s}\n", i, type, ch_maxtime[i], ch_div, &ch_labels[i], buff2);
		
		free(buff2);
	}

	//real samplig frequency 
	//int sps = (double)1 / smrx_samplefrequency;
	//printf("sps_eds=  %d \n", sps);

	long long  real_samples = smrx_maxtime / global_div;
	int real_sps = sps_spike / global_div;
	int real_secs = real_samples / real_sps; 

	printf(" ## real_samples per channel= %lld\n", real_samples ); 
	printf(" ## real_sampling_frequency= %d\n", real_sps);
	printf(" ## real_time_in_secs= %d\n", real_secs);

	// ------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------

	/*
		param nFid An integer file handle(1 - 8)
		\param nChan The waveform channel to read.This can be either Adc or RealWave.If the type is RealWave we convert
		values to short using the channel scale and offset.This may result in truncated values.
		\param pData The buffer to receive the read data.
		\param nMax The maximum number of values to read.
		\param tFrom This and tUpto define a time range in which to locate the data to read.The first data point will
		be at or after tFrom.
		\param tUpto Not data returned will be at or after this time.
		\param tFirst If any data is returned this is set to the time of the first item.
		\param nMask Integer filter handle Used with AdcMark channels to filter the data.
		\return The number of values returned or a negative error code.
		* /
		MATINT_API int S64ReadWave64(const int nFid, const int nChan, double* pData, const int nMax,
			const long long tFrom, const long long tUpto, long long* tFirst, const int nMask);
	 */

	//TODO: improve approach to handle file sizes of > 4-8GB
	double *buf = (double *) malloc(sizeof(double) * real_samples * smrx_nchannels);
	
	long long* t_first = (long long*) malloc(sizeof(long long) * smrx_nchannels);
	
	double* ch_max = (double*) malloc(sizeof (double) * smrx_nchannels);
	double* ch_min = (double*) malloc(sizeof(double) * smrx_nchannels);

	memset(t_first, 0, smrx_nchannels * sizeof(long long));
	
	for (int i = 0; i < smrx_nchannels; i++) {
		long long t_start = t_first[i];
		int aux = S64ReadWave64(smrx_fhandle, i+1, &buf[i*real_samples], real_samples, t_start, ch_maxtime[i], &t_first[i], 0);

		ch_max[i] = 0;
		ch_min[i] = 0;
		for (int j = 0; j < real_samples; j++) {
			if (buf[i*real_samples + j] > ch_max[i]) ch_max[i] = buf[i*real_samples + j];
			if (buf[i*real_samples + j] < ch_min[i]) ch_min[i] = buf[i*real_samples + j];

		}
		printf("-- > read channel[%d]: #samples=%d, first index = %lld, MAX=  %.5g, MIN= %.5g\n", i + 1, aux, t_first[i],ch_max[i], ch_min[i]);
	}
	
	/*
	//for debugging
	FILE *filePtr;
	filePtr = fopen("dump_smrx_2-10.out", "w");
	for (int i = 0; i < real_sps * 10; i++) {
		fprintf(filePtr, "%d   %.5g \n", i, buf[real_samples * 2 + real_sps * 10 + i]);
	}
	*/ 
	// ------------------------------------------------------------------------------	
	// ------------------------------------------------------------------------------	

	// dump of signal data to EDF+ 
	printf(" ## writing out EDF+ file %s\n",fout_name);
	printf(" ## edf 1 second blocks = %d\n", real_secs);
	printf(" ## edf samples per channel = %d\n", real_secs * real_sps);

	int edf_handle = edfopen_file_writeonly(fout_name, EDFLIB_FILETYPE_EDFPLUS, smrx_nchannels); //TODO max chans

	if(edf_handle < 0) {
		printf("error: edfopen_file_writeonly()\n");
		return(1);
	}

	for (int i = 0; i < smrx_nchannels; i++) {

		if (edf_set_samplefrequency(edf_handle, i, real_sps))
			printf("error: edf_set_samplefrequency()\n");
		
		if (edf_set_physical_maximum(edf_handle, i, (double) ch_max[i]))
			printf("error: edf_set_physical_maximum()\n");
		if (edf_set_physical_minimum(edf_handle, i, (double) ch_min[i]))
			printf("error: edf_set_physical_minimum()\n");
		if (edf_set_digital_maximum(edf_handle, i, 32767))
			printf("error: edf_set_digital_maximum()\n");
		if (edf_set_digital_minimum(edf_handle, i, -32767))
			printf("error: edf_set_digital_minimum()\n");

		if (edf_set_label(edf_handle, i, &ch_labels[i]))
			printf("error: edf_set_label()\n");

	}

	//TODO: handle for non integer sec measurements ... currently we are loosing the last < s

	int *aux_buff = (int *)malloc(sizeof(int) * real_sps);

	for (int j = 0; j < real_secs; j++) {
		for (int i = 0; i < smrx_nchannels; i++) {
			if (edfwrite_physical_samples(edf_handle, &buf[(i*real_samples) + (j*real_sps)])) {
				printf("error: edfwrite_physical_samples()\n");
				return(-1);
			}

			/*
			//in case we require to generate the digital data, but seems redundant

			for (int k = 0; k < real_sps; k++) {
				aux_buff[k] = (buf[(i*real_samples) + (j*real_sps) + k] / max(abs(ch_min[i]), abs(ch_min))) * 32767;
			}

			if (edfwrite_digital_samples(edf_handle, aux_buff))
			{
				printf("error: edfwrite_digital_samples()\n");
				return(-1);
			}
			*/
		}
	}

	edfclose_file(edf_handle);
	FreeLibrary(hInstance);
	//TODO: free buffers

	printf(" ## edf generated successfully\n");

	/*
	//debugging code for testing generateng output
	struct edf_hdr_struct hdr;
	int edf_handle2 = edfopen_file_readonly(fout_name, &hdr, EDFLIB_DO_NOT_READ_ANNOTATIONS); 

	int channel = 22;
	int x = 0; // start reading x seconds from start of file 

	int  hdl = hdr.handle;
	printf("\nlibrary version: %i.%02i\n", edflib_version() / 100, edflib_version() % 100);
	printf("\ngeneral header:\n\n");
	printf("filetype: %i\n", hdr.filetype);
	printf("edfsignals: %i\n", hdr.edfsignals);
	printf("file duration: %I64d seconds\n", hdr.file_duration / EDFLIB_TIME_DIMENSION);
	printf("startdate: %i-%i-%i\n", hdr.startdate_day, hdr.startdate_month, hdr.startdate_year);
	printf("starttime: %i:%02i:%02i\n", hdr.starttime_hour, hdr.starttime_minute, hdr.starttime_second);
	printf("patient: %s\n", hdr.patient);
	printf("recording: %s\n", hdr.recording);
	printf("patientcode: %s\n", hdr.patientcode);
	printf("gender: %s\n", hdr.gender);
	printf("birthdate: %s\n", hdr.birthdate);
	printf("patient_name: %s\n", hdr.patient_name);
	printf("patient_additional: %s\n", hdr.patient_additional);
	printf("admincode: %s\n", hdr.admincode);
	printf("technician: %s\n", hdr.technician);
	printf("equipment: %s\n", hdr.equipment);
	printf("recording_additional: %s\n", hdr.recording_additional);
	printf("datarecord duration: %f seconds\n", ((double)hdr.datarecord_duration) / EDFLIB_TIME_DIMENSION);
	printf("number of datarecords in the file: %I64d\n", hdr.datarecords_in_file);
	printf("number of annotations in the file: %I64d\n", hdr.annotations_in_file);
	printf("\nsignal parameters:\n\n");
	printf("label: %s\n", hdr.signalparam[channel].label);
	printf("samples in file: %I64d\n", hdr.signalparam[channel].smp_in_file);
	printf("samples in datarecord: %i\n", hdr.signalparam[channel].smp_in_datarecord);
	printf("physical maximum: %f\n", hdr.signalparam[channel].phys_max);
	printf("physical minimum: %f\n", hdr.signalparam[channel].phys_min);
	printf("digital maximum: %i\n", hdr.signalparam[channel].dig_max);
	printf("digital minimum: %i\n", hdr.signalparam[channel].dig_min);
	printf("physical dimension: %s\n", hdr.signalparam[channel].physdimension);
	printf("prefilter: %s\n", hdr.signalparam[channel].prefilter);
	printf("transducer: %s\n", hdr.signalparam[channel].transducer);
	printf("samplefrequency: %f\n", ((double)hdr.signalparam[channel].smp_in_datarecord / (double)hdr.datarecord_duration) * EDFLIB_TIME_DIMENSION);


	edfseek(hdl, channel, (long long)((((double)x) / ((double)hdr.file_duration / (double)EDFLIB_TIME_DIMENSION)) * ((double)hdr.signalparam[channel].smp_in_file)), EDFSEEK_SET);

	double *buftest = (double *)malloc(sizeof(double) * real_sps * 10);
	int outn = edfread_physical_samples(hdl, channel, real_sps * 10,  buftest);
	
	if (outn == (-1))
	{
		printf("\nerror: edf_read_physical_samples()\n");
		edfclose_file(hdl);
		free(buf);
		return(1);
	}

	printf("\nread %i samples, started at %i seconds from start of file:\n\n", outn, x);


	FILE *filePtr2;
	filePtr2 = fopen("dump_edf_2-10s.out", "w");
	for (int i = 0; i < real_sps * 10; i++) {
		fprintf(filePtr2, "%d   %.5g \n", i, buftest[i]);
	}
	
	*/
	return 0;
}
