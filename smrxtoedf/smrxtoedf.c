// smrxtoedf.c : file conversion from spike2 to eds data format

#include <stdio.h>
#include <string.h>

#include "edflib.h"
#include "ceds64int.h"

#include "windows.h"

// ----------------------------------------------------------------------------
//definition to access the CEDS64INT dll.

//MATINT_API int S64Open(const char *FileName, const int nFlag);
typedef int(*fS64Open) (const char *, int);

//MATINT_API double S64GetTimeBase(const int nFid);
typedef double(*fS64GetTimeBase) (const int);

//MATINT_API long long S64SecsToTicks(const int nFid, const double dSec);
typedef long long (*fS64SecsToTicks) (const int, const double);

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

//MATINT_API int S64GetChanOffset(const int nFid, const int nChan, double *dOffset);
//tUpto = MATINT_API long long S64ChanMaxTime(const int nFid, const int nChan);

//MATINT_API int S64ReadWave64(const int nFid, const int nChan, double* pData, const int nMax,
//								const long long tFrom, const long long tUpto, long long* tFirst, 
//								const int nMask);
typedef int(*fS64ReadWave64) (const int , const int , double* , const int , 
							  const long long , const long long , long long* , 
							  const int );

// ----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	//input argument handler
	if (argc != 2)
	{
		printf("usage: smrxtoedf <filename>\n");
		return 1;
	}

	char *fin_name = argv[1];

	FILE *file;
	if (!(file = fopen(fin_name, "r")))
	{
		printf("File %s doesn't exist.\n", fin_name);
		fclose(file);
		return 1;
	}
	fclose(file);

	// ------------------------------------------------------------------------------
	//smrx dll setup
	HINSTANCE hInstance = LoadLibrary(L"ceds64int.dll");
	if (hInstance == NULL)
	{
		printf("Failed to load Dll library.\n");
		return 1;
	}
	
	fS64Open S64Open = (fS64Open)GetProcAddress(hInstance, "S64Open");
	if (NULL == S64Open) { printf("Error to get function S64Open.\n"); return -1; }

	fS64GetTimeBase S64GetTimeBase = (fS64GetTimeBase)GetProcAddress(hInstance, "S64GetTimeBase");
	if (NULL == S64GetTimeBase) { printf( "Error to get function S64GetTimeBase.\n"); return -1;}

	fS64SecsToTicks S64SecsToTicks = (fS64SecsToTicks)GetProcAddress(hInstance, "S64SecsToTicks");
	if (NULL == S64SecsToTicks) { printf( "Error to get function S64SecsToTicks.\n"); return -1; }

	fS64MaxTime S64MaxTime = (fS64MaxTime)GetProcAddress(hInstance, "S64MaxTime");
	if (NULL == S64MaxTime) { printf( "Error to get function S64MaxTime.\n"); return -1; }

	fS64MaxChans S64MaxChans = (fS64MaxChans)GetProcAddress(hInstance, "S64MaxChans");
	if (NULL == S64MaxChans) { printf( "Error to get function S64MaxChans.\n"); return -1; }

	fS64GetFreeChan S64GetFreeChan = (fS64GetFreeChan)GetProcAddress(hInstance, "S64GetFreeChan");
	if (NULL == S64GetFreeChan) { printf( "Error to get function S64GetFreeChan.\n"); return -1; }

	fS64ChanType S64ChanType = (fS64ChanType)GetProcAddress(hInstance, "S64ChanType");
	if (NULL == S64ChanType) { printf( "Error to get function S64ChanType.\n"); return -1; }

	fS64ChanMaxTime S64ChanMaxTime = (fS64ChanMaxTime)GetProcAddress(hInstance, "S64ChanMaxTime");
	if (NULL == S64ChanMaxTime) { printf( "Error to get function S64ChanMaxTime.\n"); return -1; }

	fS64GetChanTitle S64GetChanTitle = (fS64GetChanTitle)GetProcAddress(hInstance, "S64GetChanTitle");
	if (NULL == S64GetChanTitle) { printf( "Error to get function S64GetChanTitle.\n"); return -1; }

	fS64GetChanComment S64GetChanComment = (fS64GetChanComment)GetProcAddress(hInstance, "S64GetChanComment");
	if (NULL == S64GetChanComment) { printf( "Error to get function S64GetChanComment.\n"); return -1; }

	fS64ReadWave64 S64ReadWave64 = (fS64ReadWave64)GetProcAddress(hInstance, "S64ReadWave64");
	if (NULL == S64ReadWave64) { printf("Error to get function S64ReadWave64.\n"); return -1; }

	fS64GetIdealRate S64GetIdealRate = (fS64GetIdealRate)GetProcAddress(hInstance, "S64GetIdealRate");
	if (NULL == S64GetIdealRate) { printf("Error to get function S64GetIdealRate.\n"); return -1; }

	// ------------------------------------------------------------------------------
	//smrx file read
	int smrx_fhandle = S64Open(fin_name, 1);
	printf("file open with status %d \n", smrx_fhandle);
	if (smrx_fhandle == -1) return 1;

	//sapmling and time base
	double smrx_samplefrequency = S64GetTimeBase(smrx_fhandle);
	printf("samplefrequency =  %.9g \n", smrx_samplefrequency);
	long long sps_spike = S64SecsToTicks(smrx_fhandle, 1.0);
	printf("sps_spike =  %lld \n", sps_spike);
	int sps = (double) 1 / smrx_samplefrequency;
	//int sps = sps_spike;
	printf("sps_eds=  %d \n", sps);

	long long smrx_maxtime = S64MaxTime(smrx_fhandle);
	printf("max time =  %lld \n", smrx_maxtime);

	// ------------------------------------------------------------------------------
	//number of channels
	int smrx_maxchannels = S64MaxChans(smrx_fhandle);
	printf("smrx_maxchannels =  %d \n", smrx_maxchannels);
	if (smrx_maxchannels == 0) 
	{
		printf("Error with input file");
		return 1;
	}
	
	int smrx_freechannel = S64GetFreeChan(smrx_fhandle);
	printf("smrx_freechannel =  %d \n", smrx_freechannel);

	int smrx_nchannels = smrx_freechannel - 1;
	printf("smrx file contains %d channels with type: \n", smrx_nchannels);
	for (int i = 1; i <= smrx_nchannels ; i++) {
		
		int type = S64ChanType(smrx_fhandle, i);
		char * buff = NULL;
		int title_l = S64GetChanTitle(smrx_fhandle, i, buff,-1);
		buff = (char *)malloc(title_l + 1);
		int aux = S64GetChanTitle(smrx_fhandle, i, buff, title_l);
		
		char * buff2 = NULL;
		int comment_l = S64GetChanComment(smrx_fhandle, i, buff2, -1);
		buff2 = (char *)malloc(comment_l + 1);
		int aux2 = S64GetChanComment(smrx_fhandle, i, buff2, comment_l);
		
		long long chmaxtime = S64ChanMaxTime(smrx_fhandle, i);
		int ch_ideal_rate = S64GetIdealRate(smrx_fhandle, i);


		printf("  channel[%d]: {type=%d, samples=%lld, ideal_rate=%d name=%s ,comment:%s}\n", i, type, chmaxtime, ch_ideal_rate, buff, buff2);

		free(buff);
		free(buff2);
	}
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

	double *buf;
	buf = (double *)malloc(sizeof(double) * smrx_maxtime);//sps_spike);
	long long* tFirst = (long long*) malloc (sizeof (long long ));
	long long* tFirstb = (long long*)malloc(sizeof(long long));

	for (int i = 1; i <= smrx_nchannels; i++) {

		long long chmaxtime = S64ChanMaxTime(smrx_fhandle, i);
		
		int aux = S64ReadWave64(smrx_fhandle, i, buf, chmaxtime, 0, chmaxtime, tFirst, 0);
		
		/*
		//reading chuncks of the channel data
		int aux = S64ReadWave64(smrx_fhandle, i, buf, 1000, 5000, 6000 , tFirst, 0);
		printf(" --> read %d first index = %lld\n",aux, *tFirst);

		int aux2 = S64ReadWave64(smrx_fhandle, i, buf, 10000, 50000, 60000, tFirst, 0);
		printf(" --> read %d first index = %lld\n", aux, *tFirst);
		*/
		
		printf(" --> read %d first index = %lld\n", aux, *tFirst);
	
		//debugging	
		/*
		printf("-----------------------------\n");
		int N = 0;
		printf("Dumping %d items from Channe[%d]\n", N, i);
		for (int j = 0; j < 0; j++) {
			printf(" %.5g ", buf[*tFirst + j]);		
		}
		printf("\n");
		*/
	}

	// dump of signal data 
	/*
	FILE *filePtr;
	filePtr = fopen("dump.out", "w");
	for (int i = 0; i < sps_spike; i++) {
		fprintf(filePtr, "%d   %.5g \n", i, buf[i]);
	}
	*/

	// ------------------------------------------------------------------------------

	//Work - in progress

	/*
	int edf_handle = edfopen_file_writeonly("output.edf", EDFLIB_FILETYPE_EDFPLUS, 1 ); //TODO max chans

	if(edf_handle < 0) {
		printf("error: edfopen_file_writeonly()\n");
		return(1);
	}

	int nchans = 1; 
	for (int i = 0; i < nchans; i++) {

		edf_set_samplefrequency(edf_handle, i, sps); //samplefrequency[i]

		edf_set_physical_maximum(edf_handle, i, (double) 1.0);
		edf_set_physical_minimum(edf_handle, i, (double)-1.0);
		edf_set_digital_maximum(edf_handle, i, 32767);
		if (edf_set_digital_minimum(edf_handle, i, -32767)) 
			printf("error: edf_set_digital_minimum()\n");
	}
	*/

	/*
	double *buf; 
	buf = (double *) malloc(sizeof(double) * samplefrequency);

	int aux = edfwrite_physical_samples(int handle, double *buf);
	
	edfclose_file(edf_handle);
	*/

	FreeLibrary(hInstance);
	return 0;
}
