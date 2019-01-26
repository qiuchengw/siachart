/********************************************************************************
** Form generated from reading UI file 'chartpage.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTPAGE_H
#define UI_CHARTPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "chartctrl.h"
#include "indicatorpref.h"
#include "qcolorsel.h"

QT_BEGIN_NAMESPACE

class Ui_ChartPage
{
public:
    QGridLayout *gridLayout_4;
    ChartCtrl *chart_;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *group_funcs_;
    QVBoxLayout *verticalLayout;
    QCheckBox *func_key_;
    QCheckBox *func_mouse_;
    QCheckBox *func_rubber_;
    QCheckBox *func_cross_;
    QCheckBox *func_zoom_;
    QCheckBox *func_moveview_;
    QCheckBox *func_tooltip_;
    QCheckBox *func_select_series_;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_3;
    QCheckBox *panel_hgrid_;
    QCheckBox *panel_vgrid_;
    QCheckBox *panel_middleval_;
    QDoubleSpinBox *panel_middleval_edit_;
    QSpinBox *panel_hgrid_val_;
    QSpinBox *panel_vgrid_val_;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QComboBox *series_type_;
    QPushButton *series_add_;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QComboBox *object_sel_;
    QPushButton *object_del_;
    QPushButton *add_object_;
    QComboBox *object_type_;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout_7;
    QLabel *label_6;
    QComboBox *indi_group_;
    QLabel *label_7;
    QComboBox *indi_method_;
    IndicatorPref *indi_param_;
    QPushButton *formula_apply_;
    QPushButton *formula_add_;
    QWidget *tab;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *theme_white_;
    QRadioButton *theme_black_;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_6;
    QCheckBox *sstyle_antialias_;
    QLabel *label_4;
    QCheckBox *sstyle_fillbar_;
    QDoubleSpinBox *sstyle_stroke_width_;
    QDoubleSpinBox *sstyle_bar_width_;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_5;
    QColorSel *sstyle_color_;
    QColorSel *sstyle_up_color_;
    QColorSel *sstyle_down_color_;
    QGroupBox *group_ctl_panel_;
    QVBoxLayout *verticalLayout_6;
    QCheckBox *ctl_panel_title_;
    QCheckBox *ctl_panel_valueaxis_;
    QCheckBox *ctl_panel_ratioaxis_;
    QSpacerItem *verticalSpacer;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_7;
    QPushButton *other_merge_panel_;
    QSpacerItem *verticalSpacer_3;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_5;
    QComboBox *panel_sel_;
    QPushButton *panel_del_;
    QPushButton *panel_add_;
    QPushButton *series_del_;
    QComboBox *series_sel_;

    void setupUi(QWidget *ChartPage)
    {
        if (ChartPage->objectName().isEmpty())
            ChartPage->setObjectName(QStringLiteral("ChartPage"));
        ChartPage->resize(980, 638);
        gridLayout_4 = new QGridLayout(ChartPage);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        chart_ = new ChartCtrl(ChartPage);
        chart_->setObjectName(QStringLiteral("chart_"));
        chart_->setFocusPolicy(Qt::StrongFocus);

        gridLayout_4->addWidget(chart_, 0, 0, 2, 1);

        tabWidget = new QTabWidget(ChartPage);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setMinimumSize(QSize(0, 0));
        tabWidget->setMaximumSize(QSize(250, 16777215));
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        group_funcs_ = new QGroupBox(tab_2);
        group_funcs_->setObjectName(QStringLiteral("group_funcs_"));
        verticalLayout = new QVBoxLayout(group_funcs_);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        func_key_ = new QCheckBox(group_funcs_);
        func_key_->setObjectName(QStringLiteral("func_key_"));
        func_key_->setChecked(true);

        verticalLayout->addWidget(func_key_);

        func_mouse_ = new QCheckBox(group_funcs_);
        func_mouse_->setObjectName(QStringLiteral("func_mouse_"));
        func_mouse_->setChecked(true);

        verticalLayout->addWidget(func_mouse_);

        func_rubber_ = new QCheckBox(group_funcs_);
        func_rubber_->setObjectName(QStringLiteral("func_rubber_"));
        func_rubber_->setChecked(true);

        verticalLayout->addWidget(func_rubber_);

        func_cross_ = new QCheckBox(group_funcs_);
        func_cross_->setObjectName(QStringLiteral("func_cross_"));
        func_cross_->setChecked(true);

        verticalLayout->addWidget(func_cross_);

        func_zoom_ = new QCheckBox(group_funcs_);
        func_zoom_->setObjectName(QStringLiteral("func_zoom_"));
        func_zoom_->setChecked(true);

        verticalLayout->addWidget(func_zoom_);

        func_moveview_ = new QCheckBox(group_funcs_);
        func_moveview_->setObjectName(QStringLiteral("func_moveview_"));
        func_moveview_->setChecked(true);

        verticalLayout->addWidget(func_moveview_);

        func_tooltip_ = new QCheckBox(group_funcs_);
        func_tooltip_->setObjectName(QStringLiteral("func_tooltip_"));
        func_tooltip_->setChecked(true);

        verticalLayout->addWidget(func_tooltip_);

        func_select_series_ = new QCheckBox(group_funcs_);
        func_select_series_->setObjectName(QStringLiteral("func_select_series_"));
        func_select_series_->setChecked(true);

        verticalLayout->addWidget(func_select_series_);


        verticalLayout_2->addWidget(group_funcs_);

        groupBox_4 = new QGroupBox(tab_2);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        gridLayout_3 = new QGridLayout(groupBox_4);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        panel_hgrid_ = new QCheckBox(groupBox_4);
        panel_hgrid_->setObjectName(QStringLiteral("panel_hgrid_"));

        gridLayout_3->addWidget(panel_hgrid_, 1, 0, 1, 1);

        panel_vgrid_ = new QCheckBox(groupBox_4);
        panel_vgrid_->setObjectName(QStringLiteral("panel_vgrid_"));

        gridLayout_3->addWidget(panel_vgrid_, 2, 0, 1, 1);

        panel_middleval_ = new QCheckBox(groupBox_4);
        panel_middleval_->setObjectName(QStringLiteral("panel_middleval_"));

        gridLayout_3->addWidget(panel_middleval_, 0, 0, 1, 1);

        panel_middleval_edit_ = new QDoubleSpinBox(groupBox_4);
        panel_middleval_edit_->setObjectName(QStringLiteral("panel_middleval_edit_"));
        panel_middleval_edit_->setMaximum(1e+09);
        panel_middleval_edit_->setValue(10000);

        gridLayout_3->addWidget(panel_middleval_edit_, 0, 1, 1, 1);

        panel_hgrid_val_ = new QSpinBox(groupBox_4);
        panel_hgrid_val_->setObjectName(QStringLiteral("panel_hgrid_val_"));
        panel_hgrid_val_->setValue(8);

        gridLayout_3->addWidget(panel_hgrid_val_, 1, 1, 1, 1);

        panel_vgrid_val_ = new QSpinBox(groupBox_4);
        panel_vgrid_val_->setObjectName(QStringLiteral("panel_vgrid_val_"));
        panel_vgrid_val_->setValue(6);

        gridLayout_3->addWidget(panel_vgrid_val_, 2, 1, 1, 1);


        verticalLayout_2->addWidget(groupBox_4);

        groupBox_3 = new QGroupBox(tab_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        series_type_ = new QComboBox(groupBox_3);
        series_type_->setObjectName(QStringLiteral("series_type_"));

        gridLayout_2->addWidget(series_type_, 0, 0, 1, 1);

        series_add_ = new QPushButton(groupBox_3);
        series_add_->setObjectName(QStringLiteral("series_add_"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(series_add_->sizePolicy().hasHeightForWidth());
        series_add_->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(series_add_, 0, 1, 1, 1);


        verticalLayout_2->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        object_sel_ = new QComboBox(groupBox_2);
        object_sel_->setObjectName(QStringLiteral("object_sel_"));

        gridLayout->addWidget(object_sel_, 3, 0, 1, 1);

        object_del_ = new QPushButton(groupBox_2);
        object_del_->setObjectName(QStringLiteral("object_del_"));
        sizePolicy1.setHeightForWidth(object_del_->sizePolicy().hasHeightForWidth());
        object_del_->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(object_del_, 3, 1, 1, 1);

        add_object_ = new QPushButton(groupBox_2);
        add_object_->setObjectName(QStringLiteral("add_object_"));
        sizePolicy1.setHeightForWidth(add_object_->sizePolicy().hasHeightForWidth());
        add_object_->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(add_object_, 2, 1, 1, 1);

        object_type_ = new QComboBox(groupBox_2);
        object_type_->setObjectName(QStringLiteral("object_type_"));

        gridLayout->addWidget(object_type_, 2, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        verticalLayout_5 = new QVBoxLayout(tab_3);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        label_6 = new QLabel(tab_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMaximumSize(QSize(60, 16777215));

        gridLayout_7->addWidget(label_6, 0, 0, 1, 1);

        indi_group_ = new QComboBox(tab_3);
        indi_group_->setObjectName(QStringLiteral("indi_group_"));

        gridLayout_7->addWidget(indi_group_, 0, 1, 1, 1);

        label_7 = new QLabel(tab_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMaximumSize(QSize(60, 16777215));

        gridLayout_7->addWidget(label_7, 1, 0, 1, 1);

        indi_method_ = new QComboBox(tab_3);
        indi_method_->setObjectName(QStringLiteral("indi_method_"));

        gridLayout_7->addWidget(indi_method_, 1, 1, 1, 1);


        verticalLayout_5->addLayout(gridLayout_7);

        indi_param_ = new IndicatorPref(tab_3);
        indi_param_->setObjectName(QStringLiteral("indi_param_"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(indi_param_->sizePolicy().hasHeightForWidth());
        indi_param_->setSizePolicy(sizePolicy2);

        verticalLayout_5->addWidget(indi_param_);

        formula_apply_ = new QPushButton(tab_3);
        formula_apply_->setObjectName(QStringLiteral("formula_apply_"));

        verticalLayout_5->addWidget(formula_apply_);

        formula_add_ = new QPushButton(tab_3);
        formula_add_->setObjectName(QStringLiteral("formula_add_"));

        verticalLayout_5->addWidget(formula_add_);

        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_4 = new QVBoxLayout(tab);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        theme_white_ = new QRadioButton(groupBox);
        theme_white_->setObjectName(QStringLiteral("theme_white_"));

        verticalLayout_3->addWidget(theme_white_);

        theme_black_ = new QRadioButton(groupBox);
        theme_black_->setObjectName(QStringLiteral("theme_black_"));
        theme_black_->setChecked(true);

        verticalLayout_3->addWidget(theme_black_);


        verticalLayout_4->addWidget(groupBox);

        groupBox_6 = new QGroupBox(tab);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        gridLayout_6 = new QGridLayout(groupBox_6);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        sstyle_antialias_ = new QCheckBox(groupBox_6);
        sstyle_antialias_->setObjectName(QStringLiteral("sstyle_antialias_"));

        gridLayout_6->addWidget(sstyle_antialias_, 0, 0, 1, 1);

        label_4 = new QLabel(groupBox_6);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_6->addWidget(label_4, 2, 0, 1, 1);

        sstyle_fillbar_ = new QCheckBox(groupBox_6);
        sstyle_fillbar_->setObjectName(QStringLiteral("sstyle_fillbar_"));

        gridLayout_6->addWidget(sstyle_fillbar_, 1, 0, 1, 1);

        sstyle_stroke_width_ = new QDoubleSpinBox(groupBox_6);
        sstyle_stroke_width_->setObjectName(QStringLiteral("sstyle_stroke_width_"));

        gridLayout_6->addWidget(sstyle_stroke_width_, 2, 2, 1, 1);

        sstyle_bar_width_ = new QDoubleSpinBox(groupBox_6);
        sstyle_bar_width_->setObjectName(QStringLiteral("sstyle_bar_width_"));

        gridLayout_6->addWidget(sstyle_bar_width_, 3, 2, 1, 1);

        label = new QLabel(groupBox_6);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_6->addWidget(label, 3, 0, 1, 1);

        label_2 = new QLabel(groupBox_6);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_6->addWidget(label_2, 6, 0, 1, 1);

        label_3 = new QLabel(groupBox_6);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_6->addWidget(label_3, 7, 0, 1, 1);

        label_5 = new QLabel(groupBox_6);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_6->addWidget(label_5, 5, 0, 1, 1);

        sstyle_color_ = new QColorSel(groupBox_6);
        sstyle_color_->setObjectName(QStringLiteral("sstyle_color_"));
        sstyle_color_->setMinimumSize(QSize(0, 20));
        sstyle_color_->setFrameShape(QFrame::StyledPanel);
        sstyle_color_->setFrameShadow(QFrame::Raised);

        gridLayout_6->addWidget(sstyle_color_, 5, 2, 1, 1);

        sstyle_up_color_ = new QColorSel(groupBox_6);
        sstyle_up_color_->setObjectName(QStringLiteral("sstyle_up_color_"));
        sstyle_up_color_->setMinimumSize(QSize(0, 20));
        sstyle_up_color_->setFrameShape(QFrame::StyledPanel);
        sstyle_up_color_->setFrameShadow(QFrame::Raised);

        gridLayout_6->addWidget(sstyle_up_color_, 6, 2, 1, 1);

        sstyle_down_color_ = new QColorSel(groupBox_6);
        sstyle_down_color_->setObjectName(QStringLiteral("sstyle_down_color_"));
        sstyle_down_color_->setMinimumSize(QSize(0, 20));
        sstyle_down_color_->setFrameShape(QFrame::StyledPanel);
        sstyle_down_color_->setFrameShadow(QFrame::Raised);

        gridLayout_6->addWidget(sstyle_down_color_, 7, 2, 1, 1);


        verticalLayout_4->addWidget(groupBox_6);

        group_ctl_panel_ = new QGroupBox(tab);
        group_ctl_panel_->setObjectName(QStringLiteral("group_ctl_panel_"));
        verticalLayout_6 = new QVBoxLayout(group_ctl_panel_);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        ctl_panel_title_ = new QCheckBox(group_ctl_panel_);
        ctl_panel_title_->setObjectName(QStringLiteral("ctl_panel_title_"));
        ctl_panel_title_->setChecked(true);

        verticalLayout_6->addWidget(ctl_panel_title_);

        ctl_panel_valueaxis_ = new QCheckBox(group_ctl_panel_);
        ctl_panel_valueaxis_->setObjectName(QStringLiteral("ctl_panel_valueaxis_"));
        ctl_panel_valueaxis_->setChecked(true);

        verticalLayout_6->addWidget(ctl_panel_valueaxis_);

        ctl_panel_ratioaxis_ = new QCheckBox(group_ctl_panel_);
        ctl_panel_ratioaxis_->setObjectName(QStringLiteral("ctl_panel_ratioaxis_"));
        ctl_panel_ratioaxis_->setChecked(true);

        verticalLayout_6->addWidget(ctl_panel_ratioaxis_);


        verticalLayout_4->addWidget(group_ctl_panel_);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        tabWidget->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        verticalLayout_7 = new QVBoxLayout(tab_4);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        other_merge_panel_ = new QPushButton(tab_4);
        other_merge_panel_->setObjectName(QStringLiteral("other_merge_panel_"));

        verticalLayout_7->addWidget(other_merge_panel_);

        verticalSpacer_3 = new QSpacerItem(20, 439, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_3);

        tabWidget->addTab(tab_4, QString());

        gridLayout_4->addWidget(tabWidget, 1, 1, 1, 1);

        groupBox_5 = new QGroupBox(ChartPage);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        sizePolicy1.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy1);
        groupBox_5->setMinimumSize(QSize(250, 100));
        groupBox_5->setMaximumSize(QSize(250, 16777215));
        gridLayout_5 = new QGridLayout(groupBox_5);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        panel_sel_ = new QComboBox(groupBox_5);
        panel_sel_->setObjectName(QStringLiteral("panel_sel_"));

        gridLayout_5->addWidget(panel_sel_, 0, 0, 1, 1);

        panel_del_ = new QPushButton(groupBox_5);
        panel_del_->setObjectName(QStringLiteral("panel_del_"));
        sizePolicy1.setHeightForWidth(panel_del_->sizePolicy().hasHeightForWidth());
        panel_del_->setSizePolicy(sizePolicy1);

        gridLayout_5->addWidget(panel_del_, 0, 1, 1, 1);

        panel_add_ = new QPushButton(groupBox_5);
        panel_add_->setObjectName(QStringLiteral("panel_add_"));

        gridLayout_5->addWidget(panel_add_, 0, 2, 1, 1);

        series_del_ = new QPushButton(groupBox_5);
        series_del_->setObjectName(QStringLiteral("series_del_"));
        sizePolicy1.setHeightForWidth(series_del_->sizePolicy().hasHeightForWidth());
        series_del_->setSizePolicy(sizePolicy1);

        gridLayout_5->addWidget(series_del_, 2, 2, 1, 1);

        series_sel_ = new QComboBox(groupBox_5);
        series_sel_->setObjectName(QStringLiteral("series_sel_"));

        gridLayout_5->addWidget(series_sel_, 2, 0, 1, 2);


        gridLayout_4->addWidget(groupBox_5, 0, 1, 1, 1);


        retranslateUi(ChartPage);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(ChartPage);
    } // setupUi

    void retranslateUi(QWidget *ChartPage)
    {
        ChartPage->setWindowTitle(QApplication::translate("ChartPage", "Form", nullptr));
        group_funcs_->setTitle(QApplication::translate("ChartPage", "\345\206\205\345\273\272\345\212\237\350\203\275(Built-in Functions)", nullptr));
        func_key_->setText(QApplication::translate("ChartPage", "\351\224\256\347\233\230\350\276\223\345\205\245\357\274\210KeyboardInput\357\274\211", nullptr));
        func_mouse_->setText(QApplication::translate("ChartPage", "\351\274\240\346\240\207\350\276\223\345\205\245\357\274\210MouseInput\357\274\211", nullptr));
        func_rubber_->setText(QApplication::translate("ChartPage", "\346\241\206\351\200\211\357\274\210RubberBand Select\357\274\211", nullptr));
        func_cross_->setText(QApplication::translate("ChartPage", "\345\215\201\345\255\227\347\272\277\357\274\210Crosshair\357\274\211", nullptr));
        func_zoom_->setText(QApplication::translate("ChartPage", "\351\224\256\347\233\230\347\274\251\346\224\276\357\274\210Zoom in/out\357\274\211", nullptr));
        func_moveview_->setText(QApplication::translate("ChartPage", "\351\224\256\347\233\230\347\247\273\345\212\250\350\247\206\345\233\276\357\274\210Ctrl + left/right\357\274\211", nullptr));
        func_tooltip_->setText(QApplication::translate("ChartPage", "\346\217\220\347\244\272\357\274\210Tooltip\357\274\211", nullptr));
        func_select_series_->setText(QApplication::translate("ChartPage", "\351\200\211\344\270\255\345\272\217\345\210\227\357\274\210Series Select\357\274\211", nullptr));
        groupBox_4->setTitle(QApplication::translate("ChartPage", "\351\235\242\346\235\277\357\274\210Panel\357\274\211", nullptr));
        panel_hgrid_->setText(QApplication::translate("ChartPage", "Horizon Line", nullptr));
        panel_vgrid_->setText(QApplication::translate("ChartPage", "Vert Line", nullptr));
        panel_middleval_->setText(QApplication::translate("ChartPage", "Middle Value", nullptr));
        groupBox_3->setTitle(QApplication::translate("ChartPage", "\345\272\217\345\210\227\357\274\210Series\357\274\211", nullptr));
        series_add_->setText(QApplication::translate("ChartPage", "Add Series", nullptr));
        groupBox_2->setTitle(QApplication::translate("ChartPage", "\345\257\271\350\261\241\357\274\210Object\357\274\211", nullptr));
        object_del_->setText(QApplication::translate("ChartPage", "Del Object", nullptr));
        add_object_->setText(QApplication::translate("ChartPage", "Add Object", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ChartPage", "Function", nullptr));
        label_6->setText(QApplication::translate("ChartPage", "Group:", nullptr));
        label_7->setText(QApplication::translate("ChartPage", "Method:", nullptr));
        formula_apply_->setText(QApplication::translate("ChartPage", "\346\233\264\346\226\260\346\214\207\346\240\207\345\217\202\346\225\260\357\274\210Apply Formula Param\357\274\211", nullptr));
        formula_add_->setText(QApplication::translate("ChartPage", "\346\267\273\345\212\240\346\214\207\346\240\207\357\274\210Add Formula\357\274\211", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("ChartPage", "Formula", nullptr));
        groupBox->setTitle(QApplication::translate("ChartPage", "\344\270\273\351\242\230\357\274\210Theme\357\274\211", nullptr));
        theme_white_->setText(QApplication::translate("ChartPage", "White", nullptr));
        theme_black_->setText(QApplication::translate("ChartPage", "Black", nullptr));
        groupBox_6->setTitle(QApplication::translate("ChartPage", "\345\272\217\345\210\227\351\243\216\346\240\274\357\274\210Series Style\357\274\211", nullptr));
        sstyle_antialias_->setText(QApplication::translate("ChartPage", "AntiAlias", nullptr));
        label_4->setText(QApplication::translate("ChartPage", "Stroke Width:", nullptr));
        sstyle_fillbar_->setText(QApplication::translate("ChartPage", "Fill Bar", nullptr));
        label->setText(QApplication::translate("ChartPage", "Bar Width:", nullptr));
        label_2->setText(QApplication::translate("ChartPage", "Candle Up Color:", nullptr));
        label_3->setText(QApplication::translate("ChartPage", "Candle Down Color:", nullptr));
        label_5->setText(QApplication::translate("ChartPage", "Series Color:", nullptr));
        group_ctl_panel_->setTitle(QApplication::translate("ChartPage", "\351\235\242\346\235\277\346\216\247\344\273\266\357\274\210Panel Control\357\274\211", nullptr));
        ctl_panel_title_->setText(QApplication::translate("ChartPage", "Title", nullptr));
        ctl_panel_valueaxis_->setText(QApplication::translate("ChartPage", "ValueAxis", nullptr));
        ctl_panel_ratioaxis_->setText(QApplication::translate("ChartPage", "RatioAxis", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ChartPage", "Style", nullptr));
        other_merge_panel_->setText(QApplication::translate("ChartPage", "Merge Panel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("ChartPage", "Other", nullptr));
        groupBox_5->setTitle(QApplication::translate("ChartPage", "\345\275\223\345\211\215\351\235\242\346\235\277\345\222\214\345\272\217\345\210\227", nullptr));
        panel_del_->setText(QApplication::translate("ChartPage", "Del Panel", nullptr));
        panel_add_->setText(QApplication::translate("ChartPage", "Add Panel", nullptr));
        series_del_->setText(QApplication::translate("ChartPage", "Del Series", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartPage: public Ui_ChartPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTPAGE_H
