#ifndef __COM_ERROR_H__
#define __COM_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/** @brief Enumerated type for base error codes. */
typedef enum {
    COM_SUCCESS                         =  0,                         ///< 0 Success
    COM_ERROR_HANDLER_MISSING,                                        ///< 1 Handler is missing
    COM_ERROR_NOT_ENABLED,                                            ///< 2 Operation has not been enabled
    COM_ERROR_INTERNAL,                                               ///< 3 Internal Error
    COM_ERROR_NO_MEM,                                                 ///< 4 No Memory for operation
    COM_ERROR_NOT_FOUND,                                              ///< 5 Not found
    COM_ERROR_NOT_SUPPORTED,                                          ///< 6 Not supported
    COM_ERROR_INVALID_PARAM,                                          ///< 7 Invalid Parameter
    COM_ERROR_INVALID_STATE,                                          ///< 8 Invalid state, operation disallowed in this state
    COM_ERROR_INVALID_LENGTH,                                         ///< 9 Invalid Length
    COM_ERROR_INVALID_FLAGS,                                          ///< 10 Invalid Flags
    COM_ERROR_INVALID_DATA,                                           ///< 11 Invalid Data
    COM_ERROR_DATA_SIZE,                                              ///< 12 Data size exceeds limit
    COM_ERROR_TIMEOUT,                                                ///< 13 Operation timed out
    COM_ERROR_NULL,                                                   ///< 14 Null Pointer
    COM_ERROR_FORBIDDEN,                                              ///< 15 Forbidden Operation
    COM_ERROR_INVALID_ADDR,                                           ///< 16 Bad Memory Address
    COM_ERROR_BUSY,                                                   ///< 17 Busy
    COM_ERROR_OPERATION_FAILED,                                       ///< 18 Operation Failed
    COM_ERROR_VALID_FAILED,                                           ///< 19 Validation failed
    COM_ERROR_STR_TOO_LONG,                                           ///< 20 string is too long
    COM_ERROR_STR_INVALID_CHAR,                                       ///< 21 the string has invalid char
    COM_ERROR_WITHOUT_INIT,                                           ///< 22 without init
    COM_ERROR_CRC_FAIL,                                               ///< 23 CRC check failed
    COM_ERROR_ASSERT_FAIL,                                            ///< 24 assert failed
    COM_ERROR_SYSTEM,                                                 ///< 25 system failed, used only for extern software error
    COM_ERROR_UNKNOWN,                                                ///< 26 unknown error
    COM_ERROR_REINIT,                                                 ///< 27 repeat init
    COM_ERROR_SAME_PARAM,                                             ///< 28 the parameter is same as current parameter

    // file
    COM_ERROR_FILE_NULL,                                              ///< file is not exist
    COM_ERROR_FILE_OPEN,                                              ///< file open wrong
    COM_ERROR_FILE_CLOSE,                                             ///< file close wrong
    COM_ERROR_FILE_READ,                                              ///< file read wrong
    COM_ERROR_FILE_WRITE,                                             ///< file write wrong
    COM_ERROR_FILE_NAME,                                              ///< file name wrong
    COM_ERROR_FILE_TYPE,                                              ///< file type wrong

    // ioctrl
    COM_ERROR_IOCTL_GET,                                              ///< ioctrl get fail
    COM_ERROR_IOCTL_SET,                                              ///< ioctrl set fail

    // database
    COM_ERROR_DATABASE_NULL,                                          ///< database is null
    COM_ERROR_DATABASE_OPEN,                                          ///< database open wrong
    COM_ERROR_DATABASE_CLOSE,                                         ///< database close wrong
    COM_ERROR_DATABASE_INSERT,                                        ///< database insert wrong
    COM_ERROR_DATABASE_DELETE_ITEM,                                   ///< database delete item wrong
    COM_ERROR_DATABASE_QUERY,                                         ///< database query wrong
    COM_ERROR_DATABASE_UPDATE,                                        ///< database update wrong

    // user login
    COM_ERROR_LOGIN_ACCOUNT_NULL,                                     ///< account string is null
    COM_ERROR_LOGIN_PWD_NULL,                                         ///< password string is null
    COM_ERROR_LOGIN_PWD2_NULL,                                        ///< password2 string is null
    COM_ERROR_LOGIN_ACCOUNT_ILLEGAL,                                  ///< account string is illegal
    COM_ERROR_LOGIN_PWD_ILLEGAL,                                      ///< password string is illegal
    COM_ERROR_LOGIN_PWD2_ILLEGAL,                                     ///< password2 string is illegal
    COM_ERROR_LOGIN_ACCOUNT_LENGTH_LONG,                              ///< account string is too long
    COM_ERROR_LOGIN_PWD_LENGTH_LONG,                                  ///< password string is too long
    COM_ERROR_LOGIN_PWD2_LENGTH_LONG,                                 ///< password2 string is too long
    COM_ERROR_LOGIN_ACCOUNT_LENGTH_SHORT,                             ///< account string is too short
    COM_ERROR_LOGIN_PWD_LENGTH_SHORT,                                 ///< password string is too short
    COM_ERROR_LOGIN_PWD2_LENGTH_SHORT,                                ///< password2 string is too short
    COM_ERROR_LOGIN_ACCOUNT_SIMPLE,                                   ///< account string is too simple
    COM_ERROR_LOGIN_PWD_SIMPLE,                                       ///< password string is too simple
    COM_ERROR_LOGIN_ACCOUNT_NOT_EXISTENT,                             ///< account is not existent
    COM_ERROR_LOGIN_ACCOUNT_IS_EXISTENT,                              ///< account is existent
    COM_ERROR_LOGIN_PWD_NO_MATCH,                                     ///< account and password is no match
    COM_ERROR_LOGIN_PWD_NOT_SAME,                                     ///< password1 string and password2 string is not same

    // patient infomation
    COM_ERROR_PT_INFO_NULL,                                           ///< patient infomation is null
    COM_ERROR_PT_INFO_NAME_NULL,                                      ///< patient name infomation is null

    // network
    COM_ERROR_NET_BIND_PORT,                                          ///< bind port error
}com_base_error_code_t;

/**
 * @brief execute this function when error code occurs
 * @param error_code - error code generated at runtime
 * @param line_num - the error code generated in this line
 * @param p_file_name - the error code generated in this file
 */
void com_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);

#define COM_ERROR_CHECK(ERR_CODE)           \
    do{                                     \
        if(ERR_CODE != COM_SUCCESS){        \
        com_error_handler((ERR_CODE), __LINE__, (uint8_t*) __FILE__);  \
        }                                   \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif // __COM_ERROR_H__
