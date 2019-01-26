#pragma once

#include "graphy/scene.h"
#include "defines.h"
#include "viewobject.h"
#include "chartapi.h"

namespace sia {
namespace charty {

struct ChartPrivate;
class CrosshairDetail;
class IVisualObject;

/*!
 * \class Chart
 *
 * \brief 通用图表
 *
 */
typedef sia::graphy::Scene ChartBase;
class CHARTY_API Chart : public ChartBase {
public:
    /*! \brief 创建图表对象，需要一个全局唯一的图表ID
     *
     *    @param	id	进程内全局唯一ID
     *    @return
     */
    explicit Chart(const SkString& id);

    ~Chart();

    /*! \brief 图表id
     *
     *    @param
     *    @return 图表实例的ID
     */
    SkString ID()const;

    /*! \brief 添加图表面板
     *
     *    @param
     *    @return
     */
    HPANEL addPanel(/*bool with_calendar*/);

    /*! \brief 获取panel个数
     *
     *    @param
     *    @return
     */
    uint32_t panelCount()const;

    /*! \brief 获取index对应的panel句柄
     *
     *    @param	idx		panel的顺序索引 [0, panelCount)
     *    @return	panel 句柄
     */
    HPANEL panel(uint32_t idx) const;

    /*! \brief 移除面板
     *
     *    @param
     *    @return
     */
    bool removePanel(HPANEL panel);

    /*! \brief 添加一个序列
     *
     *    @param  panel 序列要添加的面板
     *    @param  series_name 序列名字
     *    @param  type 序列类型
     *    @param  datanames 序列需要的数据名字
     *    @return   false |
     *      1，序列名冲突
     *      2，panel找不到
     */
    bool addSeries(HPANEL panel, const SkString& series_name, SeriesType typ, const SkTArray<SkString>& datanames);
    bool addSeries(HPANEL panel, const SkString& series_name, SeriesType typ, const SkString& dataname);

    /*! \brief 获取面板序列列表
     *
     *    @param	panel	面板
     *    @param	series_names [out]	序列名
     *    @return	true 获取成功
     */
    bool getSeries(HPANEL panel, SkTArray<SkString>& series_names);

    /*! \brief 选中序列
     *
     *    @param	panel	面板。如果为nullptr，则认为取消当前选中序列的选中状态。
     *    @param	series_name	要选中的序列名
     *    @return	true 选中成功
     */
    bool selectSeries(HPANEL panel, const SkString& series_name);

    /*! \brief 显示序列的最大最小值位置提示
     *
     *    @param	panel	面板
     *    @param	series_name	序列名
     *    @param	show	true，显示。
     *    @return
     */
    void showSeriesMinMaxTip(HPANEL panel, const SkString&series_name, bool show);

    /*! \brief 移除已存在的序列
     *
     *    @param	panel	序列所在面板
     *    @param	series_name	序列名
     *    @return
     */
    void removeSeries(HPANEL panel, const SkString& series_name);

    /*! \brief 设置panel中的序列显示颜色
     *		此方法仅对单色的序列有效，比如Bar/Line等。对于Candle/Updown序列请使用setSeriesStyle设置
     *
     *    @param    panel   面板索引
     *    @param    colors <SeriesName, SkColor>   序列颜色，目前的设计中，如果对应的series name不存在，则设置无效
     */
    void setSeriesColor(HPANEL panel, const std::map<SkString, SkColor>& colors);

    /*! \brief 定制序列的外观风格
     *		此时被设置的序列必须是已存在的，否则设置无效
     *    @param	panel	需要设置风格的面板
     *    @param	styles <SeriesName, SeriesStyle>	序列的外观风格
     *    @return
     */
    void setSeriesStyle(HPANEL panel, const std::map<SkString, SeriesStyle>& styles);

    /*! \brief 让panel的显示以一个中间值为区分，分为上下两个等分的部分。
     *
     *    @param    val 中间值。val == chartbase::kInvalidDataVal: 取消设置中间值
     */
    void setMiddleVal(HPANEL panel, chart_val_t val);

    /*! \brief 添加/编辑数据。
     *		因为数据的key是唯一的，后添加的数据会更新原有的数据，所以此方法也可以编辑数据。
     *
     *    @param    data_name   数据名称
     *    @param    datas   数据
     *    @return
     */
    void addData(const SkString& data_name, const std::map<chart_key_t, chart_val_t>& datas);

    /*! \brief 移除数据。
     *		移除数据名所指的所有数据，执行后数据名将销毁。
     *
     *    @param	data_name	数据名
     *    @return
     */
    void removeData(const SkString& data_name);

    /*! \brief 移除数据
     *		移除数据名所指数据结构中的部分数据。执行后数据名依然存在。
     *
     *    @param	data_name	数据名
     *    @param	keys	数据key
     *    @return
     */
    void removeData(const SkString& data_name, const SkTArray<chart_key_t>& keys);

    /*! \brief 设置当前显示的区域
     *
     *    @param    begin 显示开始index
     *    @param    end 显示结束index。-1：显示到最后一条数据
     *    @return
     */
    void setCurrentDomain(int32_t begin = 0, int32_t end = -1);

    /*! \brief 设置标题显示的名字
     *	    名字（symbol）是一个图表所有拥有的
     *    @param    symbol
     */
    void setSymbol(const SkString& symbol);

    /*! \brief 设置标题显示的信息内容
     *
     *    @param	items	定制显示内容。请参考：PanelTitleItem
     *    @return	false： 如果panel没有title
     */
    bool setTitleItems(HPANEL panel, const SkTArray<PanelTitleItem>& items);

    /*! \brief 显示/隐藏panel的Title控件
     *
     *    @param	panel	面板
     *    @param	show	是否显示
     *    @return
     */
    void showPanelTitle(HPANEL panel, bool show);

    /*! \brief 显示/隐藏整个图表上所有panel的某类控件
     *
     *    @param    item    参考PanelItemType
     *    @param    show    true 显示
     *    @return
     */
    void showPanelItem(int item, bool show);

    /*! \brief 网格值显示的档位，
     *	和showGrid配合使用，可以在每个值档位上显示一个横线分割面板
     *
     *    @param    panel 面板
     *    @param    count 共有多少档位
     *    @return
     */
    void setValueSlice(HPANEL panel, int count);

    /*! \brief 放大显示数据范围
     *
     */
    void zoomIn();

    /*! \brief 缩小显示数据范围
     *
     */
    void zoomOut();

    /*! \brief 十字线移动的时候贴合到那个dataname上
     *	    十字线的导航，只会用于第一个panel上(主panel)，
     *    @param dataname 数据名称
     */
    void setCrossHairDataname(const SkString& datanme);

    /*! \brief 定制十字线的详细内容显示
     *
     *    @param	detail	定制
     *    @return
     */
    void setCrossHairDetail(CrosshairDetail* detail);

    /*! \brief Tooltip提示内容的回调函数
     *		在设置的时候，序列名必须已经存在，否则设置失败
     *
     *    @param	panel	面板
     *    @param	callbacks <series_name, callback>	回调设置
     *				每个series单独定制一个提示
     *    @return
     */
    void setSeriesTooltipCallback(HPANEL panel, std::map<SkString, SeriesTooltipCallback> callbacks);

    /*! \brief 应用一个公式
     *
     *    @param    panel   公式计算后，在哪个panel上显示
     *    @param    ta_name 公式名
     *    @param    param   公式需要的参数
     *    @return	公式实例的ID。 >0 为添加成功
     */
    int32_t showFormula(HPANEL panel, const SkString& ta_name, const formula::TaParam& param);

    /*! \brief 销毁一个公式显示
     *
     *    @param	panel	面板
     *    @param	formula_id	公式的id
     *    @return
     */
    void removeFormula(HPANEL panel, int32_t formula_id);

    /*! \brief 添加一个自定义的可视化对象（Object）
     *
     *    @param    panel   添加到哪个panel中
     *              name    对象名
     *              para    对象参数
     *              obj     对象绘制类型
     *
     *    @return   如果panel不存在，或者对象名已存在，或者参数错误，返回false
     */
    bool addObject(HPANEL panel, const SkString& name, const ObjectParam&para, my_sp<IVisualObject> obj);

    /*! \brief 移除可视化对象
     *
     *    @param	panel	面板
     *    @param	object_name	对象名
     *    @return
     */
    void removeObject(HPANEL panel, const SkString& object_name);

    /*! \brief 设置日期/时间的格式化参数
     *
     *    @param	format 在calendar axis上显示的格式，参考c++ strftime
     *    @param	crosshair_detail_format 在十字线显示的格式，参考c++ strftime
     *    @return
     */
    void setCalendarFormat(const SkString& format, const SkString& crosshair_detail_format);

    /*! \brief 移动十字线
     *
     *    @param	offset	>0 向右移动，<0 向左移动. abs(offset) 移动的index的数量
     *    @return
     */
    void moveCrosshair(int offset);

    /*! \brief 平移视图（数据显示范围）
     *
     *    @param	offset	>0 向右移动， <0 向左移动，绝对值是移动的index数
     *    @return	返回false，代表移动失败（比如，向左已经移动到第0根index，或者向右已经是最后一个index）
     */
    bool moveView(int offset);

    /*! \brief 设置精度接口
     *			每个面板都应该有自己的显示精度/除数因子设置，比如价格和数量的显示精度和除数是不同的。
     *
     *    @param	panel
     *    @param	precision 在图表上展示的时候，保留的小数点位数
     *    @param	divisor 传入的进去的数据可以全是整数，在真正计算的时候，会除以这个因子，转换为真正的小数
     *
     *    @return
     */
    void setPrecision(HPANEL panel, int16_t precision, int32_t divisor);

    /*!
     * \enum 内建支持的功能，使用setFunction启用或者禁用某些功能。
     *
     */
    enum FunctionType : int32_t {
        //! 键盘输入
        kFunctionKeyInput = 1 << 1,

        //! 鼠标输入
        kFunctionMouseInput = 1 << 2,

        //! 鼠标框选放大，在开启了kFunctionMouseInput有效
        kFunctionRubberSelect = 1 << 3,

        //! 十字线功能
        kFunctionCrossHair = 1 << 4,

        //! 键盘缩放，在开启了kFunctionKeyInput有效
        kFunctionKeyZoom = 1 << 5,

        //! 键盘移动视图，在开启了kFunctionKeyInput有效
        kFunctionKeyMoveView = 1 << 6,

        //! 支持Tooltip功能
        kFunctionTooltip = 1 << 7,

        //! 支持选中序列
        kFunctionSelectSeries = 1 << 8,

        //! 最大可用
        kFunctionMaxShift = 16,

        //! 所有的功能
        kFunctionAll = ((1<<kFunctionMaxShift) -1),
    };

    /*! \brief 禁用/启用某些内建已经支持的功能
     *
     *    @param	functions	参考FunctionType
     *    @param	enable	true启用
     *    @return
     */
    void setFunction(int32_t functions, bool enable);

    /*! \brief 检查某些功能是否已经启用
     *
     *    @param	functions	功能项目，组合FunctionType值
     *    @return	true	被测试功能已经启用
     */
    bool functionEnabled(int32_t functions)const;

    /*! \brief 为面板设置是否显示水平/垂直分割线
     *
     *    @param	panel	面板
     *    @param	horizontal	水平线，其水平分割的个数由setValueSlice来确定
     *    @param	vertical	垂直线
     *    @param	vertical_count	垂直线的个数
     *    @return
     */
    void showGrid(HPANEL panel, bool horizontal, bool vertical);

    /*! \brief 等分整个面板显示垂直网格线
     *
     *    @param	panel	面板
     *    @param	line_count	垂直线个数
     *    @return
     */
    void setGridVerticalSlice(HPANEL panel, int line_count);

    /*! \brief 在特定的index上显示垂直网格线
     *
     *    @param	panel	面板
     *    @param	visual_indexes	需要显示垂直网格线的visual index 值
     *    @param	middle_index	作为中间的index，显示的线将比别的粗一些！-1: 无效。
     *    @return
     */
    void setGridVerticalSlice(HPANEL panel, const SkTArray<int>& visual_indexes, int middle_index = -1);

    /*! \brief 在特定的key上显示垂直网格线
     *
     *    @param	panel
     *    @param	keys_slice	key值
     *    @param	middle_key	作为中间的key，显示的线比别的粗，kInvalidDataKey：无效
     *    @return
     */
    void setGridVerticalSlice(HPANEL panel, const SkTArray<chart_key_t>& keys_slice, chart_key_t middle_key = chartbase::kInvalidDataKey);

    /*! \brief 合并面板。将sub面板合并到main面板中。
     *  sub面板将会被销毁（无法恢复），sub面板中的主视图内容被合并到main面板上。
     *
     *    @param    main    合并到的面板
     *    @param    sub    被合并的面板
     *    @return
     */
    bool mergePanel(HPANEL main, HPANEL sub);

protected:
    virtual void onDraw(SkCanvas* canvas) override;

    /*! \brief View的事件处理
     *
     *    @param
     *    @return
     */
    virtual bool onEvent(const SkEvent& evt) override;

    /*! \brief 全面更新图表，牵涉到数据的重新计算，并申请布局重绘。
     *		如果只是为了更新界面，不牵涉布局和数据计算，可使用redraw方法
     */
    void update(Bits flags);

    /*! \brief 界面的重新绘制，不牵涉数据计算
     *
     *    @param
     *    @return
     */
    void redraw();

    /*! \brief 鼠标事件处理
     *
     *    @param
     *    @return
     */
    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override;

    virtual bool onKey(sia::KeyBoard key, sia::InputState state, uint32_t modifiers) override;

    virtual void onSizeChange() override;

    virtual void onSurfaceChanged() override;
private:
    my_sp<ChartPrivate> d = nullptr;
};

/*! \brief 设置全局的序列外观展示
 *
 *    @param	ss	SeriesStyle风格设置
 *    @return
 */
CHARTY_API void setSiaSeriesStyle(const SeriesStyle& ss);

/*! \brief 返回设置的全局序列外观
 *
 *    @param
 *    @return	const SeriesStyle&
 */
CHARTY_API const SeriesStyle& siaSeriesStyle();

}
}
