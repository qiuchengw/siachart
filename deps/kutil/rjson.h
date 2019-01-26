#pragma once


// 使用std::string
#ifndef RAPIDJSON_HAS_STDSTRING
#   define RAPIDJSON_HAS_STDSTRING 1
#endif

// #include <stdint.h>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"


typedef rapidjson::Document RJsonDocument;
typedef rapidjson::Value RJsonValue;
// typedef rapidjson::Reader RJsonReader;
typedef rapidjson::StringBuffer RJsonStringBuffer;
// typedef rapidjson::Writer<RJsonStringBuffer> RJsonWriter;


// _utf8_str/_utf8_p 宏有两种使用形式：
//		_utf8_str(s, var)	// 生成名字为var的变量
//		_utf8_str(s, )	// 第二个参数留空，则生成无名临时变量
#ifdef QT_CORE_LIB

#include <QString>
//#include "misc.h"
typedef QString String;
#define _utf8_str(val, var) RJsonValue var(val.toUtf8().constData(), cfg_->GetAlloctor())
#define _utf8_p(val, var) RJsonValue var(val, cfg_->GetAlloctor())

#else   // c++ pure
#include <vector>
#include <string>
typedef std::string String;
#define _utf8_str(val, var) RJsonValue var(val.c_str(), cfg_->GetAlloctor())
#define _utf8_p(val, var) RJsonValue var(val, cfg_->GetAlloctor())

#endif  // QT_CORE_LIB

namespace rapidjson
{

    /**
     *	保存json值到字符串里面	
     *
     *	@param
     *		-[in]
     *          d       json
     *          prety   带格式化
     *          indent  格式化缩进
     *		-[out]
     *          str     保存后的字符串
    **/
    /**
    *	保存json值到字符串里面
    *
    *	@param
    *		-[in]
    *          d       json
    *          prety   带格式化
    *          indent  格式化缩进
    *		-[out]
    *          str     保存后的字符串
    **/
    template<typename _StrType> // QString / std::string
    inline bool SaveToString(RJsonValue* d, _StrType& str,
        bool prety = true, int indent = 4){
        // 保存
        RJsonStringBuffer sb;
        if (prety){
            rapidjson::PrettyWriter<RJsonStringBuffer, rapidjson::UTF8<> > wrt(sb);
            wrt.SetIndent(' ', (indent <= 0) ? 4 : indent); // 4个空格
            d->Accept(wrt);
        }
        else{
            rapidjson::Writer<RJsonStringBuffer, rapidjson::UTF8<> > wrt(sb);
            d->Accept(wrt);
        }
        str = sb.GetString();
        return true;
    }

	class RJsonDoc : public rapidjson::Document
	{
	public:
		/**
		*
		*	@param
		*		-[in]
		*		    content     json内容
		*		    auto_reset_if_fail  如果解析错误，自动设置成解析空文件
		*
		**/
		RJsonDoc(const ::String& content = "{}", bool auto_reset_if_fail = true){
			MyParse(content, auto_reset_if_fail);
		}

		// false ： has error
		bool MyParse(const ::String& content, bool auto_reset_if_fail){
#ifdef QT_CORE_LIB
			QByteArray ar = content.toUtf8();
			Parse<0>(ar.data());
#else
			Parse<0>(content.c_str());
#endif

			if (HasParseError()){
				if (auto_reset_if_fail){
					Parse<0>("{}");
				}
				return false;
			}
			return true;
		}
	};

}
