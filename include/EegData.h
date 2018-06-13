/**
 * Emotiv SDK
 * Copyright (c) 2016 Emotiv Inc.
 *
 * This file is part of the Emotiv SDK.
 *
 * Header file for EEG data related API.
 *
 */


#ifndef EEGDATA_H
#define EEGDATA_H

#ifdef __cplusplus
extern "C" {
#endif

#if(!EDK_STATIC_LIB)
#   ifdef EDK_EXPORTS
#       ifdef _WIN32
#           define EDK_API __declspec(dllexport)
#       else
#           if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER || defined __clang__
#               define EDK_API __attribute__ ((visibility("default")))
#           else
#               define EDK_API
#           endif
#       endif
#   else
#       ifdef _WIN32
#           define EDK_API __declspec(dllimport)
#       else
#           define EDK_API
#       endif
#   endif
#else
#define EDK_API extern
#endif

    #include "edk.h"

    //! Handle to data placeholder allocated by EE_DataCreate.
    /*!
        \sa EE_DataCreate()
    */
    typedef void * DataHandle;


    //! Returns a handle to memory that can hold data. This handle can be reused by the caller to retrieve subsequent data.
    /*!
        \return DataHandle
    */
    EDK_API DataHandle EE_DataCreate();


    //! Frees memory referenced by a data handle.
    /*!
        \param hData - a handle returned by EE_DataCreate()
    */
    EDK_API void EE_DataFree(DataHandle hData);


    //! Updates the content of the data handle to point to new data since the last call
    /*!
        \param userId - user ID
        \param hData - a handle returned by EE_DataCreate()
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful
    */
    EDK_API int EE_DataUpdateHandle(unsigned int userId, DataHandle hData);


    //! Extracts data of a particulat channel from the data handle
    /*!
        \param hData - a handle returned by EE_DataCreate()
        \param channel - channel that you are interested in
        \param buffer - pre-allocated buffer
        \param bufferSizeInSample - size of the pre-allocated buffer
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful
    */
    EDK_API int EE_DataGet(DataHandle hData, EE_DataChannel_t channel,
                           double buffer[], unsigned int bufferSizeInSample);


    //! Extracts data of a list of channels from the data handle
    /*!
        \param hData - a handle returned by EE_DataCreate()
        \param channels - a list of channel that you are interested in
        \param nChannels - number of channels in the channel list
        \param buffer - pre-allocated 2 dimensional buffer, has to be nChannels x bufferSizeInSample
        \param bufferSizeInSample - size of the pre-allocated buffer for each channel
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful
    */
    EDK_API int EE_DataGetMultiChannels(DataHandle hData,
                                        EE_DataChannel_t channels[], unsigned int nChannels, double * buffer[],
                                        unsigned int bufferSizeInSample);


    //! Returns number of sample of data stored in the data handle
    /*!
        \param hData - a handle returned by EE_DataCreate()
        \param nSampleOut - receives the number of sample of data stored in the data handle
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful
    */
    EDK_API int EE_DataGetNumberOfSample(DataHandle hData,
                                         unsigned int * nSampleOut);


    //! Sets the size of the data buffer. The size of the buffer affects how frequent EE_DataUpdateHandle() needs to be called to prevent data loss.
    /*!
        \param bufferSizeInSec - buffer size in second
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful
    */
    EDK_API int EE_DataSetBufferSizeInSec(float bufferSizeInSec);


    //! Returns the size of the data buffer
    /*!
        \param pBufferSizeInSecOut - receives the size of the data buffer
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful
    */
    EDK_API int EE_DataGetBufferSizeInSec(float * pBufferSizeInSecOut);


    //! Controls acquisition of data from EmoEngine (which is off by default).
    /*!
        \param userId - user ID
        \param enable - If true, then enables data acquisition
                      - If false, then disables data acquisition
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API int EE_DataAcquisitionEnable(unsigned int userId, bool enable);


    //! Returns whether data acquisition is enabled
    /*!
        \param userId - user ID
        \param pEnableOut - receives whether data acquisition is enabled
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API int EE_DataAcquisitionIsEnabled(unsigned int userId,
                                            bool * pEnableOut);


    //! Sets sychronization signal
    /*!
        \param userId - user ID
        \param signal - value of the sychronization signal
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API int EE_DataSetSychronizationSignal(unsigned int userId, int signal);


    //! Sets marker
    /*!
        \param userId - user ID
        \param marker - value of the marker
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API int EE_DataSetMarker(unsigned int userId, int marker);


    //! Gets sampling rate
    /*!
        \param userId - user ID
        \param samplingRateOut - receives the sampling rate
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API int EE_DataGetSamplingRate(unsigned int userId,
                                       unsigned int * samplingRateOut);


    //! Initializes EmoEngine instance which reads EEG data from a pre-recorded session file.
    /*!
        \param strFilePath - File path to the pre-recorded EEG file.
        \param strMotionFilePath - File path to the pre-recorded Motion file.

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if an instance is created successfully

        \sa edkErrorCode.h
    */
    EDK_API int
    EE_EngineLocalConnect(const char * strFilePath,
                          const char * strMotionFilePath = "");


    //! Start playback EDF
    /*!
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API int EE_EdfStart();


    //! Stop playback EDF
    /*!
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API int EE_EdfStop();


    //! Seek Edf File Stream
    /*!
        \param sec - time position to seek in seconds
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API int EE_EdfSeek(int sec);


    //! Get total time of EDF file
    /*!
        \param ptotalTimeOut - received total time of EDF File in seconds
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API int EE_EdfGetTotalTime(int * pTotalTimeOut);


    //! Start Saving EEG to EDF file
    /*!
        \param time - time
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API void EE_EdfStartSaving(int userID, const char * filename,
                                   const char * patientID, const char * recordID, const char * date,
                                   const char * time);


    //! Stop saving EEG from one user
    /*!

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API void EE_EdfStopSaving(int userID);


    //! Stop saving EEG from all user
    /*!
        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if the command succeeded
    */
    EDK_API void EE_EdfStopSavingAll();


    EDK_API void EE_EdfSetReserve(unsigned int bits);

};

#endif // EEGDATA_H
