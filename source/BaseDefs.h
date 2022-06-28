#pragma once

#ifdef __cplusplus
#define IU_EXTERN_C_BEGIN extern "C" {
#define IU_EXTERN_C_END }
#else
#define IU_EXTERN_C_BEGIN
#define IU_EXTERN_C_END
#endif

#define IU_B(x)  (x)
#define IU_KB(x) ((x) << 10)
#define IU_MB(x) ((x) << 20)
#define IU_GB(x) ((x) << 30)
#define IU_TB(x) (((uint64_t)x) << 40)

#define IU_ARRAY_SIZE(arr) sizeof(arr)/sizeof(arr[0])

IU_EXTERN_C_BEGIN

typedef enum {
    Iu_IoResult_Success,
    Iu_IoResult_Eof,
    Iu_IoResult_PermissionDenied,
    Iu_IoResult_OutOfSpace,
    Iu_IoResult_Other,
} Iu_IoResult_;

IU_EXTERN_C_END
