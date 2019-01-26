#include "taitem.h"

namespace sia {
namespace formula {


TaItem::TaItem(const SkString& name) {
    setName(name);
}

TaItem::~TaItem() {
    if (valid()) {
        TA_ParamHolderFree(param_holder_);
    }
}

void TaItem::setName(const SkString& name) {
    ta_name_ = name;
}

bool TaItem::init() {
    if (valid()) {
        return true;
    }
    return initFunc();
}

TaInputDataIndex toIndex(TA_InputFlags flags) {
    if (0 == flags) {
        return TaInputDataIndex::kOpen;
    }

    switch (flags) {
    case TA_IN_PRICE_OPEN://         0x00000001
        return TaInputDataIndex::kOpen;
    case TA_IN_PRICE_HIGH://         0x00000002
        return TaInputDataIndex::kHigh;
    case TA_IN_PRICE_LOW://          0x00000004
        return TaInputDataIndex::kLow;
    case TA_IN_PRICE_CLOSE://        0x00000008
        return TaInputDataIndex::kClose;
    case TA_IN_PRICE_VOLUME://       0x00000010
        return TaInputDataIndex::kVolume;
    case TA_IN_PRICE_OPENINTEREST:// 0x00000020
        return TaInputDataIndex::kOi;
// 	case TA_IN_PRICE_TIMESTAMP://    0x00000040
// 		return TaInputDataIndex::kOpen;
    }
    return TaInputDataIndex::kMaxInputIndex;
}

bool TaItem::queryParam(SkTArray<TaParamItem>& para) {
    if (!func_info_) {
        return false;
    }

    TA_RetCode rc = TA_SUCCESS;
    const TA_InputParameterInfo *input_params = nullptr;
    for (int i = 0; i < (int)func_info_->nbInput; i++) {
        TA_GetInputParameterInfo(func_info_->handle, i, &input_params);
        TaParamItem item;
        item.name = input_params->paramName;
        item.opt = false;
        switch (input_params->type) {
        case TA_Input_Price:
            item.typ = TaParamType::kPriceList;
            break;

        case TA_Input_Real:
            item.typ = TaParamType::kRealList;
            break;

        case TA_Input_Integer:
            item.typ = TaParamType::kIntList;
            break;
        }
        para.push_back(item);
    }

    // opt input
    const TA_OptInputParameterInfo *opt_input = nullptr;
    for (int i = 0; i < (int)func_info_->nbOptInput; i++) {
        TA_GetOptInputParameterInfo(func_info_->handle, i, &opt_input);
        TaParamItem item;
        item.opt = true;
        item.name = opt_input->paramName;
        item.display_name = opt_input->displayName;
        item.default_val = opt_input->defaultValue;
        item.hint = opt_input->hint;
        item.help_file = opt_input->helpFile;

        switch (opt_input->type) {
        case TA_OptInputParameterType::TA_OptInput_RealRange:
            item.typ = TaParamType::kReal;
            break;
        case TA_OptInputParameterType::TA_OptInput_IntegerRange:
            item.typ = TaParamType::kInt;
            break;
        case TA_OptInputParameterType::TA_OptInput_IntegerList:
            // assert(false);	// 不知道怎么用这样的参数
            item.typ = TaParamType::kInt;
            break;
        case TA_OptInputParameterType::TA_OptInput_RealList:
            // assert(false);	// 不知道怎么用这样的参数
            item.typ = TaParamType::kInt;
            break;
        }
        para.push_back(item);
    }
    return true;
}

bool TaItem::defaultParam(TaParam& para) {
    SkTArray<TaParamItem> tmp;
    if (queryParam(tmp)) {
        for (const TaParamItem& item : tmp) {
            if (!item.opt) {
                continue;
            }

            // 设置默认值
            para.setParam(item.name, item.default_val);
        }
        return true;
    }
    return false;
}

bool TaItem::suggestOutSeries(SkTArray<TaSeries>& series) {
    if (!valid()) {
        return false;
    }

    TA_RetCode rc = TA_SUCCESS;
    const TA_OutputParameterInfo *out_param = nullptr;
    for (int idx = 0; idx < (int)func_info_->nbOutput; idx++) {
        rc = TA_GetOutputParameterInfo(func_info_->handle, idx, &out_param);
        if (rc != TA_SUCCESS) {
            continue;
        }

        SkString para_name(out_param->paramName);
        TaSeries ts;
        ts.data_name.push_back(para_name);
        ts.series_name = para_name;
        if ((out_param->flags & TA_OUT_LINE)
                || (out_param->flags & TA_OUT_DOT_LINE)
                || (out_param->flags & TA_OUT_DASH_LINE)
                || (out_param->flags & TA_OUT_DOT)) {
            ts.typ = chartbase::SeriesType::kLine;
        } else if (out_param->flags & TA_OUT_HISTO) {
            ts.typ = chartbase::SeriesType::kUpdownBar;
        }
        series.push_back(ts);
    }
    return true;
}

bool TaItem::calc(const TaParam& para) {
    opt_in_ = para;
    if (!d_ || !valid()) {
        assert(false);
        return false;
    }

    if (!initInputParam()) {
        return false;
    }

    if (!initOutParam()) {
        return false;
    }
    return evalCall();
}

void TaItem::setData(TaData* d) {
    d_ = d;
}

bool TaItem::valid() const {
    return (param_holder_ && func_info_);
}

bool TaItem::initFunc() {
    // TALIB handle
    const TA_FuncHandle *func_handle = nullptr;
    TA_RetCode rc = TA_GetFuncHandle(ta_name_.c_str(), &func_handle);
    if (rc != TA_SUCCESS) {
        return false;
    }

    // 参数信息
    if ((rc = TA_ParamHolderAlloc(func_handle, &param_holder_)) != TA_SUCCESS) {
        return false;
    }

    // ta func 的信息获取
    if ((rc = TA_GetFuncInfo(func_handle, &func_info_)) != TA_SUCCESS) {
        return false;
    }
    return true;
}

bool TaItem::initInputParam() {
    TA_RetCode rc = TA_SUCCESS;
    const TA_InputParameterInfo *input_params = nullptr;
    for (int i = 0; i < (int)func_info_->nbInput; i++) {
        TA_GetInputParameterInfo(func_info_->handle, i, &input_params);
        switch (input_params->type) {
        case TA_Input_Price:
            rc = TA_SetInputParamPricePtr(param_holder_, i, d_->open(), d_->high(),
                                          d_->low(), d_->close(), d_->volume(), d_->oi());
            if (rc != TA_SUCCESS) {
                return false;
            }
            break;

        case TA_Input_Real: {
            int tint = toIndex(input_params->flags);
            if ((rc = TA_SetInputParamRealPtr(param_holder_, i, d_->indexData(tint))) != TA_SUCCESS) {
                return false;
            }
        }
        break;

        case TA_Input_Integer:
            break;
        default:
            break;
        }
    }

    // opt input
    const TA_OptInputParameterInfo *opt_input = nullptr;
    for (int i = 0; i < (int)func_info_->nbOptInput; i++) {
        TA_GetOptInputParameterInfo(func_info_->handle, i, &opt_input);
        SkString s(opt_input->paramName);
        switch (opt_input->type) {
        case TA_OptInputParameterType::TA_OptInput_RealRange:
            if ((TA_SetOptInputParamReal(param_holder_, i, opt_in_.asT<TA_Real>(s))) != TA_SUCCESS) {
                return false;
            }
            break;
        case TA_OptInputParameterType::TA_OptInput_IntegerRange:
            if ((rc = TA_SetOptInputParamInteger(param_holder_, i, opt_in_.asT<TA_Integer>(s))) != TA_SUCCESS) {
                return false;
            }
            break;
        case TA_OptInputParameterType::TA_OptInput_IntegerList:
            if ((rc = TA_SetOptInputParamInteger(param_holder_, i, opt_in_.asT<TA_Integer>(s))) != TA_SUCCESS) {
                return false;
            }
            break;

        default:
            break;
        }
    }
    return true;
}

bool TaItem::initOutParam() {
    TA_RetCode rc = TA_SUCCESS;

    const TA_OutputParameterInfo *out_param = nullptr;
    for (int idx = 0; idx < (int)func_info_->nbOutput; idx++) {
        if ((rc = TA_GetOutputParameterInfo(func_info_->handle, idx, &out_param)) != TA_SUCCESS) {
            continue;
        }

        switch (out_param->type) {
        case TA_Output_Integer:	// 最多只能接收一个int数组输出
            rc = TA_SetOutputParamIntegerPtr(param_holder_, idx, d_->outInt());
            if (rc != TA_SUCCESS) {
                return false;
            }
            break;

        case TA_Output_Real:
            rc = TA_SetOutputParamRealPtr(param_holder_, idx, d_->outReal((TaOutputDataIndex)idx));
            if (rc != TA_SUCCESS) {
                return false;
            }
            break;
        }
    }
    return true;
}

bool TaItem::evalCall() {
    // 调用公式函数
    TA_Integer start = 0;
    TA_Integer end = d_->dataLength() - 1;
    TA_Integer outstart = 0;
    TA_Integer count = 0;
    TA_RetCode rc = TA_CallFunc(param_holder_, start, end, &outstart, &count);
    if (rc != TA_SUCCESS) {
        return false;
    }

    // 输出数据整理
    const TA_OutputParameterInfo *out_info = nullptr;
    for (int i = 0; i < (int)func_info_->nbOutput; i++) {
        if ((rc = TA_GetOutputParameterInfo(func_info_->handle, i, &out_info)) != TA_SUCCESS) {
            continue;
        }

        SkString base(out_info->paramName);
        std::cout << base.c_str() << std::endl;

        bool int_typ = (out_info->type == TA_OutputParameterType::TA_Output_Integer);
        if (int_typ && (0 != i)) {
            // 只有第一个参数才能是整数数组！
            // assert(false);
            // TODO: fix this
            // ...
        }

        // 记录下来数据的名字和信息
        TaOutputDataIndex idx = (int_typ ? TaOutputDataIndex::kOutInt : (TaOutputDataIndex)i);
        d_->setOut(idx, base, outstart, count);
    }

    // 先不要释放内存，因为可能需要多次计算
//     if ((rc = TA_ParamHolderFree(param_holder_)) != TA_SUCCESS) {
//         return false;
//     }
    return true;
}

}
}

