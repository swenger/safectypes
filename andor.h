
// Linux C Interface
#if !defined(__atmcdLXd_h)
#define __atmcdLXd_h

#include "safectypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define at_u16 unsigned short
#ifdef _LP64
#define at_32 int
#define at_u32 unsigned int
#else
#define at_32 long
#define at_u32 unsigned long
#endif

#define at_64 long long
#define at_u64 unsigned long long 
		
// ===================================
// Version Information Definitions
// ===================================

//Version Information Enumeration - NOTE: Increment the count constant below when
//                                       this structure is extended
// Using large numbers to force size to an integer
typedef enum {
  // Using large numbers to force size to an integer
  AT_SDKVersion = 0x40000000,
		AT_DeviceDriverVersion = 0x40000001
} AT_VersionInfoId;
		
// Count of the number of elements in the Version Information Enumeration
// NOTE: Increment when extending enumeration
#define AT_NoOfVersionInfoIds 2

// Minimum recommended length of the Version Info buffer parameter
#define AT_VERSION_INFO_LEN 80
// Minimum recommended length of the Controller Card Model buffer parameter
#define AT_CONTROLLER_CARD_MODEL_LEN 80
// ===================================
		
// ===================================
// DDG Lite Definitions
// ===================================

//Channel enumeration      
typedef enum {
  // Using large numbers to force size to an integer
  AT_DDGLite_ChannelA = 0x40000000,
		AT_DDGLite_ChannelB = 0x40000001,
		AT_DDGLite_ChannelC = 0x40000002
} AT_DDGLiteChannelId;
		
// Control byte flags
#define AT_DDGLite_ControlBit_GlobalEnable   0x01

#define AT_DDGLite_ControlBit_ChannelEnable  0x01
#define AT_DDGLite_ControlBit_FreeRun        0x02
#define AT_DDGLite_ControlBit_DisableOnFrame 0x04
#define AT_DDGLite_ControlBit_RestartOnFire  0x08
#define AT_DDGLite_ControlBit_Invert         0x10
#define AT_DDGLite_ControlBit_EnableOnFire   0x20
// ===================================
		
// ===================================
// USB iStar Definitions
// ===================================

#define AT_DDG_POLARITY_POSITIVE  0
#define AT_DDG_POLARITY_NEGATIVE  1
#define AT_DDG_TERMINATION_50OHMS 0
#define AT_DDG_TERMINATION_HIGHZ  1

#define AT_STEPMODE_CONSTANT      0
#define AT_STEPMODE_EXPONENTIAL   1
#define AT_STEPMODE_LOGARITHMIC   2
#define AT_STEPMODE_LINEAR        3
#define AT_STEPMODE_OFF           100

#define AT_GATEMODE_FIRE_AND_GATE 0
#define AT_GATEMODE_FIRE_ONLY     1
#define AT_GATEMODE_GATE_ONLY     2
#define AT_GATEMODE_CW_ON         3
#define AT_GATEMODE_CW_OFF        4
#define AT_GATEMODE_DDG           5
// ===================================
		
typedef struct ANDORCAPS
{
		at_u32 ulSize;
		at_u32 ulAcqModes;
		at_u32 ulReadModes;
		at_u32 ulTriggerModes;
		at_u32 ulCameraType;
		at_u32 ulPixelMode;
		at_u32 ulSetFunctions;
		at_u32 ulGetFunctions;
		at_u32 ulFeatures;
		at_u32 ulPCICard;
		at_u32 ulEMGainCapability;
		at_u32 ulFTReadModes;
} AndorCapabilities;
	

typedef struct COLORDEMOSAICINFO
{
		int iX;
		int iY;
		int iAlgorithm;
		int iXPhase;
		int iYPhase;
		int iBackground;
} ColorDemosaicInfo;
	

typedef struct WHITEBALANCEINFO
{
		int iSize;
		int iX;
		int iY;
		int iAlgorithm;
		int iROI_left;
		int iROI_right;
		int iROI_top;
		int iROI_bottom;
		int iOperation;
} WhiteBalanceInfo;
	

_returns_(DRV_SUCCESS) unsigned int AbortAcquisition();
_returns_(DRV_SUCCESS) unsigned int CancelWait();
_returns_(DRV_SUCCESS) unsigned int CoolerOFF();
_returns_(DRV_SUCCESS) unsigned int CoolerON();
_returns_(DRV_SUCCESS) unsigned int DemosaicImage(_size_(info.iY,info.iX) unsigned short * grey, _out_ _size_(info.iY,info.iX) unsigned short * red, _out_ _size_(info.iY,info.iX) unsigned short * green, _out_ _size_(info.iY,info.iX) unsigned short * blue, ColorDemosaicInfo * info);
_returns_(DRV_SUCCESS) unsigned int EnableKeepCleans(int iMode);
_returns_(DRV_SUCCESS) unsigned int FreeInternalMemory();
_returns_(DRV_SUCCESS) unsigned int Filter_GetAveragingFactor(_out_ int * averagingFactor);
_returns_(DRV_SUCCESS) unsigned int Filter_GetAveragingFrameCount(_out_ int * frames);
_returns_(DRV_SUCCESS) unsigned int Filter_GetDataAveragingMode(_out_ int * mode);
_returns_(DRV_SUCCESS) unsigned int Filter_GetMode(_out_ unsigned int * mode);
_returns_(DRV_SUCCESS) unsigned int Filter_GetThreshold(_out_ float * threshold);
_returns_(DRV_SUCCESS) unsigned int Filter_SetAveragingFactor(int averagingFactor);
_returns_(DRV_SUCCESS) unsigned int Filter_SetAveragingFrameCount(int frames);
_returns_(DRV_SUCCESS) unsigned int Filter_SetDataAveragingMode(int mode);
_returns_(DRV_SUCCESS) unsigned int Filter_SetMode(unsigned int mode);
_returns_(DRV_SUCCESS) unsigned int Filter_SetThreshold(float threshold);
_returns_(DRV_SUCCESS) unsigned int GetAcquiredData(_out_ _size_(size) at_32 * arr, at_u32 size); // TODO auto size
_returns_(DRV_SUCCESS) unsigned int GetAcquiredData16(_out_ _size_(size) unsigned short * arr, at_u32 size); // TODO auto size
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetAcquiredFloatData(_out_ _size_(size) float * arr, at_u32 size); // TODO auto size
_returns_(DRV_SUCCESS) unsigned int GetAcquisitionProgress(_out_ at_32 * acc, _out_ at_32 * series);
_returns_(DRV_SUCCESS) unsigned int GetAcquisitionTimings(_out_ float * exposure, _out_ float * accumulate, _out_ float * kinetic);
_returns_(DRV_SUCCESS) unsigned int GetAdjustedRingExposureTimes(int inumTimes, _out_ _size_(inumTimes) float * fptimes);
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetAllDMAData(_out_ _size_(size) at_32 * arr, at_u32 size); // TODO auto size
_returns_(DRV_SUCCESS) unsigned int GetAmpDesc(int index, _out_ _string_ _size_(length) char * name, int length);
_returns_(DRV_SUCCESS) unsigned int GetAmpMaxSpeed(int index, _out_ float * speed);
_returns_(DRV_SUCCESS) unsigned int GetAvailableCameras(_out_ at_32 * totalCameras);
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetBackground(_out_ _size_(size) at_32 * arr, at_u32 size); // TODO auto size
_returns_(DRV_SUCCESS) unsigned int GetBaselineClamp(_out_ int * state);
_returns_(DRV_SUCCESS) unsigned int GetBitDepth(int channel, _out_ int * depth);
_returns_(DRV_SUCCESS) unsigned int GetCameraEventStatus(_out_ at_u32 * camStatus);
_returns_(DRV_SUCCESS) unsigned int GetCameraHandle(at_32 cameraIndex, _out_ at_32 * cameraHandle);
_returns_(DRV_SUCCESS) unsigned int GetCameraInformation(int index, _out_ at_32 * information);
_returns_(DRV_SUCCESS) unsigned int GetCameraSerialNumber(_out_ int * number);
_returns_(DRV_SUCCESS) unsigned int GetCapabilities(_out_ AndorCapabilities * caps);
_returns_(DRV_SUCCESS) unsigned int GetControllerCardModel(_out_ _string_ _size_(11) char * controllerCardModel);
_returns_(DRV_SUCCESS) unsigned int GetCountConvertWavelengthRange(_out_ float * minval, _out_ float * maxval);
_returns_(DRV_SUCCESS) unsigned int GetCurrentCamera(_out_ at_32 * cameraHandle);
_returns_(DRV_SUCCESS) unsigned int GetDDGPulse(double wid, double resolution, _out_ double * Delay, _out_ double * Width);
_returns_(DRV_SUCCESS) unsigned int GetDDGIOCFrequency(_out_ double * frequency);
_returns_(DRV_SUCCESS) unsigned int GetDDGIOCNumber(_out_ unsigned long * numberPulses);
_returns_(DRV_SUCCESS) unsigned int GetDDGIOCPulses(_out_ int * pulses);
_returns_(DRV_SUCCESS) unsigned int GetDetector(_out_ int * xpixels, _out_ int * ypixels);
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetDICameraInfo(void * info);
_returns_(DRV_SUCCESS) unsigned int GetDualExposureTimes(_out_ float * exposure1, _out_ float * exposure2);
_returns_(DRV_SUCCESS) unsigned int GetEMCCDGain(_out_ int * gain);
_returns_(DRV_SUCCESS) unsigned int GetEMGainRange(_out_ int * low, _out_ int * high);
_returns_(DRV_SUCCESS) unsigned int GetFastestRecommendedVSSpeed(_out_ int * index, _out_ float * speed);
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetFIFOUsage(_out_ int * FIFOusage);
_returns_(DRV_SUCCESS) unsigned int GetFilterMode(_out_ int * mode);
_returns_(DRV_SUCCESS) unsigned int GetFKExposureTime(_out_ float * time);
_deprecated_ _returns_(DRV_SUCCESS) unsigned int GetFKVShiftSpeed(int index, _out_ int * speed);
_returns_(DRV_SUCCESS) unsigned int GetFKVShiftSpeedF(int index, _out_ float * speed);
_returns_(DRV_SUCCESS) unsigned int GetHardwareVersion(_out_ unsigned int * PCB, _out_ unsigned int * Decode, _out_ unsigned int * dummy1, _out_ unsigned int * dummy2, _out_ unsigned int * CameraFirmwareVersion, _out_ unsigned int * CameraFirmwareBuild);
_returns_(DRV_SUCCESS) unsigned int GetHeadModel(_out_ _string_ _size_(MAX_PATH) char * name);
_returns_(DRV_SUCCESS) unsigned int GetHorizontalSpeed(int index, _out_ int * speed);
_returns_(DRV_SUCCESS) unsigned int GetHSSpeed(int channel, int typ, int index, _out_ float * speed);
_returns_(DRV_SUCCESS) unsigned int GetHVflag(_out_ int * bFlag);
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetID(int devNum, _out_ int * id);
_returns_(DRV_SUCCESS) unsigned int GetImageFlip(_out_ int * iHFlip, _out_ int * iVFlip);
_returns_(DRV_SUCCESS) unsigned int GetImageRotate(_out_ int * iRotate);
_returns_(DRV_SUCCESS) unsigned int GetImages(at_32 first, at_32 last, _out_ _size_(size) at_32 * arr, at_u32 size, _out_ at_32 * validfirst, _out_ at_32 * validlast); // TODO auto size
_returns_(DRV_SUCCESS) unsigned int GetImages16(at_32 first, at_32 last, _out_ _size_(size) unsigned short * arr, at_u32 size, _out_ at_32 * validfirst, _out_ at_32 * validlast); // TODO auto size
_returns_(DRV_SUCCESS) unsigned int GetImagesPerDMA(_out_ at_u32 * images);
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetIRQ(_out_ int * IRQ);
_returns_(DRV_SUCCESS) unsigned int GetKeepCleanTime(_out_ float * KeepCleanTime);
_returns_(DRV_SUCCESS) unsigned int GetMaximumBinning(int ReadMode, int HorzVert, _out_ int * MaxBinning);
_returns_(DRV_SUCCESS) unsigned int GetMaximumExposure(_out_ float * MaxExp);
_returns_(DRV_SUCCESS) unsigned int GetMCPGain(_out_ int * piGain);
_returns_(DRV_SUCCESS) unsigned int GetMCPGainRange(_out_ int * iLow, _out_ int * iHigh);
_returns_(DRV_SUCCESS) unsigned int GetMCPVoltage(_out_ int * iVoltage);
_returns_(DRV_SUCCESS) unsigned int GetMinimumImageLength(_out_ int * MinImageLength);
_returns_(DRV_SUCCESS) unsigned int GetMostRecentColorImage16(at_u32 size, int algorithm, _out_ _size_(size) unsigned short * red, _out_ _size_(size) unsigned short * green, _out_ _size_(size) unsigned short * blue); // TODO auto size
_returns_(DRV_SUCCESS) unsigned int GetMostRecentImage(_out_ _size_(size) at_32 * arr, at_u32 size); // TODO auto size
_returns_(DRV_SUCCESS) unsigned int GetMostRecentImage16(_out_ _size_(size) unsigned short * arr, at_u32 size); // TODO auto size
_deprecated_ _returns_(DRV_SUCCESS) unsigned int GetNewData(_out_ _size_(size) at_32 * arr, at_u32 size); // TODO auto size
_deprecated_ _returns_(DRV_SUCCESS) unsigned int GetNewData16(_out_ _size_(size) unsigned short * arr, at_u32 size); // TODO auto size
_deprecated_ _returns_(DRV_SUCCESS) unsigned int GetNewData8(_out_ _size_(size) unsigned char * arr, at_u32 size); // TODO auto size
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetNewFloatData(_out_ _size_(size) float * arr, at_u32 size); // TODO auto size
_returns_(DRV_SUCCESS) unsigned int GetNumberADChannels(_out_ int * channels);
_returns_(DRV_SUCCESS) unsigned int GetNumberAmp(_out_ int * amp);
_returns_(DRV_SUCCESS) unsigned int GetNumberAvailableImages(_out_ at_32 * first, _out_ at_32 * last);
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetNumberDevices(_out_ int * numDevs);
_returns_(DRV_SUCCESS) unsigned int GetNumberFKVShiftSpeeds(_out_ int * number);
_deprecated_ _returns_(DRV_SUCCESS) unsigned int GetNumberHorizontalSpeeds(_out_ int * number);
_returns_(DRV_SUCCESS) unsigned int GetNumberHSSpeeds(int channel, int typ, _out_ int * speeds);
_returns_(DRV_SUCCESS) unsigned int GetNumberNewImages(_out_ at_32 * first, _out_ at_32 * last);
_returns_(DRV_SUCCESS) unsigned int GetNumberPreAmpGains(_out_ int * noGains);
_returns_(DRV_SUCCESS) unsigned int GetNumberRingExposureTimes(_out_ int * ipnumTimes);
_returns_(DRV_SUCCESS) unsigned int GetNumberIO(_out_ int * iNumber);
_deprecated_ _returns_(DRV_SUCCESS) unsigned int GetNumberVerticalSpeeds(_out_ int * number);
_returns_(DRV_SUCCESS) unsigned int GetNumberVSAmplitudes(_out_ int * number);
_returns_(DRV_SUCCESS) unsigned int GetNumberVSSpeeds(_out_ int * speeds);
_returns_(DRV_SUCCESS) unsigned int GetOldestImage(_out_ _size_(size) at_32 * arr, at_u32 size); // TODO auto size
_returns_(DRV_SUCCESS) unsigned int GetOldestImage16(_out_ _size_(size) unsigned short * arr, at_u32 size); // TODO auto size
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetPhysicalDMAAddress(_out_ at_u32 * Address1, _out_ at_u32 * Address2);
_returns_(DRV_SUCCESS) unsigned int GetPixelSize(_out_ float * xSize, _out_ float * ySize);
_returns_(DRV_SUCCESS) unsigned int GetPreAmpGain(int index, _out_ float * gain);
_returns_(DRV_SUCCESS) unsigned int GetQE(_string_ char * sensor, float wavelength, unsigned int mode, _out_ float * QE);
_returns_(DRV_SUCCESS) unsigned int GetReadOutTime(_out_ float * ReadOutTime);
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetRegisterDump(_out_ int * mode);
_returns_(DRV_SUCCESS) unsigned int GetRingExposureRange(_out_ float * fpMin, _out_ float * fpMax);
_returns_(DRV_SUCCESS) unsigned int GetSensitivity(int channel, int horzShift, int amplifier, int pa, _out_ float * sensitivity);
_returns_(DRV_SUCCESS) unsigned int GetSizeOfCircularBuffer(_out_ at_32 * index);
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetSlotBusDeviceFunction(_out_ at_u32 * dwslot, _out_ at_u32 * dwBus, _out_ at_u32 * dwDevice, _out_ at_u32 * dwFunction);
_returns_(DRV_SUCCESS) unsigned int GetSoftwareVersion(_out_ unsigned int * eprom, _out_ unsigned int * coffile, _out_ unsigned int * vxdrev, _out_ unsigned int * vxdver, _out_ unsigned int * dllrev, _out_ unsigned int * dllver);
_deprecated_ _returns_(DRV_SUCCESS) unsigned int GetSpoolProgress(_out_ at_32 * index);
_returns_(DRV_SUCCESS) unsigned int GetStatus(_out_ int * status);
_returns_(DRV_SUCCESS) unsigned int GetTemperature(_out_ int * temperature);
_returns_(DRV_SUCCESS) unsigned int GetTemperatureF(_out_ float * temperature);
_returns_(DRV_SUCCESS) unsigned int GetTemperatureRange(_out_ int * mintemp, _out_ int * maxtemp);
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetTemperatureStatus(_out_ float * SensorTemp, _out_ float * TargetTemp, _out_ float * AmbientTemp, _out_ float * CoolerVolts);
_returns_(DRV_SUCCESS) unsigned int GetTotalNumberImagesAcquired(_out_ at_32 * index);
_returns_(DRV_SUCCESS) unsigned int GetIODirection(int index, _out_ int * iDirection);
_returns_(DRV_SUCCESS) unsigned int GetIOLevel(int index, _out_ int * iLevel);
_returns_(DRV_SUCCESS) unsigned int GetVersionInfo(AT_VersionInfoId arr, _out_ _string_ _size_(ui32BufferLen) char * szVersionInfo, at_u32 ui32BufferLen);
_deprecated_ _returns_(DRV_SUCCESS) unsigned int GetVerticalSpeed(int index, _out_ int * speed);
_reserved_ _returns_(DRV_SUCCESS) unsigned int GetVirtualDMAAddress(_out_ void ** Address1, _out_ void ** Address2);
_returns_(DRV_SUCCESS) unsigned int GetVSSpeed(int index, _out_ float * speed);
_returns_(DRV_SUCCESS) unsigned int GPIBReceive(int id, short address, _out_ _string_ _size_(size) char * text, int size);
_returns_(DRV_SUCCESS) unsigned int GPIBSend(int id, short address, _string_ char * text);
_returns_(DRV_SUCCESS) unsigned int I2CBurstRead(unsigned char i2cAddress, at_32 nBytes, _out_ _size_(nBytes) unsigned char * data);
_returns_(DRV_SUCCESS) unsigned int I2CBurstWrite(unsigned char i2cAddress, _value_(len(data)) at_32 nBytes, _size_(nBytes) unsigned char * data);
_returns_(DRV_SUCCESS) unsigned int I2CRead(unsigned char deviceID, unsigned char intAddress, _out_ unsigned char * pdata);
_returns_(DRV_SUCCESS) unsigned int I2CReset();
_returns_(DRV_SUCCESS) unsigned int I2CWrite(unsigned char deviceID, unsigned char intAddress, unsigned char data);
_reserved_ _returns_(DRV_SUCCESS) unsigned int IdAndorDll();
_returns_(DRV_SUCCESS) unsigned int InAuxPort(int port, _out_ int * state);
_returns_(DRV_SUCCESS) unsigned int Initialize(_string_ char * dir);
_reserved_ _returns_(DRV_SUCCESS) unsigned int InitializeDevice(_string_ char * dir);
_returns_(DRV_SUCCESS) unsigned int IsCoolerOn(_out_ int * iCoolerStatus);
_returns_(DRV_SUCCESS) unsigned int IsCountConvertModeAvailable(int mode);
_returns_(DRV_SUCCESS) unsigned int IsInternalMechanicalShutter(_out_ int * InternalShutter);
_returns_(DRV_SUCCESS) unsigned int IsAmplifierAvailable(int iamp);
_returns_(DRV_SUCCESS) unsigned int IsPreAmpGainAvailable(int channel, int amplifier, int index, int pa, _out_ int * status);
_returns_(DRV_SUCCESS) unsigned int IsTriggerModeAvailable(int iTriggerMode);
_reserved_ _returns_(DRV_SUCCESS) unsigned int Merge(const at_32 * arr, at_32 nOrder, at_32 nPoint, at_32 nPixel, float * coeff, at_32 fit, at_32 hbin, at_32 * output, float * start, float * step_Renamed);
_returns_(DRV_SUCCESS) unsigned int OA_AddMode(_string_ _size_(uiModeNameLen) char * pcModeName, _value_(len(pcModeName)) unsigned int uiModeNameLen, _string_ _size_(uiModeDescriptionLen) char * pcModeDescription, _value_(len(pcModeDescription)) unsigned int uiModeDescriptionLen);
_returns_(DRV_SUCCESS) unsigned int OA_DeleteMode(_string_ _size_(uiModeNameLen) const char * const pcModeName, _value_(len(pcModeName)) unsigned int uiModeNameLen);
_returns_(DRV_SUCCESS) unsigned int OA_EnableMode(_string_ const char * const pcModeName);
_returns_(DRV_SUCCESS) unsigned int OA_GetFloat(_string_ const char * const pcModeName, _string_ const char * const pcModeParam, _out_ float * fFloatValue);
_returns_(DRV_SUCCESS) unsigned int OA_GetInt(_string_ const char * const pcModeName, _string_ const char * const pcModeParam, _out_ int * iIntValue);
_returns_(DRV_SUCCESS) unsigned int OA_GetModeAcqParams(_string_ const char * const pcModeName, _out_ _string_ char * const pcListOfParams); // TODO size?
_returns_(DRV_SUCCESS) unsigned int OA_GetNumberOfAcqParams(_string_ const char * const pcModeName, _out_ unsigned int * const puiNumberOfParams);
_returns_(DRV_SUCCESS) unsigned int OA_GetNumberOfPreSetModes(_out_ unsigned int * const puiNumberOfModes);
_returns_(DRV_SUCCESS) unsigned int OA_GetNumberOfUserModes(_out_ unsigned int * const puiNumberOfModes);
_returns_(DRV_SUCCESS) unsigned int OA_GetPreSetModeNames(_out_ _string_ char * pcListOfModes); // TODO size?
_returns_(DRV_SUCCESS) unsigned int OA_GetString(_string_ const char * const pcModeName, _string_ const char * const pcModeParam, _out_ _string_ _size_(uiStringLen) char * pcStringValue, const unsigned int uiStringLen);
_returns_(DRV_SUCCESS) unsigned int OA_GetUserModeNames(_out_ _string_ char * pcListOfModes); // TODO size?
_returns_(DRV_SUCCESS) unsigned int OA_Initialize(_string_ _size_(uiFileNameLen) const char * const pcFilename, _value_(len(pcFilename)) unsigned int uiFileNameLen);
_returns_(DRV_SUCCESS) unsigned int OA_SetFloat(_string_ const char * const pcModeName, _string_ const char * pcModeParam, const float fFloatValue);
_returns_(DRV_SUCCESS) unsigned int OA_SetInt(_string_ const char * const pcModeName, _string_ const char * pcModeParam, const int iIntValue);
_returns_(DRV_SUCCESS) unsigned int OA_SetString(_string_ const char * const pcModeName, _string_ const char * pcModeParam, _string_ _size_(uiStringLen) char * pcStringValue, _value_(len(pcStringValue)) const unsigned int uiStringLen);
_returns_(DRV_SUCCESS) unsigned int OA_WriteToFile(_string_ _size_(uiFileNameLen) const char * const pcFileName, _value_(len(pcFileName)) unsigned int uiFileNameLen);
_returns_(DRV_SUCCESS) unsigned int OutAuxPort(int port, int state);
_returns_(DRV_SUCCESS) unsigned int PrepareAcquisition();
_returns_(DRV_SUCCESS) unsigned int PostProcessCountConvert(_size_(iNumImages,iHeight,iWidth) at_32 * pInputImage, _out_ _size_(iNumImages,iHeight,iWidth) at_32 * pOutputImage, _value_(pInputImage.size) int iOutputBufferSize, _value_(pInputImage.shape[0]) int iNumImages, int iBaseline, int iMode, int iEmGain, float fQE, float fSensitivity, _value_(pInputImage.shape[1]) int iHeight, _value_(pInputImage.shape[2]) int iWidth);
_returns_(DRV_SUCCESS) unsigned int PostProcessNoiseFilter(_size_(iHeight,iWidth) at_32 * pInputImage, _out_ _size_(iHeight,iWidth) at_32 * pOutputImage, _value_(pInputImage.size) int iOutputBufferSize, int iBaseline, int iMode, float fThreshold, _value_(pInputImage.shape[0]) int iHeight, _value_(pInputImage.shape[1]) int iWidth);
_returns_(DRV_SUCCESS) unsigned int PostProcessPhotonCounting(_size_(iNumImages,iHeight,iWidth) at_32 * pInputImage, _out_ _size_(iNumImages,iHeight,iWidth) at_32 * pOutputImage, _value_(pInputImage.size) int iOutputBufferSize, _value_(pInputImage.shape[0]) int iNumImages, int iNumframes, int iNumberOfThresholds, float * pfThreshold, _value_(pInputImage.shape[1]) int iHeight, _value_(pInputImage.shape[2]) int iWidth);
_returns_(DRV_SUCCESS) unsigned int SaveAsBmp(_string_ char * path, _string_ char * palette, _default_(0) at_32 ymin, _default_(0) at_32 ymax);
_returns_(DRV_SUCCESS) unsigned int SaveAsCommentedSif(_string_ char * path, _string_ char * comment);
_returns_(DRV_SUCCESS) unsigned int SaveAsEDF(_string_ char * szPath, int iMode);
_returns_(DRV_SUCCESS) unsigned int SaveAsFITS(_string_ char * szFileTitle, int typ);
_returns_(DRV_SUCCESS) unsigned int SaveAsRaw(_string_ char * szFileTitle, int typ);
_returns_(DRV_SUCCESS) unsigned int SaveAsSif(_string_ char * path);
_returns_(DRV_SUCCESS) unsigned int SaveAsTiff(_string_ char * path, _string_ char * palette, _default_(1) int position, _default_(1) int typ);
_returns_(DRV_SUCCESS) unsigned int SaveAsTiffEx(_string_ char * path, _string_ char * palette, _default_(1) int position, _default_(1) int typ, _default_(1) int mode);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SaveEEPROMToFile(_string_ char * cFileName);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SaveToClipBoard(_string_ char * palette);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SelectDevice(int devNum);
_returns_(DRV_SUCCESS) unsigned int SendSoftwareTrigger();
_returns_(DRV_SUCCESS) unsigned int SetAccumulationCycleTime(float time);
_returns_(DRV_SUCCESS) unsigned int SetAcquisitionMode(int mode);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetAcquisitionType(int typ);
_returns_(DRV_SUCCESS) unsigned int SetADChannel(int channel);
_returns_(DRV_SUCCESS) unsigned int SetAdvancedTriggerModeState(int iState);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetBackground(_size_(size) at_32 * arr, _value_(arr.size) at_u32 size);
_returns_(DRV_SUCCESS) unsigned int SetBaselineClamp(int state);
_returns_(DRV_SUCCESS) unsigned int SetBaselineOffset(int offset);
_returns_(DRV_SUCCESS) unsigned int SetCameraStatusEnable(unsigned long Enable);
_returns_(DRV_SUCCESS) unsigned int SetComplexImage(_value_(len(areas)) int numAreas, _size_(numAreas,6) int * areas);
_returns_(DRV_SUCCESS) unsigned int SetCoolerMode(int mode);
_returns_(DRV_SUCCESS) unsigned int SetCountConvertMode(int Mode);
_returns_(DRV_SUCCESS) unsigned int SetCountConvertWavelength(float wavelength);
_returns_(DRV_SUCCESS) unsigned int SetCropMode(int active, int cropHeight, int reserved);
_returns_(DRV_SUCCESS) unsigned int SetCurrentCamera(at_32 cameraHandle);
_returns_(DRV_SUCCESS) unsigned int SetCustomTrackHBin(int bin);
_returns_(DRV_SUCCESS) unsigned int SetDACOutputScale(int iScale);
_returns_(DRV_SUCCESS) unsigned int SetDACOutput(int iOption, int iResolution, int iValue);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetDataType(int typ);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetDDGAddress(unsigned char t0, unsigned char t1, unsigned char t2, unsigned char t3, unsigned char address);
_deprecated_ _returns_(DRV_SUCCESS) unsigned int SetDDGGain(int gain);
_returns_(DRV_SUCCESS) unsigned int SetDDGGateStep(double step_Renamed);
_returns_(DRV_SUCCESS) unsigned int SetDDGInsertionDelay(int state);
_returns_(DRV_SUCCESS) unsigned int SetDDGIntelligate(int state);
_returns_(DRV_SUCCESS) unsigned int SetDDGIOC(int state);
_returns_(DRV_SUCCESS) unsigned int SetDDGIOCFrequency(double frequency);
_returns_(DRV_SUCCESS) unsigned int SetDDGIOCNumber(unsigned long numberPulses);
_returns_(DRV_SUCCESS) unsigned int SetDDGTimes(double t0, double t1, double t2);
_returns_(DRV_SUCCESS) unsigned int SetDDGTriggerMode(int mode);
_returns_(DRV_SUCCESS) unsigned int SetDDGVariableGateStep(int mode, double p1, double p2);
_returns_(DRV_SUCCESS) unsigned int SetDelayGenerator(int board, short address, int typ);
_returns_(DRV_SUCCESS) unsigned int SetDMAParameters(int MaxImagesPerDMA, float SecondsPerDMA);
_returns_(DRV_SUCCESS) unsigned int SetDualExposureMode(int mode);
_returns_(DRV_SUCCESS) unsigned int SetDualExposureTimes(float expTime1, float expTime2);
_returns_(DRV_SUCCESS) unsigned int SetEMAdvanced(int state);
_returns_(DRV_SUCCESS) unsigned int SetEMCCDGain(int gain);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetEMClockCompensation(int EMClockCompensationFlag);
_returns_(DRV_SUCCESS) unsigned int SetEMGainMode(int mode);
_returns_(DRV_SUCCESS) unsigned int SetExposureTime(float time);
_returns_(DRV_SUCCESS) unsigned int SetFanMode(int mode);
_returns_(DRV_SUCCESS) unsigned int SetFastKinetics(int exposedRows, int seriesLength, float time, int mode, int hbin, int vbin);
_returns_(DRV_SUCCESS) unsigned int SetFastKineticsEx(int exposedRows, int seriesLength, float time, int mode, int hbin, int vbin, int offset);
_returns_(DRV_SUCCESS) unsigned int SetFastExtTrigger(int mode);
_returns_(DRV_SUCCESS) unsigned int SetFilterMode(int mode);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetFilterParameters(int width, float sensitivity, int range, float accept, int smooth, int noise);
_returns_(DRV_SUCCESS) unsigned int SetFKVShiftSpeed(int index);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetFPDP(int state);
_returns_(DRV_SUCCESS) unsigned int SetFrameTransferMode(int mode);
_deprecated_ _returns_(DRV_SUCCESS) unsigned int SetFullImage(int hbin, int vbin);
_returns_(DRV_SUCCESS) unsigned int SetFVBHBin(int bin);
_deprecated_ _returns_(DRV_SUCCESS) unsigned int SetGain(int gain);
_returns_(DRV_SUCCESS) unsigned int SetGate(float delay, float width, float stepRenamed);
_returns_(DRV_SUCCESS) unsigned int SetGateMode(int gatemode);
_returns_(DRV_SUCCESS) unsigned int SetHighCapacity(int state);
_deprecated_ _returns_(DRV_SUCCESS) unsigned int SetHorizontalSpeed(int index);
_returns_(DRV_SUCCESS) unsigned int SetHSSpeed(int typ, int index);
_returns_(DRV_SUCCESS) unsigned int SetImage(int hbin, int vbin, int hstart, int hend, int vstart, int vend);
_returns_(DRV_SUCCESS) unsigned int SetImageFlip(int iHFlip, int iVFlip);
_returns_(DRV_SUCCESS) unsigned int SetImageRotate(int iRotate);
_returns_(DRV_SUCCESS) unsigned int SetIsolatedCropMode(int active, int cropheight, int cropwidth, int vbin, int hbin);
_returns_(DRV_SUCCESS) unsigned int SetKineticCycleTime(float time);
_returns_(DRV_SUCCESS) unsigned int SetMCPGain(int gain);
_returns_(DRV_SUCCESS) unsigned int SetMCPGating(int gating);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetMessageWindow(int wnd);
_returns_(DRV_SUCCESS) unsigned int SetMetaData(int state);
_returns_(DRV_SUCCESS) unsigned int SetMultiTrack(int number, int height, int offset, int * bottom, int * gap);
_returns_(DRV_SUCCESS) unsigned int SetMultiTrackHBin(int bin);
_returns_(DRV_SUCCESS) unsigned int SetMultiTrackHRange(int iStart, int iEnd);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetNextAddress(at_32 * data, at_32 lowAdd, at_32 highAdd, at_32 length, at_32 physical);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetNextAddress16(at_32 * data, at_32 lowAdd, at_32 highAdd, at_32 length, at_32 physical);
_returns_(DRV_SUCCESS) unsigned int SetNumberAccumulations(int number);
_returns_(DRV_SUCCESS) unsigned int SetNumberKinetics(int number);
_returns_(DRV_SUCCESS) unsigned int SetNumberPrescans(int iNumber);
_returns_(DRV_SUCCESS) unsigned int SetOutputAmplifier(int typ);
_returns_(DRV_SUCCESS) unsigned int SetOverlapMode(int mode);
_returns_(DRV_SUCCESS) unsigned int SetPCIMode(int mode, int value);
_returns_(DRV_SUCCESS) unsigned int SetPhotonCounting(int state);
_returns_(DRV_SUCCESS) unsigned int SetPhotonCountingThreshold(at_32 min, at_32 max);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetPixelMode(int bitdepth, int colormode);
_returns_(DRV_SUCCESS) unsigned int SetPreAmpGain(int index);
_returns_(DRV_SUCCESS) unsigned int SetRandomTracks(_value_(len(areas)) int numTracks, _size_(numTracks,2) int * areas);
_returns_(DRV_SUCCESS) unsigned int SetReadMode(int mode);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetRegisterDump(int mode);
_returns_(DRV_SUCCESS) unsigned int SetRingExposureTimes(_value_(len(times)) int numTimes, _size_(numTimes) float * times);
_returns_(DRV_SUCCESS) unsigned int SetShutter(int typ, int mode, int closingtime, int openingtime);
_returns_(DRV_SUCCESS) unsigned int SetShutterEx(int typ, int mode, int closingtime, int openingtime, int extmode);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetShutters(int typ, int mode, int closingtime, int openingtime, int exttype, int extmode, int dummy1, int dummy2);
_returns_(DRV_SUCCESS) unsigned int SetSifComment(_string_ char * comment);
_returns_(DRV_SUCCESS) unsigned int SetSingleTrack(int centre, int height);
_returns_(DRV_SUCCESS) unsigned int SetSingleTrackHBin(int bin);
_returns_(DRV_SUCCESS) unsigned int SetSpool(int active, int method, _string_ char * path, int framebuffersize);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetStorageMode(at_32 mode);
_returns_(DRV_SUCCESS) unsigned int SetTemperature(int temperature);
_returns_(DRV_SUCCESS) unsigned int SetTriggerInvert(int mode);
_returns_(DRV_SUCCESS) unsigned int SetTriggerMode(int mode);
_returns_(DRV_SUCCESS) unsigned int SetIODirection(int index, int iDirection);
_returns_(DRV_SUCCESS) unsigned int SetIOLevel(int index, int iLevel);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetVerticalRowBuffer(int rows);
_deprecated_ _returns_(DRV_SUCCESS) unsigned int SetVerticalSpeed(int index);
_reserved_ _returns_(DRV_SUCCESS) unsigned int SetVirtualChip(int state);
_returns_(DRV_SUCCESS) unsigned int SetVSAmplitude(int index);
_returns_(DRV_SUCCESS) unsigned int SetVSSpeed(int index);
_returns_(DRV_SUCCESS) unsigned int ShutDown();
_returns_(DRV_SUCCESS) unsigned int StartAcquisition();
_reserved_ _returns_(DRV_SUCCESS) unsigned int UnMapPhysicalAddress();
_returns_(DRV_SUCCESS) unsigned int WaitForAcquisition();
_returns_(DRV_SUCCESS) unsigned int WaitForAcquisitionByHandle(at_32 cameraHandle);
_returns_(DRV_SUCCESS) unsigned int WaitForAcquisitionByHandleTimeOut(long cameraHandle, int iTimeOutMs);
_returns_(DRV_SUCCESS) unsigned int WaitForAcquisitionTimeOut(int iTimeOutMs);
_returns_(DRV_SUCCESS) unsigned int WhiteBalance(_size_(info.iY,info.iX) unsigned short * wRed, _size_(info.iY,info.iX) unsigned short * wGreen, _size_(info.iY,info.iX) unsigned short * wBlue, _out_ float * fRelR, _out_ float * fRelB, WhiteBalanceInfo * info);

/* undocumented:
_returns_(DRV_SUCCESS) unsigned int GetCYMGShift(_out_ int * iXshift, _out_ int * iYShift);
_returns_(DRV_SUCCESS) unsigned int GetDDGExternalOutputEnabled(at_u32 uiIndex, _out_ at_u32 * puiEnabled);
_returns_(DRV_SUCCESS) unsigned int GetDDGExternalOutputPolarity(at_u32 uiIndex, _out_ at_u32 * puiPolarity);
_returns_(DRV_SUCCESS) unsigned int GetDDGExternalOutputStepEnabled(at_u32 uiIndex, _out_ at_u32 * puiEnabled);
_returns_(DRV_SUCCESS) unsigned int GetDDGExternalOutputTime(at_u32 uiIndex, _out_ at_u64 * puiDelay, _out_ at_u64 * puiWidth);
_returns_(DRV_SUCCESS) unsigned int GetDDGGateTime(_out_ at_u64 * puiDelay, _out_ at_u64 * puiWidth);
_returns_(DRV_SUCCESS) unsigned int GetDDGInsertionDelay(_out_ int * piState);
_returns_(DRV_SUCCESS) unsigned int GetDDGIntelligate(_out_ int * piState);
_returns_(DRV_SUCCESS) unsigned int GetDDGIOC(_out_ int * state);
_returns_(DRV_SUCCESS) unsigned int GetDDGIOCNumberRequested(_out_ at_u32 * pulses);
_returns_(DRV_SUCCESS) unsigned int GetDDGIOCPeriod(_out_ at_u64 * period);
_returns_(DRV_SUCCESS) unsigned int GetDDGLiteGlobalControlByte(_out_ unsigned char * control);
_returns_(DRV_SUCCESS) unsigned int GetDDGLiteControlByte(AT_DDGLiteChannelId channel, _out_ unsigned char * control);
_returns_(DRV_SUCCESS) unsigned int GetDDGLiteInitialDelay(AT_DDGLiteChannelId channel, _out_ float * fDelay);
_returns_(DRV_SUCCESS) unsigned int GetDDGLitePulseWidth(AT_DDGLiteChannelId channel, _out_ float * fWidth);
_returns_(DRV_SUCCESS) unsigned int GetDDGLiteInterPulseDelay(AT_DDGLiteChannelId channel, _out_ float * fDelay);
_returns_(DRV_SUCCESS) unsigned int GetDDGLitePulsesPerExposure(AT_DDGLiteChannelId channel, _out_ at_u32 * ui32Pulses);
_returns_(DRV_SUCCESS) unsigned int GetDDGStepCoefficients(at_u32 mode, _out_ double * p1, _out_ double * p2);
_returns_(DRV_SUCCESS) unsigned int GetDDGStepMode(_out_ at_u32 * mode);
_returns_(DRV_SUCCESS) unsigned int GetExternalTriggerTermination(_out_ at_u32 * puiTermination);
_returns_(DRV_SUCCESS) unsigned int GetFrontEndStatus(_out_ int * piFlag);
_returns_(DRV_SUCCESS) unsigned int GetGateMode(_out_ int * piGatemode);
_returns_(DRV_SUCCESS) unsigned int GetMCPGainTable(int iNum, _out_ int * piGain, _out_ float * pfPhotoepc);
_returns_(DRV_SUCCESS) unsigned int GetMinimumNumberInSeries(_out_ int * number);
_returns_(DRV_SUCCESS) unsigned int GetNumberDDGExternalOutputs(_out_ at_u32 * puiCount);
_returns_(DRV_SUCCESS) unsigned int GetPhosphorStatus(_out_ int * piFlag);
_returns_(DRV_SUCCESS) unsigned int GetStartUpTime(_out_ float * time);
_returns_(DRV_SUCCESS) unsigned int GetTECStatus(_out_ int * piFlag);
_returns_(DRV_SUCCESS) unsigned int PostProcessDataAveraging(_size_(iNumImages,iHeight,iWidth) at_32 * pInputImage, _out_ _size_(iNumImages,iHeight,iWidth) at_32 * pOutputImage, _value_(pInputImage.size) int iOutputBufferSize, _value_(pInputImage.shape[0]) int iNumImages, int iAveragingFilterMode, _value_(pInputImage.shape[1]) int iHeight, _value_(pInputImage.shape[2]) int iWidth, int iFrameCount, int iAveragingFactor);
_returns_(DRV_SUCCESS) unsigned int SetDDGExternalOutputEnabled(at_u32 uiIndex, at_u32 uiEnabled);
_returns_(DRV_SUCCESS) unsigned int SetDDGExternalOutputPolarity(at_u32 uiIndex, at_u32 uiPolarity);
_returns_(DRV_SUCCESS) unsigned int SetDDGExternalOutputStepEnabled(at_u32 uiIndex, at_u32 uiEnabled);
_returns_(DRV_SUCCESS) unsigned int SetDDGExternalOutputTime(at_u32 uiIndex, at_u64 uiDelay, at_u64 uiWidth);
_returns_(DRV_SUCCESS) unsigned int SetDDGGateTime(at_u64 uiDelay, at_u64 uiWidth);
_returns_(DRV_SUCCESS) unsigned int SetDDGIOCPeriod(at_u64 period);
_returns_(DRV_SUCCESS) unsigned int SetDDGLiteGlobalControlByte(unsigned char control);
_returns_(DRV_SUCCESS) unsigned int SetDDGLiteControlByte(AT_DDGLiteChannelId channel, unsigned char control);
_returns_(DRV_SUCCESS) unsigned int SetDDGLiteInitialDelay(AT_DDGLiteChannelId channel, float fDelay);
_returns_(DRV_SUCCESS) unsigned int SetDDGLitePulseWidth(AT_DDGLiteChannelId channel, float fWidth);
_returns_(DRV_SUCCESS) unsigned int SetDDGLiteInterPulseDelay(AT_DDGLiteChannelId channel, float fDelay);
_returns_(DRV_SUCCESS) unsigned int SetDDGLitePulsesPerExposure(AT_DDGLiteChannelId channel, at_u32 ui32Pulses);
_returns_(DRV_SUCCESS) unsigned int SetDDGStepCoefficients(at_u32 mode, double p1, double p2);
_returns_(DRV_SUCCESS) unsigned int SetDDGStepMode(at_u32 mode);
_returns_(DRV_SUCCESS) unsigned int SetExternalTriggerTermination(at_u32 uiTermination);
_returns_(DRV_SUCCESS) unsigned int SetMultiTrackScan(int trackHeight, int numberTracks, int iSIHStart, int iSIHEnd, int trackHBinning, int trackVBinning, int trackGap, int trackOffset, int trackSkip, int numberSubFrames);
_returns_(DRV_SUCCESS) unsigned int SetPhotonCountingDivisions(at_u32 noOfDivisions, at_32 * divisions);
_returns_(DRV_SUCCESS) unsigned int GetTriggerLevelRange(_out_ float * minimum, _out_ float * maximum);
_returns_(DRV_SUCCESS) unsigned int SetTriggerLevel(float f_level);
_returns_(DRV_SUCCESS) unsigned int SetUSGenomics(at_32 width, at_32 height);
*/

#define DRV_ERROR_CODES 20001
#define DRV_SUCCESS 20002
#define DRV_VXDNOTINSTALLED 20003
#define DRV_ERROR_SCAN 20004
#define DRV_ERROR_CHECK_SUM 20005
#define DRV_ERROR_FILELOAD 20006
#define DRV_UNKNOWN_FUNCTION 20007
#define DRV_ERROR_VXD_INIT 20008
#define DRV_ERROR_ADDRESS 20009
#define DRV_ERROR_PAGELOCK 20010
#define DRV_ERROR_PAGEUNLOCK 20011
#define DRV_ERROR_BOARDTEST 20012
#define DRV_ERROR_ACK 20013
#define DRV_ERROR_UP_FIFO 20014
#define DRV_ERROR_PATTERN 20015

#define DRV_ACQUISITION_ERRORS 20017
#define DRV_ACQ_BUFFER 20018
#define DRV_ACQ_DOWNFIFO_FULL 20019
#define DRV_PROC_UNKONWN_INSTRUCTION 20020
#define DRV_ILLEGAL_OP_CODE 20021
#define DRV_KINETIC_TIME_NOT_MET 20022
#define DRV_ACCUM_TIME_NOT_MET 20023
#define DRV_NO_NEW_DATA 20024
#define KERN_MEM_ERROR 20025
#define DRV_SPOOLERROR 20026
#define DRV_SPOOLSETUPERROR 20027
#define DRV_FILESIZELIMITERROR 20028
#define DRV_ERROR_FILESAVE 20029

#define DRV_TEMPERATURE_CODES 20033
#define DRV_TEMPERATURE_OFF 20034
#define DRV_TEMPERATURE_NOT_STABILIZED 20035
#define DRV_TEMPERATURE_STABILIZED 20036
#define DRV_TEMPERATURE_NOT_REACHED 20037
#define DRV_TEMPERATURE_OUT_RANGE 20038
#define DRV_TEMPERATURE_NOT_SUPPORTED 20039
#define DRV_TEMPERATURE_DRIFT 20040

#define DRV_TEMP_CODES 20033
#define DRV_TEMP_OFF 20034
#define DRV_TEMP_NOT_STABILIZED 20035
#define DRV_TEMP_STABILIZED 20036
#define DRV_TEMP_NOT_REACHED 20037
#define DRV_TEMP_OUT_RANGE 20038
#define DRV_TEMP_NOT_SUPPORTED 20039
#define DRV_TEMP_DRIFT 20040

#define DRV_GENERAL_ERRORS 20049
#define DRV_INVALID_AUX 20050
#define DRV_COF_NOTLOADED 20051
#define DRV_FPGAPROG 20052
#define DRV_FLEXERROR 20053
#define DRV_GPIBERROR 20054
#define DRV_EEPROMVERSIONERROR 20055

#define DRV_DATATYPE 20064
#define DRV_DRIVER_ERRORS 20065
#define DRV_P1INVALID 20066
#define DRV_P2INVALID 20067
#define DRV_P3INVALID 20068
#define DRV_P4INVALID 20069
#define DRV_INIERROR 20070
#define DRV_COFERROR 20071
#define DRV_ACQUIRING 20072
#define DRV_IDLE 20073
#define DRV_TEMPCYCLE 20074
#define DRV_NOT_INITIALIZED 20075
#define DRV_P5INVALID 20076
#define DRV_P6INVALID 20077
#define DRV_INVALID_MODE 20078
#define DRV_INVALID_FILTER 20079

#define DRV_I2CERRORS 20080
#define DRV_I2CDEVNOTFOUND 20081
#define DRV_I2CTIMEOUT 20082
#define DRV_P7INVALID 20083
#define DRV_P8INVALID 20084
#define DRV_P9INVALID 20085
#define DRV_P10INVALID 20086
#define DRV_P11INVALID 20087

#define DRV_USBERROR 20089
#define DRV_IOCERROR 20090
#define DRV_VRMVERSIONERROR 20091
#define DRV_USB_INTERRUPT_ENDPOINT_ERROR 20093
#define DRV_RANDOM_TRACK_ERROR 20094
#define DRV_INVALID_TRIGGER_MODE 20095
#define DRV_LOAD_FIRMWARE_ERROR 20096
#define DRV_DIVIDE_BY_ZERO_ERROR 20097
#define DRV_INVALID_RINGEXPOSURES 20098
#define DRV_BINNING_ERROR 20099
#define DRV_INVALID_AMPLIFIER 20100
#define DRV_INVALID_COUNTCONVERT_MODE 20101

#define DRV_ERROR_NOCAMERA 20990
#define DRV_NOT_SUPPORTED 20991
#define DRV_NOT_AVAILABLE 20992

#define DRV_ERROR_MAP 20115
#define DRV_ERROR_UNMAP 20116
#define DRV_ERROR_MDL 20117
#define DRV_ERROR_UNMDL 20118
#define DRV_ERROR_BUFFSIZE 20119
#define DRV_ERROR_NOHANDLE 20121

#define DRV_GATING_NOT_AVAILABLE 20130
#define DRV_FPGA_VOLTAGE_ERROR 20131

#define DRV_OW_CMD_FAIL 20150
#define DRV_OWMEMORY_BAD_ADDR 20151
#define DRV_OWCMD_NOT_AVAILABLE 20152
#define DRV_OW_NO_SLAVES 20153
#define DRV_OW_NOT_INITIALIZED 20154
#define DRV_OW_ERROR_SLAVE_NUM 20155
#define DRV_MSTIMINGS_ERROR 20156

#define DRV_OA_NULL_ERROR 20173
#define DRV_OA_PARSE_DTD_ERROR 20174
#define DRV_OA_DTD_VALIDATE_ERROR 20175
#define DRV_OA_FILE_ACCESS_ERROR 20176
#define DRV_OA_FILE_DOES_NOT_EXIST 20177
#define DRV_OA_XML_INVALID_OR_NOT_FOUND_ERROR 20178
#define DRV_OA_PRESET_FILE_NOT_LOADED 20179
#define DRV_OA_USER_FILE_NOT_LOADED 20180
#define DRV_OA_PRESET_AND_USER_FILE_NOT_LOADED 20181
#define DRV_OA_INVALID_FILE 20182
#define DRV_OA_FILE_HAS_BEEN_MODIFIED 20183
#define DRV_OA_BUFFER_FULL 20184
#define DRV_OA_INVALID_STRING_LENGTH 20185
#define DRV_OA_INVALID_CHARS_IN_NAME 20186
#define DRV_OA_INVALID_NAMING 20187
#define DRV_OA_GET_CAMERA_ERROR 20188
#define DRV_OA_MODE_ALREADY_EXISTS 20189
#define DRV_OA_STRINGS_NOT_EQUAL 20190
#define DRV_OA_NO_USER_DATA 20191
#define DRV_OA_VALUE_NOT_SUPPORTED 20192
#define DRV_OA_MODE_DOES_NOT_EXIST 20193
#define DRV_OA_CAMERA_NOT_SUPPORTED 20194
#define DRV_OA_FAILED_TO_GET_MODE 20195

#define DRV_PROCESSING_FAILED 20211

#define AC_ACQMODE_SINGLE 1
#define AC_ACQMODE_VIDEO 2
#define AC_ACQMODE_ACCUMULATE 4
#define AC_ACQMODE_KINETIC 8
#define AC_ACQMODE_FRAMETRANSFER 16
#define AC_ACQMODE_FASTKINETICS 32
#define AC_ACQMODE_OVERLAP 64

#define AC_READMODE_FULLIMAGE 1
#define AC_READMODE_SUBIMAGE 2
#define AC_READMODE_SINGLETRACK 4
#define AC_READMODE_FVB 8
#define AC_READMODE_MULTITRACK 16
#define AC_READMODE_RANDOMTRACK 32
#define AC_READMODE_MULTITRACKSCAN 64

#define AC_TRIGGERMODE_INTERNAL 1
#define AC_TRIGGERMODE_EXTERNAL 2
#define AC_TRIGGERMODE_EXTERNAL_FVB_EM 4
#define AC_TRIGGERMODE_CONTINUOUS 8
#define AC_TRIGGERMODE_EXTERNALSTART 16
#define AC_TRIGGERMODE_EXTERNALEXPOSURE 32
#define AC_TRIGGERMODE_INVERTED 0x40

// Deprecated for AC_TRIGGERMODE_EXTERNALEXPOSURE
#define AC_TRIGGERMODE_BULB 32

#define AC_CAMERATYPE_PDA 0
#define AC_CAMERATYPE_IXON 1
#define AC_CAMERATYPE_ICCD 2
#define AC_CAMERATYPE_EMCCD 3
#define AC_CAMERATYPE_CCD 4
#define AC_CAMERATYPE_ISTAR 5
#define AC_CAMERATYPE_VIDEO 6
#define AC_CAMERATYPE_IDUS 7
#define AC_CAMERATYPE_NEWTON 8
#define AC_CAMERATYPE_SURCAM 9
#define AC_CAMERATYPE_USBICCD 10
#define AC_CAMERATYPE_LUCA 11
#define AC_CAMERATYPE_RESERVED 12
#define AC_CAMERATYPE_IKON 13
#define AC_CAMERATYPE_INGAAS 14
#define AC_CAMERATYPE_IVAC 15
#define AC_CAMERATYPE_UNPROGRAMMED 16
#define AC_CAMERATYPE_CLARA 17
#define AC_CAMERATYPE_USBISTAR 18
#define AC_CAMERATYPE_SIMCAM 19
#define AC_CAMERATYPE_SCMOS 20
#define AC_CAMERATYPE_EXTREME 21

#define AC_PIXELMODE_8BIT 1
#define AC_PIXELMODE_14BIT 2
#define AC_PIXELMODE_16BIT 4
#define AC_PIXELMODE_32BIT 8

#define AC_PIXELMODE_MONO 0x000000
#define AC_PIXELMODE_RGB 0x010000
#define AC_PIXELMODE_CMY 0x020000

#define AC_SETFUNCTION_VREADOUT 0x01
#define AC_SETFUNCTION_HREADOUT 0x02
#define AC_SETFUNCTION_TEMPERATURE 0x04
#define AC_SETFUNCTION_MCPGAIN 0x08
#define AC_SETFUNCTION_EMCCDGAIN 0x10
#define AC_SETFUNCTION_BASELINECLAMP 0x20
#define AC_SETFUNCTION_VSAMPLITUDE 0x40
#define AC_SETFUNCTION_HIGHCAPACITY 0x80
#define AC_SETFUNCTION_BASELINEOFFSET 0x0100
#define AC_SETFUNCTION_PREAMPGAIN 0x0200
#define AC_SETFUNCTION_CROPMODE 0x0400
#define AC_SETFUNCTION_DMAPARAMETERS 0x0800
#define AC_SETFUNCTION_HORIZONTALBIN 0x1000
#define AC_SETFUNCTION_MULTITRACKHRANGE 0x2000
#define AC_SETFUNCTION_RANDOMTRACKNOGAPS 0x4000
#define AC_SETFUNCTION_EMADVANCED 0x8000
#define AC_SETFUNCTION_GATEMODE 0x010000
#define AC_SETFUNCTION_DDGTIMES 0x020000
#define AC_SETFUNCTION_IOC 0x040000
#define AC_SETFUNCTION_INTELLIGATE 0x080000
#define AC_SETFUNCTION_INSERTION_DELAY 0x100000
#define AC_SETFUNCTION_GATESTEP 0x200000
#define AC_SETFUNCTION_TRIGGERTERMINATION 0x400000

// Deprecated for AC_SETFUNCTION_MCPGAIN
#define AC_SETFUNCTION_GAIN 8
#define AC_SETFUNCTION_ICCDGAIN 8

#define AC_GETFUNCTION_TEMPERATURE 0x01
#define AC_GETFUNCTION_TARGETTEMPERATURE 0x02
#define AC_GETFUNCTION_TEMPERATURERANGE 0x04
#define AC_GETFUNCTION_DETECTORSIZE 0x08
#define AC_GETFUNCTION_MCPGAIN 0x10
#define AC_GETFUNCTION_EMCCDGAIN 0x20
#define AC_GETFUNCTION_HVFLAG 0x40
#define AC_GETFUNCTION_GATEMODE 0x80
#define AC_GETFUNCTION_DDGTIMES 0x0100
#define AC_GETFUNCTION_IOC 0x0200
#define AC_GETFUNCTION_INTELLIGATE 0x0400
#define AC_GETFUNCTION_INSERTION_DELAY 0x0800
#define AC_GETFUNCTION_GATESTEP 0x1000
#define AC_GETFUNCTION_PHOSPHORSTATUS 0x2000
#define AC_GETFUNCTION_MCPGAINTABLE 0x4000
#define AC_GETFUNCTION_BASELINECLAMP 0x8000

// Deprecated for AC_GETFUNCTION_MCPGAIN
#define AC_GETFUNCTION_GAIN 0x10
#define AC_GETFUNCTION_ICCDGAIN 0x10

#define AC_FEATURES_POLLING 1
#define AC_FEATURES_EVENTS 2
#define AC_FEATURES_SPOOLING 4
#define AC_FEATURES_SHUTTER 8
#define AC_FEATURES_SHUTTEREX 16
#define AC_FEATURES_EXTERNAL_I2C 32
#define AC_FEATURES_SATURATIONEVENT 64
#define AC_FEATURES_FANCONTROL 128
#define AC_FEATURES_MIDFANCONTROL 256
#define AC_FEATURES_TEMPERATUREDURINGACQUISITION 512
#define AC_FEATURES_KEEPCLEANCONTROL 1024
#define AC_FEATURES_DDGLITE 0x0800
#define AC_FEATURES_FTEXTERNALEXPOSURE 0x1000
#define AC_FEATURES_KINETICEXTERNALEXPOSURE 0x2000
#define AC_FEATURES_DACCONTROL 0x4000
#define AC_FEATURES_METADATA 0x8000
#define AC_FEATURES_IOCONTROL 0x10000
#define AC_FEATURES_PHOTONCOUNTING 0x20000
#define AC_FEATURES_COUNTCONVERT 0x40000
#define AC_FEATURES_DUALMODE 0x80000
#define AC_FEATURES_OPTACQUIRE 0x100000
#define AC_FEATURES_REALTIMESPURIOUSNOISEFILTER 0x200000
#define AC_FEATURES_POSTPROCESSSPURIOUSNOISEFILTER 0x400000

#define AC_EMGAIN_8BIT 1
#define AC_EMGAIN_12BIT 2
#define AC_EMGAIN_LINEAR12 4
#define AC_EMGAIN_REAL12 8

#ifdef __cplusplus
}
#endif

#endif

