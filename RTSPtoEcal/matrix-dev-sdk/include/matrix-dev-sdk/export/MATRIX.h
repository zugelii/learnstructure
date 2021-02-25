//
// Copyright 2018 Horizon Robotics.
//

#ifndef MATRIX_DEV_SDK_EXPORT_MATRIX_H_
#define MATRIX_DEV_SDK_EXPORT_MATRIX_H_

#define MATRIX_DLL
#define MATRIX_DLL_EXPORTS

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#define MATRIX_HELPER_DLL_IMPORT __declspec(dllimport)
#define MATRIX_HELPER_DLL_EXPORT __declspec(dllexport)
#define MATRIX_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define MATRIX_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define MATRIX_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define MATRIX_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define MATRIX_HELPER_DLL_IMPORT
#define MATRIX_HELPER_DLL_EXPORT
#define MATRIX_HELPER_DLL_LOCAL
#endif
#endif

// Now we use the generic helper definitions above
//   to define MATRIX_API and MATRIX_LOCAL.
// MATRIX_API is used for the public API symbols. It either DLL
//   imports or DLL exports (or does nothing for static build)
// MATRIX_LOCAL is used for non-api symbols.

// defined if ADAS is compiled as a DLL
#ifdef MATRIX_DLL
// defined if we are building the ADAS DLL (instead of using it)
#ifdef MATRIX_DLL_EXPORTS
#define MATRIX_API MATRIX_HELPER_DLL_EXPORT
#else
#define MATRIX_API MATRIX_HELPER_DLL_IMPORT
#endif  // MATRIX_DLL_EXPORTS
#define MATRIX_LOCAL MATRIX_HELPER_DLL_LOCAL
#else  // MATRIX_DLL is not defined: this means ADAS is a static lib.
#define MATRIX_API
#define MATRIX_LOCAL
#endif  // MATRIX_DLL

#endif  // MATRIX_DEV_SDK_EXPORT_MATRIX_H_

