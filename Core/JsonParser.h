//
// JsonParser.h
//

#pragma once

#include "json/json.h"

#define _jverify_return_log(json_key, log) if ((json_key) == Json::nullValue) { Global::CacheLog(log); return; }
#define _jget_int(json_key) ((json_key) == Json::nullValue) ? 0 : (json_key).asInt()
#define _jget_uint(json_key) ((json_key) == Json::nullValue) ? 0u : (json_key).asUInt()
#define _jget_float(json_key) ((json_key) == Json::nullValue) ? 0.0f : (json_key).asFloat()

#define _jget_cstring_default(json_key, default) (json_key).isString() ? (((json_key) == Json::nullValue) ? default : (*(json_key).asCString() == 0 ? default : (json_key).asCString())) : default
#define _jget_cstring(json_key) _jget_cstring_default(json_key, "null")
#define _jget_string(json_key) std::string(_jget_cstring_default(json_key, "null"))
#define _jget_string_default(json_key, default) std::string(_jget_cstring_default(json_key, default))

#define _jis_auto(json_key) (_jget_string(json_key) == "auto")
#define _jget_hex(json_key) (json_key).isString() ? Util::StringToHex(_jget_string_default(json_key, "0x00")) : _jget_uint(json_key);