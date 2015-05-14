/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *s_mainWindowGridLayout;
    QSpacerItem *horizontalSpacer;
    QTabWidget *m_optionsTabWidget;
    QWidget *m_flockTab;
    QGridLayout *gridLayout_4;
    QGroupBox *m_BehaviourOptions;
    QGridLayout *gridLayout;
    QCheckBox *m_fov;
    QSlider *m_separation;
    QSlider *m_alignment;
    QLabel *m_cohesionLabel;
    QLabel *m_spearatioLabel;
    QLabel *m_alignLabel;
    QLabel *m_speedLabel;
    QSlider *m_speed;
    QSlider *m_cohesion;
    QSpinBox *m_fovAngle;
    QLabel *m_fovAngleLabel;
    QLabel *m_massLabel;
    QSlider *m_mass;
    QGroupBox *m_flockOptions;
    QGridLayout *gridLayout_3;
    QPushButton *m_addBoid;
    QCheckBox *m_leader;
    QPushButton *m_newFlock;
    QSpinBox *m_flockCount;
    QCheckBox *m_predator;
    QCheckBox *m_steer;
    QPushButton *m_removeBoid;
    QWidget *m_obstacleTab;
    QGridLayout *gridLayout_5;
    QGroupBox *m_presetObstacleOptions;
    QGridLayout *gridLayout_7;
    QComboBox *m_presetObstacles;
    QGroupBox *m_customObstacleOptions;
    QGridLayout *gridLayout_2;
    QLabel *obstacleradius;
    QComboBox *m_customObstacle;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_4;
    QDoubleSpinBox *m_customEndX;
    QDoubleSpinBox *m_customEndY;
    QDoubleSpinBox *m_customEndZ;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_5;
    QDoubleSpinBox *m_customStartX;
    QDoubleSpinBox *m_customStartY;
    QDoubleSpinBox *m_customStartZ;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_6;
    QDoubleSpinBox *m_customCtrlX;
    QDoubleSpinBox *m_customCtrlY;
    QDoubleSpinBox *m_customCtrlZ;
    QPushButton *m_removeCustomObstacle;
    QPushButton *m_addCustomObstacle;
    QCheckBox *m_showCtrlPoints;
    QSpinBox *m_customObsRad;
    QGroupBox *m_obstacleOptions;
    QGridLayout *gridLayout_6;
    QPushButton *m_clearObstacle;
    QPushButton *m_removeObstacle;
    QPushButton *m_addObstacle;
    QSpacerItem *verticalSpacer;
    QWidget *m_visTab;
    QFormLayout *formLayout;
    QCheckBox *m_visOctree;
    QCheckBox *m_tails;
    QCheckBox *m_pov;
    QSpacerItem *horizontalSpacer_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(966, 578);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        s_mainWindowGridLayout = new QGridLayout(centralwidget);
        s_mainWindowGridLayout->setObjectName(QStringLiteral("s_mainWindowGridLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        m_optionsTabWidget = new QTabWidget(centralwidget);
        m_optionsTabWidget->setObjectName(QStringLiteral("m_optionsTabWidget"));
        m_optionsTabWidget->setEnabled(true);
        m_optionsTabWidget->setUsesScrollButtons(true);
        m_optionsTabWidget->setDocumentMode(false);
        m_optionsTabWidget->setTabsClosable(false);
        m_optionsTabWidget->setMovable(false);
        m_optionsTabWidget->setTabBarAutoHide(false);
        m_flockTab = new QWidget();
        m_flockTab->setObjectName(QStringLiteral("m_flockTab"));
        gridLayout_4 = new QGridLayout(m_flockTab);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        m_BehaviourOptions = new QGroupBox(m_flockTab);
        m_BehaviourOptions->setObjectName(QStringLiteral("m_BehaviourOptions"));
        gridLayout = new QGridLayout(m_BehaviourOptions);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        m_fov = new QCheckBox(m_BehaviourOptions);
        m_fov->setObjectName(QStringLiteral("m_fov"));

        gridLayout->addWidget(m_fov, 5, 0, 1, 1);

        m_separation = new QSlider(m_BehaviourOptions);
        m_separation->setObjectName(QStringLiteral("m_separation"));
        m_separation->setMaximum(300);
        m_separation->setValue(100);
        m_separation->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(m_separation, 1, 2, 1, 1);

        m_alignment = new QSlider(m_BehaviourOptions);
        m_alignment->setObjectName(QStringLiteral("m_alignment"));
        m_alignment->setMaximum(300);
        m_alignment->setValue(50);
        m_alignment->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(m_alignment, 2, 2, 1, 1);

        m_cohesionLabel = new QLabel(m_BehaviourOptions);
        m_cohesionLabel->setObjectName(QStringLiteral("m_cohesionLabel"));

        gridLayout->addWidget(m_cohesionLabel, 0, 0, 1, 1);

        m_spearatioLabel = new QLabel(m_BehaviourOptions);
        m_spearatioLabel->setObjectName(QStringLiteral("m_spearatioLabel"));

        gridLayout->addWidget(m_spearatioLabel, 1, 0, 1, 1);

        m_alignLabel = new QLabel(m_BehaviourOptions);
        m_alignLabel->setObjectName(QStringLiteral("m_alignLabel"));

        gridLayout->addWidget(m_alignLabel, 2, 0, 1, 1);

        m_speedLabel = new QLabel(m_BehaviourOptions);
        m_speedLabel->setObjectName(QStringLiteral("m_speedLabel"));

        gridLayout->addWidget(m_speedLabel, 3, 0, 1, 1);

        m_speed = new QSlider(m_BehaviourOptions);
        m_speed->setObjectName(QStringLiteral("m_speed"));
        m_speed->setMaximum(50);
        m_speed->setSingleStep(1);
        m_speed->setValue(5);
        m_speed->setSliderPosition(5);
        m_speed->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(m_speed, 3, 2, 1, 1);

        m_cohesion = new QSlider(m_BehaviourOptions);
        m_cohesion->setObjectName(QStringLiteral("m_cohesion"));
        m_cohesion->setMaximum(300);
        m_cohesion->setValue(200);
        m_cohesion->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(m_cohesion, 0, 2, 1, 1);

        m_fovAngle = new QSpinBox(m_BehaviourOptions);
        m_fovAngle->setObjectName(QStringLiteral("m_fovAngle"));
        m_fovAngle->setMaximum(180);
        m_fovAngle->setValue(100);

        gridLayout->addWidget(m_fovAngle, 6, 2, 1, 1);

        m_fovAngleLabel = new QLabel(m_BehaviourOptions);
        m_fovAngleLabel->setObjectName(QStringLiteral("m_fovAngleLabel"));

        gridLayout->addWidget(m_fovAngleLabel, 6, 0, 1, 1);

        m_massLabel = new QLabel(m_BehaviourOptions);
        m_massLabel->setObjectName(QStringLiteral("m_massLabel"));

        gridLayout->addWidget(m_massLabel, 4, 0, 1, 1);

        m_mass = new QSlider(m_BehaviourOptions);
        m_mass->setObjectName(QStringLiteral("m_mass"));
        m_mass->setMinimum(1);
        m_mass->setMaximum(30);
        m_mass->setValue(15);
        m_mass->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(m_mass, 4, 2, 1, 1);

        m_cohesion->raise();
        m_separation->raise();
        m_alignment->raise();
        m_cohesionLabel->raise();
        m_spearatioLabel->raise();
        m_alignLabel->raise();
        m_speedLabel->raise();
        m_speed->raise();
        m_fov->raise();
        m_fovAngleLabel->raise();
        m_fovAngle->raise();
        m_massLabel->raise();
        m_mass->raise();

        gridLayout_4->addWidget(m_BehaviourOptions, 1, 0, 1, 1);

        m_flockOptions = new QGroupBox(m_flockTab);
        m_flockOptions->setObjectName(QStringLiteral("m_flockOptions"));
        gridLayout_3 = new QGridLayout(m_flockOptions);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        m_addBoid = new QPushButton(m_flockOptions);
        m_addBoid->setObjectName(QStringLiteral("m_addBoid"));

        gridLayout_3->addWidget(m_addBoid, 0, 0, 1, 1);

        m_leader = new QCheckBox(m_flockOptions);
        m_leader->setObjectName(QStringLiteral("m_leader"));

        gridLayout_3->addWidget(m_leader, 0, 1, 1, 1);

        m_newFlock = new QPushButton(m_flockOptions);
        m_newFlock->setObjectName(QStringLiteral("m_newFlock"));

        gridLayout_3->addWidget(m_newFlock, 3, 1, 1, 1);

        m_flockCount = new QSpinBox(m_flockOptions);
        m_flockCount->setObjectName(QStringLiteral("m_flockCount"));
        m_flockCount->setMinimum(1);
        m_flockCount->setMaximum(500);
        m_flockCount->setValue(1);

        gridLayout_3->addWidget(m_flockCount, 3, 0, 1, 1);

        m_predator = new QCheckBox(m_flockOptions);
        m_predator->setObjectName(QStringLiteral("m_predator"));

        gridLayout_3->addWidget(m_predator, 2, 1, 1, 1);

        m_steer = new QCheckBox(m_flockOptions);
        m_steer->setObjectName(QStringLiteral("m_steer"));

        gridLayout_3->addWidget(m_steer, 1, 1, 1, 1);

        m_removeBoid = new QPushButton(m_flockOptions);
        m_removeBoid->setObjectName(QStringLiteral("m_removeBoid"));

        gridLayout_3->addWidget(m_removeBoid, 1, 0, 1, 1);


        gridLayout_4->addWidget(m_flockOptions, 0, 0, 1, 1);

        m_optionsTabWidget->addTab(m_flockTab, QString());
        m_obstacleTab = new QWidget();
        m_obstacleTab->setObjectName(QStringLiteral("m_obstacleTab"));
        gridLayout_5 = new QGridLayout(m_obstacleTab);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        m_presetObstacleOptions = new QGroupBox(m_obstacleTab);
        m_presetObstacleOptions->setObjectName(QStringLiteral("m_presetObstacleOptions"));
        gridLayout_7 = new QGridLayout(m_presetObstacleOptions);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        m_presetObstacles = new QComboBox(m_presetObstacleOptions);
        m_presetObstacles->setObjectName(QStringLiteral("m_presetObstacles"));

        gridLayout_7->addWidget(m_presetObstacles, 0, 0, 1, 1);


        gridLayout_5->addWidget(m_presetObstacleOptions, 1, 0, 1, 1);

        m_customObstacleOptions = new QGroupBox(m_obstacleTab);
        m_customObstacleOptions->setObjectName(QStringLiteral("m_customObstacleOptions"));
        gridLayout_2 = new QGridLayout(m_customObstacleOptions);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        obstacleradius = new QLabel(m_customObstacleOptions);
        obstacleradius->setObjectName(QStringLiteral("obstacleradius"));

        gridLayout_2->addWidget(obstacleradius, 6, 0, 1, 1);

        m_customObstacle = new QComboBox(m_customObstacleOptions);
        m_customObstacle->setObjectName(QStringLiteral("m_customObstacle"));
        m_customObstacle->setMaxCount(10);

        gridLayout_2->addWidget(m_customObstacle, 1, 1, 1, 1);

        label = new QLabel(m_customObstacleOptions);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, -1);
        label_3 = new QLabel(m_customObstacleOptions);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_2->addWidget(label_3);

        horizontalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        m_customEndX = new QDoubleSpinBox(m_customObstacleOptions);
        m_customEndX->setObjectName(QStringLiteral("m_customEndX"));
        m_customEndX->setMinimum(-1000);
        m_customEndX->setMaximum(1000);
        m_customEndX->setSingleStep(2);
        m_customEndX->setValue(0);

        horizontalLayout_2->addWidget(m_customEndX);

        m_customEndY = new QDoubleSpinBox(m_customObstacleOptions);
        m_customEndY->setObjectName(QStringLiteral("m_customEndY"));
        m_customEndY->setMinimum(-1000);
        m_customEndY->setMaximum(1000);
        m_customEndY->setSingleStep(2);
        m_customEndY->setValue(-150);

        horizontalLayout_2->addWidget(m_customEndY);

        m_customEndZ = new QDoubleSpinBox(m_customObstacleOptions);
        m_customEndZ->setObjectName(QStringLiteral("m_customEndZ"));
        m_customEndZ->setMinimum(-1000);
        m_customEndZ->setMaximum(1000);
        m_customEndZ->setSingleStep(2);
        m_customEndZ->setValue(-100);

        horizontalLayout_2->addWidget(m_customEndZ);


        gridLayout_2->addLayout(horizontalLayout_2, 3, 0, 1, 2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        label_2 = new QLabel(m_customObstacleOptions);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        horizontalSpacer_5 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        m_customStartX = new QDoubleSpinBox(m_customObstacleOptions);
        m_customStartX->setObjectName(QStringLiteral("m_customStartX"));
        m_customStartX->setMinimum(-1000);
        m_customStartX->setMaximum(1000);
        m_customStartX->setSingleStep(2);

        horizontalLayout->addWidget(m_customStartX);

        m_customStartY = new QDoubleSpinBox(m_customObstacleOptions);
        m_customStartY->setObjectName(QStringLiteral("m_customStartY"));
        m_customStartY->setMinimum(-1000);
        m_customStartY->setMaximum(1000);
        m_customStartY->setSingleStep(2);
        m_customStartY->setValue(-150);

        horizontalLayout->addWidget(m_customStartY);

        m_customStartZ = new QDoubleSpinBox(m_customObstacleOptions);
        m_customStartZ->setObjectName(QStringLiteral("m_customStartZ"));
        m_customStartZ->setMinimum(-1000);
        m_customStartZ->setMaximum(1000);
        m_customStartZ->setSingleStep(2);
        m_customStartZ->setValue(100);

        horizontalLayout->addWidget(m_customStartZ);


        gridLayout_2->addLayout(horizontalLayout, 2, 0, 1, 2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 0, -1, -1);
        label_4 = new QLabel(m_customObstacleOptions);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_3->addWidget(label_4);

        horizontalSpacer_6 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        m_customCtrlX = new QDoubleSpinBox(m_customObstacleOptions);
        m_customCtrlX->setObjectName(QStringLiteral("m_customCtrlX"));
        m_customCtrlX->setMinimum(-1000);
        m_customCtrlX->setMaximum(1000);
        m_customCtrlX->setSingleStep(2);

        horizontalLayout_3->addWidget(m_customCtrlX);

        m_customCtrlY = new QDoubleSpinBox(m_customObstacleOptions);
        m_customCtrlY->setObjectName(QStringLiteral("m_customCtrlY"));
        m_customCtrlY->setMinimum(-1000);
        m_customCtrlY->setMaximum(1000);
        m_customCtrlY->setSingleStep(2);
        m_customCtrlY->setValue(100);

        horizontalLayout_3->addWidget(m_customCtrlY);

        m_customCtrlZ = new QDoubleSpinBox(m_customObstacleOptions);
        m_customCtrlZ->setObjectName(QStringLiteral("m_customCtrlZ"));
        m_customCtrlZ->setMinimum(-1000);
        m_customCtrlZ->setMaximum(1000);
        m_customCtrlZ->setSingleStep(2);

        horizontalLayout_3->addWidget(m_customCtrlZ);


        gridLayout_2->addLayout(horizontalLayout_3, 4, 0, 1, 2);

        m_removeCustomObstacle = new QPushButton(m_customObstacleOptions);
        m_removeCustomObstacle->setObjectName(QStringLiteral("m_removeCustomObstacle"));

        gridLayout_2->addWidget(m_removeCustomObstacle, 0, 1, 1, 1);

        m_addCustomObstacle = new QPushButton(m_customObstacleOptions);
        m_addCustomObstacle->setObjectName(QStringLiteral("m_addCustomObstacle"));

        gridLayout_2->addWidget(m_addCustomObstacle, 0, 0, 1, 1);

        m_showCtrlPoints = new QCheckBox(m_customObstacleOptions);
        m_showCtrlPoints->setObjectName(QStringLiteral("m_showCtrlPoints"));

        gridLayout_2->addWidget(m_showCtrlPoints, 8, 0, 1, 1);

        m_customObsRad = new QSpinBox(m_customObstacleOptions);
        m_customObsRad->setObjectName(QStringLiteral("m_customObsRad"));
        m_customObsRad->setValue(10);

        gridLayout_2->addWidget(m_customObsRad, 6, 1, 1, 1);


        gridLayout_5->addWidget(m_customObstacleOptions, 2, 0, 1, 1);

        m_obstacleOptions = new QGroupBox(m_obstacleTab);
        m_obstacleOptions->setObjectName(QStringLiteral("m_obstacleOptions"));
        gridLayout_6 = new QGridLayout(m_obstacleOptions);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        m_clearObstacle = new QPushButton(m_obstacleOptions);
        m_clearObstacle->setObjectName(QStringLiteral("m_clearObstacle"));

        gridLayout_6->addWidget(m_clearObstacle, 1, 0, 1, 2);

        m_removeObstacle = new QPushButton(m_obstacleOptions);
        m_removeObstacle->setObjectName(QStringLiteral("m_removeObstacle"));

        gridLayout_6->addWidget(m_removeObstacle, 0, 1, 1, 1);

        m_addObstacle = new QPushButton(m_obstacleOptions);
        m_addObstacle->setObjectName(QStringLiteral("m_addObstacle"));

        gridLayout_6->addWidget(m_addObstacle, 0, 0, 1, 1);


        gridLayout_5->addWidget(m_obstacleOptions, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 3, 0, 1, 1);

        m_optionsTabWidget->addTab(m_obstacleTab, QString());
        m_visTab = new QWidget();
        m_visTab->setObjectName(QStringLiteral("m_visTab"));
        formLayout = new QFormLayout(m_visTab);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        m_visOctree = new QCheckBox(m_visTab);
        m_visOctree->setObjectName(QStringLiteral("m_visOctree"));

        formLayout->setWidget(0, QFormLayout::FieldRole, m_visOctree);

        m_tails = new QCheckBox(m_visTab);
        m_tails->setObjectName(QStringLiteral("m_tails"));

        formLayout->setWidget(1, QFormLayout::FieldRole, m_tails);

        m_pov = new QCheckBox(m_visTab);
        m_pov->setObjectName(QStringLiteral("m_pov"));

        formLayout->setWidget(2, QFormLayout::FieldRole, m_pov);

        m_optionsTabWidget->addTab(m_visTab, QString());

        s_mainWindowGridLayout->addWidget(m_optionsTabWidget, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 966, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        m_optionsTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Flock", 0));
#ifndef QT_NO_TOOLTIP
        m_flockTab->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        m_BehaviourOptions->setTitle(QApplication::translate("MainWindow", "Boid Behaviours", 0));
        m_fov->setText(QApplication::translate("MainWindow", "Field of View", 0));
        m_cohesionLabel->setText(QApplication::translate("MainWindow", "Cohesion", 0));
        m_spearatioLabel->setText(QApplication::translate("MainWindow", "Separation", 0));
        m_alignLabel->setText(QApplication::translate("MainWindow", "Alignment", 0));
        m_speedLabel->setText(QApplication::translate("MainWindow", "Speed", 0));
        m_fovAngleLabel->setText(QApplication::translate("MainWindow", "Field of View Angle", 0));
        m_massLabel->setText(QApplication::translate("MainWindow", "Mass", 0));
        m_flockOptions->setTitle(QApplication::translate("MainWindow", "Flock", 0));
        m_addBoid->setText(QApplication::translate("MainWindow", "Add Boid", 0));
        m_leader->setText(QApplication::translate("MainWindow", "Leader", 0));
        m_newFlock->setText(QApplication::translate("MainWindow", "Create New Flock", 0));
        m_predator->setText(QApplication::translate("MainWindow", "Predator", 0));
        m_steer->setText(QApplication::translate("MainWindow", "Steerable Leader", 0));
        m_removeBoid->setText(QApplication::translate("MainWindow", "Remove Boid", 0));
        m_optionsTabWidget->setTabText(m_optionsTabWidget->indexOf(m_flockTab), QApplication::translate("MainWindow", "Flock", 0));
        m_presetObstacleOptions->setTitle(QApplication::translate("MainWindow", "Preset Courses", 0));
        m_presetObstacles->clear();
        m_presetObstacles->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "None", 0)
         << QApplication::translate("MainWindow", "Crecents", 0)
         << QApplication::translate("MainWindow", "Horn", 0)
         << QApplication::translate("MainWindow", "Boromean Rings", 0)
         << QApplication::translate("MainWindow", "Wavey Tunnel", 0)
        );
        m_presetObstacles->setCurrentText(QApplication::translate("MainWindow", "None", 0));
        m_customObstacleOptions->setTitle(QApplication::translate("MainWindow", "Custom Obstacles", 0));
        obstacleradius->setText(QApplication::translate("MainWindow", "Obstacle Radius", 0));
        label->setText(QApplication::translate("MainWindow", "Custom Obstacle number", 0));
        label_3->setText(QApplication::translate("MainWindow", "End point (x,y,z", 0));
        label_2->setText(QApplication::translate("MainWindow", "Start point (x,y,z)", 0));
        label_4->setText(QApplication::translate("MainWindow", "Control Point (x,y,z)", 0));
        m_removeCustomObstacle->setText(QApplication::translate("MainWindow", "Remove Custom Obstacle", 0));
        m_addCustomObstacle->setText(QApplication::translate("MainWindow", "Add Custom Obstacle", 0));
        m_showCtrlPoints->setText(QApplication::translate("MainWindow", "Show Control Point", 0));
        m_obstacleOptions->setTitle(QApplication::translate("MainWindow", "Obstacles", 0));
        m_clearObstacle->setText(QApplication::translate("MainWindow", "Clear Obstacles", 0));
        m_removeObstacle->setText(QApplication::translate("MainWindow", "Remove Obstacle", 0));
        m_addObstacle->setText(QApplication::translate("MainWindow", "Add Obstacle", 0));
        m_optionsTabWidget->setTabText(m_optionsTabWidget->indexOf(m_obstacleTab), QApplication::translate("MainWindow", "Obstacle", 0));
        m_visOctree->setText(QApplication::translate("MainWindow", "Visualise Octree", 0));
        m_tails->setText(QApplication::translate("MainWindow", "Trails", 0));
        m_pov->setText(QApplication::translate("MainWindow", "Point of View", 0));
        m_optionsTabWidget->setTabText(m_optionsTabWidget->indexOf(m_visTab), QApplication::translate("MainWindow", "Visualisation", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
