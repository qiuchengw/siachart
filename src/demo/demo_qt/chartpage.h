#ifndef CHARTPAGE_H
#define CHARTPAGE_H

#include <QWidget>
#include <QSignalMapper>
#include <QCheckBox>
#include "mychart.h"

namespace Ui {
class ChartPage;
}

class ChartPage : public QWidget {
    Q_OBJECT

public:
    explicit ChartPage(QWidget *parent = 0);
    ~ChartPage();

    // create/init chart
    void createChart();

protected:
    // 当前的panel
    HPANEL currentPanel() const;

    // 刷新panel
    void refreshPanels();

    // 当前序列
    SkString currentSeries()const;

    // panel grid
    void setPanelGrid();

    // 刷新series
    SeriesType refreshSeries();

    // 刷新series style
    void refreshSeriesStyle();

    // 链接信号
    void initConnections();

protected slots:
    void on_funcs_toggled(QWidget* box);
    void on_ctl_panel_toggled(QWidget* box);

private slots:
    /////////////////////////////////////////////////
    /// panel
    void on_panel_add__clicked();
    void on_panel_del__clicked();
    void on_panel_sel__currentIndexChanged(int index);

    /////////////////////////////////////////////////
    /// series
    void on_series_sel__currentIndexChanged(const QString &arg1);
    void on_series_add__clicked();
    void on_series_del__clicked();

    ////////////////////////////////////////////////
    // panel::grid
    void on_panel_middleval__toggled(bool checked);
    void on_panel_middleval_edit__valueChanged(double arg1);
    void on_panel_hgrid__toggled(bool checked);
    void on_panel_vgrid__toggled(bool checked);
    void on_panel_hgrid_val__valueChanged(int arg1);
    void on_panel_vgrid_val__valueChanged(int arg1);

    ////////////////////////////////////////////
    // series style
    void on_sstyle_antialias__toggled(bool checked);
    void on_sstyle_fillbar__toggled(bool checked);
    void on_sstyle_stroke_width__valueChanged(double arg1);
    void on_sstyle_bar_width__valueChanged(double arg1);
    void on_sstyle_colorSelected(const QColor &arg1);

    ////////////////////////////////////////////
    // theme
    void on_theme_white__toggled(bool checked);
    void on_theme_black__toggled(bool checked);

    ////////////////////////////////////////////
    // indicator
    void refreshFormulaGroup();
    void on_indi_group__currentIndexChanged(const QString &arg1);
    void on_indi_method__currentIndexChanged(const QString &arg1);

    void on_formula_apply__clicked();
    void on_formula_add__clicked();

    ////////////////////////////////////////////
    // other
    void on_other_merge_panel__clicked();

private:
    Ui::ChartPage *ui;
    MyChart* chart_;
    QSignalMapper funcs_mapper_;
    QSignalMapper ctl_panel_mapper_;
};

#endif // CHARTPAGE_H
