#ifndef kconfig_h__
#define kconfig_h__

#include "rjson.h"

#include <algorithm>
#ifndef QT_CORE_LIB
#include <fstream>
#endif

class IConfig
{
public:
    // pretty : true 带格式化（换行和缩进），会增大数据包
    virtual String GetConfig(bool pretty = true)const = 0;
};

class KConfig;
class KConfigValue : public IConfig
{
    friend class KConfig;

    KConfigValue(const KConfigValue &) = delete;
    KConfigValue &operator=(const KConfigValue &) = delete;

protected:
    KConfigValue(rapidjson::Type typ, KConfig* c)
    {
        assert(nullptr != c);

        jv_ = new RJsonValue(typ);
        cfg_ = c;
    }

    inline KConfigValue(RJsonValue *v, KConfig* c);

    KConfigValue()
    {
        jv_ = nullptr;
        cfg_ = nullptr;
    }

    virtual ~KConfigValue()
    {
        if (nullptr != jv_)
        {
            delete jv_;
        }

        for (auto *p : values_)
        {
            delete p;
        }
    }

    inline RJsonValue* jval()const
    {
        return jv_;
    }

public:
    // 数组类型的值使用
    inline KConfigValue& PushBack(KConfigValue* v);

    inline KConfigValue& PushBack(const String& v);

    template <class _typ>
    KConfigValue& PushBack(_typ val);

    virtual String GetConfig(bool pretty = true)const override
    {
        String s_conf;
        if (rapidjson::SaveToString(jv_, s_conf, pretty, 2))
        {
            return s_conf;
        }
        return "";
    }

#ifdef QT_CORE_LIB
    inline QByteArray GetConfigUtf8() const{
        return GetConfig().toUtf8();
    }

    bool WriteToFile(const String& file_path/*, bool encode_base64 = false*/){
        String s_conf;
        if (rapidjson::SaveToString(jv_, s_conf, true, 2)){
            return kutil::writeTextFile(file_path, s_conf);
        }
        return false;
    }
#endif

    /**
    *	在保存配置的过程中会生成value
    *      需要通过此方法生成
    *
    **/
    inline KConfigValue* NewValue(rapidjson::Type typ)
    {
        KConfigValue *v = new KConfigValue(typ, this->cfg_);
        values_.push_back(v);
        return v;
    }

    inline KConfigValue* nodeValue(const String& node_path);    // path/to/node

    inline KConfig* NewConfig(const String& json);

    inline KConfigValue& AddMember(const String& name, const char* val);

    inline KConfigValue& AddMember(const String& name, KConfigValue* val);
    
    inline KConfigValue& AddMember(const String& name, const String& val);

    inline KConfigValue& AddMember(const String& name, KConfig* cfg)
    {
        return AddMember(name, (KConfigValue*)cfg);
    }

    template <class _typ>
    KConfigValue& AddMember(const String& name, _typ val)
    {
        static_assert(!std::is_pointer<_typ>::value, "can't be pointer type!");

        // _typ 不能是value，config
        // GCC/ mingw 需要生成一个临时变量
        RJsonValue _v(val);
        return _AddMember_internal(name, _v);
    }

protected:
    inline bool Exist(KConfigValue* val) const
    {
        return std::find(values_.begin(), values_.end(), val) != values_.end();
    }

    inline KConfigValue& _AddMember_internal(const String& name, RJsonValue& v);
protected:
    // real value
    RJsonValue*   jv_ = nullptr;

    // 保留所有的value
    std::vector<KConfigValue*> values_;

private:
    // 文档
    KConfig* cfg_ = nullptr;
};

// 对应于rapidjson：：document
class KConfig : public KConfigValue
{
    typedef RJsonDocument::AllocatorType Alloctor;
    
    KConfig(const KConfig &) = delete; 
    KConfig &operator=(const KConfig &) = delete;

public:
    KConfig(const String& d = "{}")
    {
        jv_ = new RJsonDocument;
        cfg_ = this;

#ifdef QT_CORE_LIB
        doc()->Parse<0>(d.toUtf8().constData());
#else
        doc()->Parse<0>(d.c_str());
#endif
    }

    virtual ~KConfig()
    {
        delete doc();

        jv_ = nullptr;
    }

    inline RJsonDocument* doc()const
    {
        return (RJsonDocument*)(jv_);
    }

    inline bool hasError()const
    {
        return doc()->HasParseError();
    }

public:
    inline Alloctor& GetAlloctor()
    {
        return doc()->GetAllocator();
    }

	bool ReadFromFile(const String& file_path) {
#ifdef QT_CORE_LIB
		QFile file(file_path);
		if (!file.open(QFile::ReadOnly)) {
			return false;
		}
		String str = file.readAll();
		doc()->Parse<0>(str.toUtf8().constData());
#else
		std::ifstream file(file_path);
		if (!file.is_open()) {
			// std::cout << "Error opening file" << std::endl;
			return false;
		}
		//read file
		std::string str((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
		file.close();
		doc()->Parse<0>(str.c_str());
#endif

		if (doc()->HasParseError()) {
			doc()->Parse<0>("{}");
			return false;
		}
		return true;
	}
};

inline KConfigValue::KConfigValue(RJsonValue *v, KConfig* c)
{
    assert(nullptr != c);
    assert(nullptr != v);

    jv_ = new RJsonValue(*v, c->GetAlloctor());
    cfg_ = c;
}

inline KConfigValue* KConfigValue::nodeValue(const String& node_path)    // path/to/node
{
	assert(false);
	return nullptr;

//     RJsonValue v(*jval(), cfg_->GetAlloctor());
//     for (const QString& p : node_path.split("/")) {
//         QByteArray pm = p.toUtf8();
//         if (v.HasMember(pm.constData())) {
//             v = v.operator[](pm.constData());
//         }
//         else {
//             return nullptr;
//         }
//     }
//     KConfigValue* t = new KConfigValue(&v, cfg_);
//     values_.push_back(t);
//     return t;
}

inline KConfig* KConfigValue::NewConfig(const String& json)
{
    KConfig *v = new KConfig(json);
    values_.push_back(v);
    return v;
}

inline KConfigValue& KConfigValue::_AddMember_internal(const String& name, RJsonValue& v)
{
    jv_->AddMember(_utf8_str(name,), v, cfg_->GetAlloctor());
    return *this;
}

inline KConfigValue& KConfigValue::AddMember(const String& name, const char* val)
{
    assert(nullptr != val);
        
    _utf8_p(val, _v);
    return _AddMember_internal(name, _v);
}

inline KConfigValue& KConfigValue::AddMember(const String& name, const String& val)
{
    _utf8_str(val, _v);
    return _AddMember_internal(name, _v);
}

inline KConfigValue& KConfigValue::AddMember(const String& name, KConfigValue* val)
{
    assert(nullptr != val);
    assert(Exist(val));
    return _AddMember_internal(name, *(val->jval()));
}

KConfigValue& KConfigValue::PushBack(KConfigValue* v)
{
    jv_->PushBack(*(v->jval()), cfg_->GetAlloctor());

    return *this;
}

template <class _typ>
KConfigValue& KConfigValue::PushBack(_typ val)
{
    jv_->PushBack(val, cfg_->GetAlloctor());

    return *this;
}

inline KConfigValue& KConfigValue::PushBack(const String& str)
{
    jv_->PushBack(_utf8_str(str,), cfg_->GetAlloctor());
    return *this;
}

#endif // config_h__
