#ifndef EMOSTATEAFFECTIV_H
#define EMOSTATEAFFECTIV_H

#ifdef __cplusplus
extern "C" {
#endif

#if (!EDK_STATIC_LIB)
#   ifdef EMOSTATE_DLL_EXPORTS
#       ifdef _WIN32
#           define EMOSTATE_DLL_API __declspec(dllexport)
#       else
#           if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER || defined __clang__
#               define EMOSTATE_DLL_API __attribute__ ((visibility("default")))
#           else
#               define EMOSTATE_DLL_API
#           endif
#       endif
#   else
#       ifdef _WIN32
#           define EMOSTATE_DLL_API __declspec(dllimport)
#       else
#           define EMOSTATE_DLL_API
#       endif
#   endif
#else
#   define EMOSTATE_DLL_API extern
#endif

#include "EmoStateDLL.h"

    /**
    * Affectiv emotional type enumerator
    */
    typedef enum EE_AffectivAlgo_enum {

        AFF_EXCITEMENT  = 0x0001,
        AFF_MEDITATION  = 0x0002,
        AFF_FRUSTRATION = 0x0004,
        AFF_ENGAGEMENT_BOREDOM = 0x0008,
        AFF_VALENCE     = 0x0010

    } EE_AffectivAlgo_t;

    //! Returns the long term excitement level of the user
    /*!
        \param state - EmoStateHandle

        \return excitement level (0.0 to 1.0)

        \sa ES_AffectivGetExcitementShortTermScore
    */
    EMOSTATE_DLL_API float ES_AffectivGetExcitementLongTermScore(
        EmoStateHandle state);

    //! Returns short term excitement level of the user
    /*!
        \param state - EmoStateHandle

        \return excitement level (0.0 to 1.0)

        \sa ES_AffectivGetExcitementLongTermScore
    */
    EMOSTATE_DLL_API float ES_AffectivGetExcitementShortTermScore(
        EmoStateHandle state);

    //! Query whether the signal is too noisy for Affectiv detection to be active
    /*!
        \param state - EmoStateHandle
        \param type  - Affectiv detection type

        \return detection state (0: Not Active, 1: Active)

        \sa EE_AffectivAlgo_t
    */
    EMOSTATE_DLL_API int ES_AffectivIsActive(EmoStateHandle state,
            EE_AffectivAlgo_t type);

    //! Returns meditation level of the user
    /*!
        \param state - EmoStateHandle

        \return meditation level (0.0 to 1.0)
    */
    EMOSTATE_DLL_API float ES_AffectivGetMeditationScore(EmoStateHandle state);

    //! Returns frustration level of the user
    /*!
        \param state - EmoStateHandle

        \return frustration level (0.0 to 1.0)
    */
    EMOSTATE_DLL_API float ES_AffectivGetFrustrationScore(EmoStateHandle state);

    //! Returns engagement/boredom level of the user
    /*!
        \param state - EmoStateHandle

        \return engagement/boredom level (0.0 to 1.0)
    */
    EMOSTATE_DLL_API float ES_AffectivGetEngagementBoredomScore(
        EmoStateHandle state);

    //! Returns valence level of the user
    /*!
        \param state - EmoStateHandle

        \return valence level (0.0 to 1.0)
    */
    EMOSTATE_DLL_API float ES_AffectivGetValenceScore(EmoStateHandle state);

    //! Check whether two states are with identical 'emotiv' state
    /*!
        \param a - EmoStateHandle
        \param b - EmoStateHandle

        \return 1: Equal, 0: Different

        \sa ES_ExpressivEqual, ES_CognitivEqual, ES_EmoEngineEqual, ES_Equal
    */
    EMOSTATE_DLL_API int ES_AffectivEqual(EmoStateHandle a, EmoStateHandle b);

    //! Returns short term excitement model parameters
    /*!
        \param state                - EmoStateHandle
        \param rawScore             - return raw score
        \param minScale, maxScale   - return scale range
        \sa ES_AffectivGetExcitementShortTermModelParams
    */
    EMOSTATE_DLL_API void ES_AffectivGetExcitementShortTermModelParams(
        EmoStateHandle state, double * rawScore, double * minScale, double * maxScale);

    //! Returns Meditation model parameters
    /*!
        \param state                - EmoStateHandle
        \param rawScore             - return raw score
        \param minScale, maxScale   - return scale range
        \sa ES_AffectivGetMeditationModelParams
    */
    EMOSTATE_DLL_API void ES_AffectivGetMeditationModelParams(EmoStateHandle state,
            double * rawScore, double * minScale, double * maxScale);

    //! Returns EngagementBoredom model parameters
    /*!
        \param state                - EmoStateHandle
        \param rawScore             - return raw score
        \param minScale, maxScale   - return scale range
        \sa ES_AffectivGetEngagementBoredomModelParams
    */
    EMOSTATE_DLL_API void ES_AffectivGetEngagementBoredomModelParams(
        EmoStateHandle state, double * rawScore, double * minScale, double * maxScale);

    //! Returns Frustration model parameters
    /*!
        \param state                - EmoStateHandle
        \param rawScore             - return raw score
        \param minScale, maxScale   - return scale range
        \sa ES_AffectivGetFrustrationModelParams
    */
    EMOSTATE_DLL_API void ES_AffectivGetFrustrationModelParams(
        EmoStateHandle state, double * rawScore, double * minScale, double * maxScale);

    //! Returns Valence model parameters
    /*!
        \param state                - EmoStateHandle
        \param rawScore             - return raw score
        \param minScale, maxScale   - return scale range
        \sa ES_AffectivGetValenceModelParams
    */
    EMOSTATE_DLL_API void ES_AffectivGetValenceModelParams(EmoStateHandle state,
            double * rawScore, double * minScale, double * maxScale);


#ifdef __cplusplus
}
#endif

#endif // EMOSTATEAFFECTIV_H
