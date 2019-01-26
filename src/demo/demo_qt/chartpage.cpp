#include "chartpage.h"
#include "ui_chartpage.h"
#include "../../resy/coloritem.h"
#include "../../graphy/paintman.h"
#include "../../charty/charty/chartman.h"
#include "../../charty/formula/formulaman.h"

#include <QMessageBox>

ChartPage::ChartPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartPage) {
    ui->setupUi(this);


    // 链接信号槽
    initConnections();

    chart_ = ui->chart_->chart();
    refreshPanels();

    // 刷新序列
    refreshSeries();

    // 功能
    on_funcs_toggled(nullptr);

    // 刷新指标
    refreshFormulaGroup();

}

ChartPage::~ChartPage() {
    delete ui;
}

HPANEL ChartPage::currentPanel() const {
    int idx = ui->panel_sel_->currentText().toInt();
    return chart_->panel(idx);
}

void ChartPage::refreshPanels() {
    ui->panel_sel_->clear();

    for (int i = 0; i < chart_->panelCount(); ++i) {
        ui->panel_sel_->addItem(QString::number(i));
    }
}

SkString ChartPage::currentSeries() const {
    QString str = ui->series_sel_->currentText();
    return SkString(str.toUtf8().data());
}

////////////////////////////////////////////////////
/// functions enable/disable
void ChartPage::on_funcs_toggled(QWidget* box) {
    static const QHash<QCheckBox*, Chart::FunctionType> _funcs = {
        {ui->func_cross_, Chart::kFunctionCrossHair},
        {ui->func_key_, Chart::kFunctionKeyInput},
        {ui->func_mouse_, Chart::kFunctionMouseInput},
        {ui->func_moveview_, Chart::kFunctionKeyMoveView},
        {ui->func_rubber_, Chart::kFunctionRubberSelect},
        {ui->func_tooltip_, Chart::kFunctionTooltip},
        {ui->func_zoom_, Chart::kFunctionKeyZoom},
        {ui->func_select_series_, Chart::kFunctionSelectSeries},
    };

    qint32 func_list = 0;
    for (auto i = _funcs.begin(); i != _funcs.end(); ++i) {
        if (i.key()->checkState() == Qt::Checked) {
            func_list |= i.value();
        }
    }

    chart_->setFunction(Chart::kFunctionAll, false);
    chart_->setFunction(func_list, true);
}

void ChartPage::on_ctl_panel_toggled(QWidget *box) {
    static const QHash<QCheckBox*, int> _funcs = {
        {ui->ctl_panel_ratioaxis_, PanelItemType::kPanelRatioAxis},
        {ui->ctl_panel_title_, PanelItemType::kPanelTitle},
        {ui->ctl_panel_valueaxis_, PanelItemType::kPanelValueAxis},
    };

    qint32 func_list = 0;
    for (auto i = _funcs.begin(); i != _funcs.end(); ++i) {
        if (i.key()->checkState() == Qt::Checked) {
            func_list |= i.value();
        }
    }

    chart_->showPanelItem(PanelItemType::kPanelAll, false);
    chart_->showPanelItem(func_list, true);
}

////////////////////////////////////////////////////////
/// panel add/delete/select
void ChartPage::on_panel_add__clicked() {
    chart_->addPanel();
    refreshPanels();
}

void ChartPage::on_panel_del__clicked() {
    chart_->removePanel(currentPanel());
    refreshPanels();
}

void ChartPage::on_panel_sel__currentIndexChanged(int index) {
    SkTArray<SkString> series;
    if (chart_->getSeries(currentPanel(), series)) {
        ui->series_sel_->clear();
        for (SkString& s : series) {
            ui->series_sel_->addItem(s.c_str());
        }
    }
}

/////////////////////////////////////////////////////////
/// series delete/select/add
///
SeriesType ChartPage::refreshSeries() {
    static QHash<int, QString> _types = {
        { (int)SeriesType::kLine, QStringLiteral("Line") },
        { (int)SeriesType::kBar, QStringLiteral("Bar") },
        { (int)SeriesType::kUpdownBar, QStringLiteral("Updown") },
        { (int)SeriesType::kCandle, QStringLiteral("Candle") },
    };

    if (ui->series_type_->count() <= 0) {
        for (auto i : _types) {
            ui->series_type_->addItem(i);
        }
    }

    QString typ = ui->series_type_->currentText();
    return (SeriesType)_types.key(typ);
}

void ChartPage::on_series_sel__currentIndexChanged(const QString &arg1) {
    chart_->selectSeries(currentPanel(), SkString(arg1.toUtf8().data()) );
}

void ChartPage::on_series_del__clicked() {
    chart_->removeSeries(currentPanel(), currentSeries());
}

void ChartPage::on_series_add__clicked() {
    SeriesType typ = refreshSeries();

//    ui->series_type_
}

////////////////////////////////////////////////////
/// panel grid
void ChartPage::setPanelGrid() {
    HPANEL pa = currentPanel();

    // middle val
    bool show_middle = (ui->panel_middleval_->checkState() == Qt::Checked);
    double middle_val = ui->panel_middleval_edit_->value();
    chart_->setMiddleVal(pa, show_middle ? middle_val: chartbase::kInvalidDataValue);


    // grid
    bool h_checked = (ui->panel_hgrid_->checkState() == Qt::Checked);
    bool v_checked = (ui->panel_vgrid_->checkState() == Qt::Checked);
    chart_->showGrid(pa, h_checked, v_checked);

    // grid count
    chart_->setValueSlice(pa, ui->panel_hgrid_val_->value());
    chart_->setGridVerticalSlice(pa, ui->panel_vgrid_val_->value());
}

void ChartPage::on_panel_middleval__toggled(bool checked) {
    setPanelGrid();
}

void ChartPage::on_panel_middleval_edit__valueChanged(double val) {
    setPanelGrid();
}

void ChartPage::on_panel_hgrid__toggled(bool h_checked) {
    setPanelGrid();
}

void ChartPage::on_panel_vgrid__toggled(bool v_checked) {
    setPanelGrid();
}

void ChartPage::on_panel_hgrid_val__valueChanged(int arg1) {
    setPanelGrid();
}

void ChartPage::on_panel_vgrid_val__valueChanged(int arg1) {
    setPanelGrid();
}

////////////////////////////////////
// series style
void ChartPage::on_sstyle_antialias__toggled(bool checked) {
    refreshSeriesStyle();
}

void ChartPage::on_sstyle_fillbar__toggled(bool checked) {
    refreshSeriesStyle();
}

void ChartPage::on_sstyle_stroke_width__valueChanged(double arg1) {
    refreshSeriesStyle();
}

void ChartPage::on_sstyle_bar_width__valueChanged(double arg1) {
    refreshSeriesStyle();
}

void ChartPage::on_sstyle_colorSelected(const QColor &arg1) {
    refreshSeriesStyle();
}

void ChartPage::refreshSeriesStyle() {
    SeriesStyle ss;
    ss.bar_width = ui->sstyle_bar_width_->value();
    ss.fill_bar = (ui->sstyle_fillbar_->checkState() == Qt::Checked);
    ss.antialias = (ui->sstyle_antialias_->checkState() == Qt::Checked);
    ss.stroke_width = ui->sstyle_stroke_width_->value();
    ss.up_color = (ui->sstyle_up_color_->skcolor());
    ss.down_color = (ui->sstyle_down_color_->skcolor());
    ss.color = (ui->sstyle_color_->skcolor());
    setSiaSeriesStyle(ss);
}

void ChartPage::initConnections() {
    ui->sstyle_color_->setColor(Qt::cyan);
    ui->sstyle_up_color_->setColor(Qt::red);
    ui->sstyle_down_color_->setColor(Qt::green);

    connect(ui->sstyle_color_, &QColorSel::colorSelected, this, &ChartPage::on_sstyle_colorSelected);
    connect(ui->sstyle_up_color_, &QColorSel::colorSelected, this, &ChartPage::on_sstyle_colorSelected);
    connect(ui->sstyle_down_color_, &QColorSel::colorSelected, this, &ChartPage::on_sstyle_colorSelected);

    refreshSeriesStyle();

    // chart functions
    for (QCheckBox* w : ui->group_funcs_->findChildren<QCheckBox*>()) {
        connect(w, SIGNAL(clicked()), &funcs_mapper_, SLOT(map()));
        funcs_mapper_.setMapping(w, w);
    }
    connect(&funcs_mapper_, SIGNAL(mapped(QWidget*)), this, SLOT(on_funcs_toggled(QWidget*)));

    // panel controls
    for (QCheckBox* w : ui->group_ctl_panel_->findChildren<QCheckBox*>()) {
        connect(w, SIGNAL(clicked()), &ctl_panel_mapper_, SLOT(map()));
        ctl_panel_mapper_.setMapping(w, w);
    }
    connect(&ctl_panel_mapper_, SIGNAL(mapped(QWidget*)), this, SLOT(on_ctl_panel_toggled(QWidget*)));
}

/////////////////////////////////////////////////////////
/// theme
void ChartPage::on_theme_white__toggled(bool checked) {
    if (checked) {
        SkString err;
        if (!ChartMan::instance()->loadThemeConfig(SkString("./white_theme.lua"), err)) {
            QMessageBox::information(this, QStringLiteral("error"), QString(err.c_str()));
        }
    }
}

void ChartPage::on_theme_black__toggled(bool checked) {
    if (checked) {
        SkString err;
        if (!ChartMan::instance()->loadThemeConfig(SkString("./black_theme.lua"), err)) {
            QMessageBox::information(this, QStringLiteral("error"), QString(err.c_str()));
        }
    }
}

void ChartPage::refreshFormulaGroup() {
    auto man = ChartMan::instance()->formulaMan();
    SkTArray<SkString> groups;
    man->groups(groups);
    for(auto s : groups) {
        ui->indi_group_->addItem(s.c_str());
    }
}

void ChartPage::on_indi_group__currentIndexChanged(const QString &group) {
    ui->indi_method_->clear();

    auto man = ChartMan::instance()->formulaMan();
    SkTArray<SkString> methods;
    man->methods(SkString(group.toUtf8().data()), methods);
    for(auto s : methods) {
        ui->indi_method_->addItem(s.c_str());
    }
}

void ChartPage::on_indi_method__currentIndexChanged(const QString &arg1) {
    ui->indi_param_->setIndicator(arg1);

    on_formula_apply__clicked();
}

void ChartPage::on_formula_apply__clicked() {
    SkString indi_name = ui->indi_param_->taName();
    formula::TaParam pa;
    if (ui->indi_param_->getParam(pa)) {
        chart_->showFormula(indi_name, pa);
    }
}

void ChartPage::on_formula_add__clicked() {
    SkString indi_name = ui->indi_param_->taName();
    formula::TaParam pa;
    if (ui->indi_param_->getParam(pa)) {
        chart_->addFormula(currentPanel(), indi_name, pa);
    }
}

////////////////////////////////////////////////////////
/// other
void ChartPage::on_other_merge_panel__clicked() {
    int n = chart_->panelCount();
    if (n >= 2) {
        chart_->mergePanel(chart_->panel(0), chart_->panel(n - 1));
    }
}
