#include "Window.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QMetaObject>
#include <QPixmap>
#include <QBrush>
#include <QPalette>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QFile>
#include <QDebug>

const QString MAIN_STYLE = R"(
    QMainWindow {
        background: qradialgradient(cx: 0.5, cy: 0.5, radius: 1.0, fx: 0.5, fy: 0.5, stop: 0 #1a4c28, stop: 1 #0d2614);
    }
    QLabel {
        color: #E0E0E0;
        font-family: 'Segoe UI', sans-serif;
    }
)";

const QString PANEL_STYLE = R"(
    QWidget {
        background-color: rgba(20, 30, 25, 200);
        border: 1px solid #D4AF37;
        border-radius: 15px;
    }
)";

const QString BUTTON_STYLE = R"(
    QPushButton {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4a4a4a, stop:1 #2b2b2b);
        color: white;
        border: 1px solid #555;
        border-radius: 8px;
        padding: 8px;
        font-weight: bold;
        font-size: 13px;
    }
    QPushButton:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5a5a5a, stop:1 #3b3b3b);
        border: 1px solid #D4AF37;
    }
    QPushButton:pressed {
        background-color: #1a1a1a;
    }
    QPushButton:disabled {
        background-color: #1a1a1a;
        color: #555;
        border: 1px solid #333;
    }
)";

const QString ACTION_BTN_STYLE = R"(
    QPushButton {
        border-radius: 10px;
        font-size: 14px;
        font-weight: 900;
        padding: 12px;
        color: white;
    }
)";

const QString HIT_STYLE = ACTION_BTN_STYLE + "QPushButton { background-color: #2E8B57; border: 2px solid #3CB371; } QPushButton:hover { background-color: #3CB371; }";
const QString STAND_STYLE = ACTION_BTN_STYLE + "QPushButton { background-color: #B22222; border: 2px solid #CD5C5C; } QPushButton:hover { background-color: #CD5C5C; }";
const QString SPECIAL_STYLE = ACTION_BTN_STYLE + "QPushButton { background-color: #DAA520; color: #111; border: 2px solid #FFD700; } QPushButton:hover { background-color: #FFD700; }";

const QString SPINBOX_STYLE = R"(
    QSpinBox {
        background-color: rgba(0, 0, 0, 150);
        color: #FFD700;
        border: 1px solid #D4AF37;
        border-radius: 5px;
        padding: 5px;
        font-size: 14px;
        font-weight: bold;
    }
    QSpinBox::up-button, QSpinBox::down-button { width: 0px; }
)";

const QString SLIDER_STYLE = R"(
    QSlider::groove:horizontal {
        border: 1px solid #555;
        height: 6px;
        background: rgba(0,0,0,0.5);
        margin: 2px 0;
        border-radius: 3px;
    }
    QSlider::handle:horizontal {
        background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #FFD700, stop:1 #B8860B);
        border: 1px solid #5c5c5c;
        width: 16px;
        height: 16px;
        margin: -6px 0;
        border-radius: 8px;
    }
    QSlider::handle:horizontal:hover {
        background: #FFFACD;
    }
)";

const QString CHECKBOX_STYLE = R"(
    QCheckBox { color: #DDD; font-size: 12px; spacing: 8px; }
    QCheckBox::indicator { width: 18px; height: 18px; border-radius: 3px; border: 1px solid #777; background: rgba(0,0,0,0.5); }
    QCheckBox::indicator:checked { background-color: #D4AF37; border-color: #FFD700; image: url(:/resource/Images/check.png); } /* Якщо немає іконки, буде просто жовтий */
)";


Window::Window(QWidget* parent) : QMainWindow(parent)
{
    bgMusic = new QMediaPlayer(this);
    bgAudio = new QAudioOutput(this);
    bgMusic->setAudioOutput(bgAudio);

    buttonSound = new QSoundEffect(this);
    cardSound = new QSoundEffect(this);

    bgMusic->setSource(QUrl("qrc:/resource/Sounds/background.mp3"));
    bgAudio->setVolume(0.15);
    bgMusic->setLoops(QMediaPlayer::Infinite);
    bgMusic->play();

    buttonSound->setSource(QUrl("qrc:/resource/Sounds/button.wav"));
    buttonSound->setVolume(0.6);

    cardSound->setSource(QUrl("qrc:/resource/Sounds/card.wav"));
    cardSound->setVolume(0.7);

    CreateUI();
}

Window::~Window()
{
    ClearCards();
}

void Window::closeEvent(QCloseEvent* event)
{
    if (bgMusic) {
        bgMusic->stop();
    }
    QMainWindow::closeEvent(event);
}

void Window::CreateUI()
{
    setWindowTitle("BlackJack Royal");
    setFixedSize(1280, 720);
    setStyleSheet(MAIN_STYLE);

    QPixmap background(":/resource/Images/BackGround.jpg");
    if (!background.isNull()) {
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(background.scaled(1280, 720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        setPalette(palette);
    }

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* mainHLayout = new QHBoxLayout(central);
    mainHLayout->setSpacing(25);
    mainHLayout->setContentsMargins(30, 20, 30, 20);

    QWidget* leftWidget = new QWidget(this);
    QVBoxLayout* gameLayout = new QVBoxLayout(leftWidget);
    gameLayout->setSpacing(15);
    gameLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout* topInfoLayout = new QHBoxLayout();

    QFrame* balanceFrame = new QFrame(this);
    balanceFrame->setStyleSheet("background-color: rgba(0, 0, 0, 180); border: 1px solid #D4AF37; border-radius: 20px; padding: 5px;");
    balanceFrame->setFixedSize(250, 50);
    QHBoxLayout* balanceLayout = new QHBoxLayout(balanceFrame);

    QLabel* balanceIcon = new QLabel("$", this);
    balanceIcon->setStyleSheet("font-size: 20px; border: none; background: transparent;");

    balanceLabel = new QLabel("$1000", this);
    balanceLabel->setStyleSheet("color: #FFD700; font-size: 22px; font-weight: bold; border: none; background: transparent;");
    balanceLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    balanceLayout->addWidget(balanceIcon);
    balanceLayout->addStretch();
    balanceLayout->addWidget(balanceLabel);

    topInfoLayout->addStretch();
    topInfoLayout->addWidget(balanceFrame);
    gameLayout->addLayout(topInfoLayout);

    QLabel* dealerTitle = new QLabel("DEALER", this);
    dealerTitle->setStyleSheet("color: rgba(255,255,255,0.7); font-size: 14px; font-weight: bold; letter-spacing: 2px;");
    dealerTitle->setAlignment(Qt::AlignCenter);
    gameLayout->addWidget(dealerTitle);

    dealerArea = new QWidget(this);
    dealerArea->setFixedHeight(160);
    gameLayout->addWidget(dealerArea);

    dealerValueLabel = new QLabel("", dealerArea);
    dealerValueLabel->setStyleSheet("color: #222; font-size: 14px; font-weight: bold; background-color: #FFD700; padding: 4px 10px; border-radius: 10px;");
    dealerValueLabel->setGeometry(10, 10, 80, 30);
    dealerValueLabel->hide();

    QLabel* playerTitle = new QLabel("YOUR HAND", this);
    playerTitle->setStyleSheet("color: rgba(255,255,255,0.7); font-size: 14px; font-weight: bold; letter-spacing: 2px;");
    playerTitle->setAlignment(Qt::AlignCenter);
    gameLayout->addWidget(playerTitle);

    playerArea = new QWidget(this);
    playerArea->setFixedHeight(180);
    gameLayout->addWidget(playerArea);

    QFrame* controlsPanel = new QFrame(this);
    controlsPanel->setStyleSheet("background-color: rgba(0, 0, 0, 100); border-radius: 15px;");
    QHBoxLayout* controlsLayout = new QHBoxLayout(controlsPanel);
    controlsLayout->setContentsMargins(20, 15, 20, 15);

    QVBoxLayout* betControlLayout = new QVBoxLayout();
    QLabel* betText = new QLabel("BET AMOUNT", this);
    betText->setStyleSheet("color: #AAA; font-size: 10px; font-weight: bold;");
    betText->setAlignment(Qt::AlignCenter);

    betSpinBox = new QSpinBox(this);
    betSpinBox->setRange(1, 1000);
    betSpinBox->setValue(100);
    betSpinBox->setFixedWidth(100);
    betSpinBox->setFixedHeight(40);
    betSpinBox->setStyleSheet(SPINBOX_STYLE);
    betSpinBox->setAlignment(Qt::AlignCenter);

    betControlLayout->addWidget(betText);
    betControlLayout->addWidget(betSpinBox);
    controlsLayout->addLayout(betControlLayout);

    betButton = new QPushButton("PLACE BET", this);
    betButton->setStyleSheet(SPECIAL_STYLE);
    betButton->setCursor(Qt::PointingHandCursor);
    betButton->setFixedWidth(140);
    betButton->setFixedHeight(50);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(255, 215, 0, 100));
    shadow->setOffset(0, 0);
    betButton->setGraphicsEffect(shadow);

    controlsLayout->addSpacing(15);
    controlsLayout->addWidget(betButton);

    betLabel = new QLabel("", this);
    betLabel->setStyleSheet("color: #FFD700; font-size: 16px; font-weight: bold; margin-left: 10px;");
    controlsLayout->addWidget(betLabel);

    controlsLayout->addStretch();

    hitButton = new QPushButton("HIT", this);
    hitButton->setStyleSheet(HIT_STYLE);
    hitButton->setCursor(Qt::PointingHandCursor);
    hitButton->setFixedWidth(100);
    hitButton->setFixedHeight(50);
    hitButton->setEnabled(false);

    standButton = new QPushButton("STAND", this);
    standButton->setStyleSheet(STAND_STYLE);
    standButton->setCursor(Qt::PointingHandCursor);
    standButton->setFixedWidth(100);
    standButton->setFixedHeight(50);
    standButton->setEnabled(false);

    doubleDownButton = new QPushButton("DOUBLE", this);
    doubleDownButton->setStyleSheet(BUTTON_STYLE);
    doubleDownButton->setCursor(Qt::PointingHandCursor);
    doubleDownButton->setFixedWidth(90);
    doubleDownButton->setFixedHeight(40);
    doubleDownButton->setEnabled(false);

    splitButton = new QPushButton("SPLIT", this);
    splitButton->setStyleSheet(BUTTON_STYLE);
    splitButton->setCursor(Qt::PointingHandCursor);
    splitButton->setFixedWidth(90);
    splitButton->setFixedHeight(40);
    splitButton->setEnabled(false);

    QHBoxLayout* gameActionsLayout = new QHBoxLayout();
    gameActionsLayout->addWidget(doubleDownButton);
    gameActionsLayout->addWidget(splitButton);
    gameActionsLayout->addSpacing(15);
    gameActionsLayout->addWidget(hitButton);
    gameActionsLayout->addWidget(standButton);

    controlsLayout->addLayout(gameActionsLayout);

    gameLayout->addStretch();
    gameLayout->addWidget(controlsPanel);


    QFrame* rightWidget = new QFrame(this);
    rightWidget->setFixedWidth(280);
    rightWidget->setStyleSheet(PANEL_STYLE);

    QVBoxLayout* settingsLayout = new QVBoxLayout(rightWidget);
    settingsLayout->setSpacing(15);
    settingsLayout->setContentsMargins(20, 25, 20, 25);

    QLabel* settingsTitle = new QLabel("Налаштування", this);
    settingsTitle->setStyleSheet("color: #FFD700; font-size: 18px; font-weight: bold; background: transparent; border: none;");
    settingsTitle->setAlignment(Qt::AlignCenter);
    settingsLayout->addWidget(settingsTitle);

    QFrame* line1 = new QFrame();
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet("color: #555; background: transparent; border: none; border-bottom: 1px solid #555;");
    settingsLayout->addWidget(line1);

    QLabel* rulesLabel = new QLabel("Параметри", this);
    rulesLabel->setStyleSheet("color: #AAA; font-size: 12px; font-weight: bold; background: transparent; border: none;");
    settingsLayout->addWidget(rulesLabel);

    doubleDownCheckBox = new QCheckBox("Double Down", this);
    doubleDownCheckBox->setChecked(true);
    doubleDownCheckBox->setStyleSheet(CHECKBOX_STYLE);
    settingsLayout->addWidget(doubleDownCheckBox);

    blackjackPayoutCheckBox = new QCheckBox("3:2 Blackjack Payout", this);
    blackjackPayoutCheckBox->setChecked(false);
    blackjackPayoutCheckBox->setStyleSheet(CHECKBOX_STYLE);
    settingsLayout->addWidget(blackjackPayoutCheckBox);

    perfectPairsCheckBox = new QCheckBox("Perfect Pairs Side Bet", this);
    perfectPairsCheckBox->setChecked(true);
    perfectPairsCheckBox->setStyleSheet(CHECKBOX_STYLE);
    settingsLayout->addWidget(perfectPairsCheckBox);

    QFrame* line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet("color: #555; background: transparent; border: none; border-bottom: 1px solid #555;");
    settingsLayout->addWidget(line2);

    QLabel* visualLabel = new QLabel("Скіни", this);
    visualLabel->setStyleSheet("color: #AAA; font-size: 12px; font-weight: bold; background: transparent; border: none;");
    settingsLayout->addWidget(visualLabel);

    skinSlider = new QSlider(Qt::Horizontal, this);
    skinSlider->setRange(1, 2);
    skinSlider->setValue(2);
    skinSlider->setStyleSheet(SLIDER_STYLE);
    settingsLayout->addWidget(skinSlider);

    QHBoxLayout* skinLabels = new QHBoxLayout();
    QLabel* l1 = new QLabel("Звичайний", this); l1->setStyleSheet("color: #888; font-size: 10px; border:none; background:transparent;");
    QLabel* l2 = new QLabel("ТЕСТ", this); l2->setStyleSheet("color: #888; font-size: 10px; border:none; background:transparent;");
    skinLabels->addWidget(l1);
    skinLabels->addStretch();
    skinLabels->addWidget(l2);
    settingsLayout->addLayout(skinLabels);

    QFrame* line3 = new QFrame();
    line3->setFrameShape(QFrame::HLine);
    line3->setStyleSheet("color: #555; background: transparent; border: none; border-bottom: 1px solid #555;");
    settingsLayout->addWidget(line3);

    QLabel* audioLabel = new QLabel("Аудіо", this);
    audioLabel->setStyleSheet("color: #AAA; font-size: 12px; font-weight: bold; background: transparent; border: none;");
    settingsLayout->addWidget(audioLabel);

    QLabel* musicLabel = new QLabel("Музика", this);
    musicLabel->setStyleSheet("color: #DDD; font-size: 11px; border:none; background:transparent;");
    settingsLayout->addWidget(musicLabel);

    QSlider* musicSlider = new QSlider(Qt::Horizontal, this);
    musicSlider->setRange(0, 100);
    musicSlider->setValue(bgAudio->volume() * 100);
    musicSlider->setStyleSheet(SLIDER_STYLE);
    settingsLayout->addWidget(musicSlider);

    connect(musicSlider, &QSlider::valueChanged, this, [this](int value) {
        bgAudio->setVolume(value / 100.0);
        });

    QLabel* sfxLabel = new QLabel("Звуки", this);
    sfxLabel->setStyleSheet("color: #DDD; font-size: 11px; border:none; background:transparent;");
    settingsLayout->addWidget(sfxLabel);

    QSlider* sfxSlider = new QSlider(Qt::Horizontal, this);
    sfxSlider->setRange(0, 100);
    sfxSlider->setValue(buttonSound->volume() * 100);
    sfxSlider->setStyleSheet(SLIDER_STYLE);
    settingsLayout->addWidget(sfxSlider);

    connect(sfxSlider, &QSlider::valueChanged, this, [this](int value) {
        qreal vol = value / 100.0;
        buttonSound->setVolume(vol);
        cardSound->setVolume(vol);
        });

    settingsLayout->addStretch();
    QLabel* seedLabel = new QLabel("Сід", this);
    seedLabel->setStyleSheet("color: #AAA; font-size: 12px; font-weight: bold; background: transparent; border: none;");
    settingsLayout->addWidget(seedLabel);

    QHBoxLayout* seedLayout = new QHBoxLayout();
    seedSpinBox = new QSpinBox(this);
    seedSpinBox->setRange(0, 2147483647);
    seedSpinBox->setValue(0);
    seedSpinBox->setStyleSheet(SPINBOX_STYLE);

    setSeedButton = new QPushButton("Застосувати", this);
    setSeedButton->setStyleSheet(BUTTON_STYLE);
    setSeedButton->setFixedWidth(60);

    seedLayout->addWidget(seedSpinBox);
    seedLayout->addWidget(setSeedButton);
    settingsLayout->addLayout(seedLayout);

    mainHLayout->addWidget(leftWidget, 1);
    mainHLayout->addWidget(rightWidget);

    connect(betButton, &QPushButton::clicked, this, &Window::on_BetButton_clicked);
    connect(hitButton, &QPushButton::clicked, this, &Window::on_HitButton_clicked);
    connect(standButton, &QPushButton::clicked, this, &Window::on_StandButton_clicked);
    connect(doubleDownButton, &QPushButton::clicked, this, &Window::on_DoubleDownButton_clicked);
    connect(splitButton, &QPushButton::clicked, this, &Window::on_SplitButton_clicked);
    connect(doubleDownCheckBox, &QCheckBox::checkStateChanged, this, &Window::on_DoubleDownCheckBox_changed);
    connect(blackjackPayoutCheckBox, &QCheckBox::checkStateChanged, this, &Window::on_BlackjackPayoutCheckBox_changed);
    connect(perfectPairsCheckBox, &QCheckBox::checkStateChanged, this, &Window::on_PerfectPairsCheckBox_changed);
    connect(skinSlider, &QSlider::valueChanged, this, &Window::on_SkinSlider_changed);
    connect(setSeedButton, &QPushButton::clicked, this, &Window::on_SetSeedButton_clicked);
}

void Window::DrawCard(int x, int y, Card card, QWidget* parent)
{
    QLabel* cardLabel = new QLabel(parent);
    QString skinFile = (currentSkin == 1) ? ":/resource/Images/Skin1.png" : ":/resource/Images/Skin2.png";
    QPixmap cardPixmap(skinFile);

    int posX = (card.value + 1) * 124;
    if (card.value == 12) posX = 0;
    int posY = card.suit * 173;

    QRect rect(posX, posY, 124, 173);
    cardLabel->setPixmap(cardPixmap.copy(rect));
    cardLabel->setGeometry(x, y, 124, 173);
    cardLabel->show();
}

void Window::SetBalance(int balance)
{
    QMetaObject::invokeMethod(this, [this, balance]() {
        balanceLabel->setText(QString("$%1").arg(balance));
        }, Qt::QueuedConnection);
}

void Window::SetDealerHand(std::vector<Card> DealerHand, bool hideSecond)
{
    QMetaObject::invokeMethod(this, "UpdateDealerCards", Qt::QueuedConnection,
        Q_ARG(std::vector<Card>, DealerHand),
        Q_ARG(bool, hideSecond));
}

void Window::UpdateDealerCards(std::vector<Card> DealerHand, bool hideSecond)
{
    int cardWidth = 90;
    int cardHeight = 130;
    int spacing = 30;
    int startX = (dealerArea->width() - (DealerHand.size() * spacing + cardWidth)) / 2;
    if (startX < 20) startX = 20;

    if (dealerCards.size() > DealerHand.size()) {
        for (size_t i = DealerHand.size(); i < dealerCards.size(); ++i) {
            delete dealerCards[i];
        }
        dealerCards.resize(DealerHand.size());
        dealerCardSkins.resize(DealerHand.size());
    }

    for (size_t i = 0; i < DealerHand.size(); ++i) {
        bool isNewCard = (i >= prevDealerCardCount);
        QLabel* cardLabel = nullptr;

        if (!isNewCard && i < dealerCards.size()) {
            cardLabel = dealerCards[i];
        }
        else {
            cardLabel = new QLabel(dealerArea);
            QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
            shadow->setBlurRadius(10);
            shadow->setColor(QColor(0, 0, 0, 150));
            shadow->setOffset(2, 2);
            cardLabel->setGraphicsEffect(shadow);

            cardLabel->setScaledContents(true);
            cardLabel->setFixedSize(cardWidth, cardHeight);

            if (i >= dealerCards.size()) {
                dealerCards.push_back(cardLabel);
                dealerCardSkins.push_back(currentSkin);
            }
            else {
                dealerCards[i] = cardLabel;
                dealerCardSkins[i] = currentSkin;
            }
        }

        int skinToUse = dealerCardSkins[i];
        QString skinFile = (skinToUse == 1) ? ":/resource/Images/Skin1.png" : ":/resource/Images/Skin2.png";
        QPixmap cardPixmap(skinFile);

        QPixmap finalPixmap;
        if (i == 1 && hideSecond) {
            int posX = 1620;
            int posY = 0;
            finalPixmap = cardPixmap.copy(posX, posY, 124, 173);
        }
        else {
            Card& card = DealerHand[i];
            int posX = (card.value + 1) * 124;
            if (card.value == 12) posX = 0;
            int posY = card.suit * 173;
            finalPixmap = cardPixmap.copy(posX, posY, 124, 173);
        }

        cardLabel->setPixmap(finalPixmap);

        int endX = startX + i * (cardWidth * 0.4);
        int endY = 15;

        if (isNewCard) {
            AnimateCardAppearance(cardLabel, -200, -200, endX, endY);
        }
        else {
            cardLabel->move(endX, endY);
        }

        cardLabel->show();
        cardLabel->raise();
    }

    prevDealerCardCount = DealerHand.size();
}

void Window::SetPlayerHands(std::vector<std::vector<Card>> PlayerHands)
{
    QMetaObject::invokeMethod(this, "UpdatePlayerCards", Qt::QueuedConnection,
        Q_ARG(std::vector<std::vector<Card>>, PlayerHands));
}

void Window::ShowHandValues(std::vector<std::vector<int>> handValues)
{
    QMetaObject::invokeMethod(this, "DisplayHandValues", Qt::QueuedConnection,
        Q_ARG(std::vector<std::vector<int>>, handValues));
}

void Window::ShowDealerValue(int value)
{
    QMetaObject::invokeMethod(this, "DisplayDealerValue", Qt::QueuedConnection,
        Q_ARG(int, value));
}

void Window::DisplayDealerValue(int value)
{
    dealerValueLabel->setText(QString("%1").arg(value));
    dealerValueLabel->show();
    if (!dealerCards.empty()) {
        int lastCardX = dealerCards.back()->x();
        dealerValueLabel->move(lastCardX + 100, 60);
    }
}

void Window::DisplayHandValues(std::vector<std::vector<int>> handValues)
{
    for (auto label : handValueLabels) {
        delete label;
    }
    handValueLabels.clear();

    int totalHands = handValues.size();
    int areaWidth = playerArea->width();
    int sectionWidth = areaWidth / (totalHands > 0 ? totalHands : 1);

    for (size_t i = 0; i < handValues.size(); ++i) {
        QString valueStr = "";
        if (handValues[i].size() == 1) {
            valueStr = QString::number(handValues[i][0]);
        }
        else if (handValues[i].size() == 2) {
            valueStr = QString::number(handValues[i][0]) + " / " + QString::number(handValues[i][1]);
        }

        QLabel* valueLabel = new QLabel(valueStr, playerArea);
        valueLabel->setStyleSheet("color: #222; font-size: 12px; font-weight: bold; background-color: #FFD700; padding: 3px 8px; border-radius: 8px;");
        valueLabel->adjustSize();

        int centerX = (i * sectionWidth) + (sectionWidth / 2) - (valueLabel->width() / 2);
        valueLabel->move(centerX, 140);
        valueLabel->show();
        handValueLabels.push_back(valueLabel);
    }
}

void Window::UpdatePlayerCards(std::vector<std::vector<Card>> PlayerHands)
{
    int startY = 10;
    int cardWidth = 90;
    int cardHeight = 130;

    if (prevPlayerCardCounts.size() < PlayerHands.size()) {
        prevPlayerCardCounts.resize(PlayerHands.size(), 0);
        playerCardSkins.resize(PlayerHands.size());
    }

    while (playerCards.size() > PlayerHands.size()) {
        for (auto label : playerCards.back()) {
            delete label;
        }
        playerCards.pop_back();
        prevPlayerCardCounts.pop_back();
        playerCardSkins.pop_back();
    }

    int totalHands = PlayerHands.size();
    int areaWidth = playerArea->width();
    int sectionWidth = areaWidth / (totalHands > 0 ? totalHands : 1);

    for (size_t handIdx = 0; handIdx < PlayerHands.size(); ++handIdx) {
        if (handIdx >= playerCards.size()) {
            playerCards.push_back(std::vector<QLabel*>());
            playerCardSkins.push_back(std::vector<int>());
            prevPlayerCardCounts[handIdx] = 0;
        }

        while (playerCards[handIdx].size() > PlayerHands[handIdx].size()) {
            delete playerCards[handIdx].back();
            playerCards[handIdx].pop_back();
            playerCardSkins[handIdx].pop_back();
        }

        int sectionCenter = (handIdx * sectionWidth) + (sectionWidth / 2);
        int handWidth = (PlayerHands[handIdx].size() - 1) * 35 + cardWidth;
        int startX = sectionCenter - (handWidth / 2);

        for (size_t cardIdx = 0; cardIdx < PlayerHands[handIdx].size(); ++cardIdx) {
            bool isNewCard = (cardIdx >= prevPlayerCardCounts[handIdx]);
            QLabel* cardLabel = nullptr;

            if (!isNewCard && cardIdx < playerCards[handIdx].size()) {
                cardLabel = playerCards[handIdx][cardIdx];
            }
            else {
                cardLabel = new QLabel(playerArea);

                QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
                shadow->setBlurRadius(10);
                shadow->setColor(QColor(0, 0, 0, 150));
                shadow->setOffset(2, 2);
                cardLabel->setGraphicsEffect(shadow);

                cardLabel->setScaledContents(true);
                cardLabel->setFixedSize(cardWidth, cardHeight);

                playerCards[handIdx].push_back(cardLabel);
                playerCardSkins[handIdx].push_back(currentSkin);
            }

            int skinToUse = playerCardSkins[handIdx][cardIdx];
            QString skinFile = (skinToUse == 1) ? ":/resource/Images/Skin1.png" : ":/resource/Images/Skin2.png";
            QPixmap cardPixmap(skinFile);

            Card& card = PlayerHands[handIdx][cardIdx];
            int posX = (card.value + 1) * 124;
            if (card.value == 12) posX = 0;
            int posY = card.suit * 173;

            QPixmap croppedCard = cardPixmap.copy(posX, posY, 124, 173);
            cardLabel->setPixmap(croppedCard);

            int endX = startX + cardIdx * 35;
            int endY = startY + (cardIdx * 5);

            if (isNewCard) {
                AnimateCardAppearance(cardLabel, -100, 300, endX, endY);
            }
            else {
                cardLabel->move(endX, endY);
            }

            cardLabel->show();
            cardLabel->raise();
        }

        prevPlayerCardCounts[handIdx] = PlayerHands[handIdx].size();
    }
}

void Window::EnableGameButtons(bool enabled)
{
    QMetaObject::invokeMethod(this, [this, enabled]() {
        hitButton->setEnabled(enabled);
        standButton->setEnabled(enabled);
        doubleDownButton->setEnabled(enabled);
        splitButton->setEnabled(enabled);

        QString opacity = enabled ? "1.0" : "0.5";
        }, Qt::QueuedConnection);
}

void Window::ShowGameOver(QString message)
{
    QMetaObject::invokeMethod(this, [this, message]() {
        betButton->setEnabled(true);
        betSpinBox->setEnabled(true);
        EnableGameButtons(false);
        dealerValueLabel->setText(message);
        dealerValueLabel->resize(200, 30);
        }, Qt::QueuedConnection);
}

void Window::ClearCards()
{
    for (auto label : dealerCards) {
        delete label;
    }
    dealerCards.clear();
    dealerCardSkins.clear();
    prevDealerCardCount = 0;

    for (auto& hand : playerCards) {
        for (auto label : hand) {
            delete label;
        }
        hand.clear();
    }
    playerCards.clear();
    playerCardSkins.clear();
    prevPlayerCardCounts.clear();

    dealerValueLabel->hide();
    for (auto l : handValueLabels) delete l;
    handValueLabels.clear();
}

void Window::AnimateCardAppearance(QLabel* cardLabel, int startX, int startY, int endX, int endY)
{
    if (cardSound) cardSound->play();

    QPropertyAnimation* posAnimation = new QPropertyAnimation(cardLabel, "pos");
    posAnimation->setDuration(500);
    posAnimation->setStartValue(QPoint(startX, startY));
    posAnimation->setEndValue(QPoint(endX, endY));
    posAnimation->setEasingCurve(QEasingCurve::OutBack);

    connect(posAnimation, &QPropertyAnimation::finished, posAnimation, &QObject::deleteLater);
    posAnimation->start();
}

void Window::on_BetButton_clicked()
{
    if (buttonSound) buttonSound->play();

    if (blackjack) {
        int bet = betSpinBox->value();
        blackjack->PlaceBet(bet);
        betLabel->setText(QString("Current Bet: $%1").arg(bet));
        betButton->setEnabled(false);
        betSpinBox->setEnabled(false);
        EnableGameButtons(true);
        dealerValueLabel->hide();
    }
}

void Window::on_HitButton_clicked()
{
    if (buttonSound) buttonSound->play();
    if (blackjack) blackjack->Hit();
}

void Window::on_StandButton_clicked()
{
    if (buttonSound) buttonSound->play();
    if (blackjack) blackjack->Stand();
}

void Window::on_DoubleDownButton_clicked()
{
    if (buttonSound) buttonSound->play();
    if (blackjack) blackjack->DoubleDown();
}

void Window::on_SplitButton_clicked()
{
    if (buttonSound) buttonSound->play();
    if (blackjack) blackjack->Split();
}

void Window::on_DoubleDownCheckBox_changed(Qt::CheckState state)
{
    if (blackjack) blackjack->IsDoubleDownAllowed = (state == Qt::Checked);
}

void Window::on_BlackjackPayoutCheckBox_changed(Qt::CheckState state)
{
    if (blackjack) blackjack->NaturalBlackjackpayout32 = (state == Qt::Checked);
}

void Window::on_PerfectPairsCheckBox_changed(Qt::CheckState state)
{
    if (blackjack) blackjack->PerfectPairsSideBet = (state == Qt::Checked);
}

void Window::on_SkinSlider_changed(int value)
{
    currentSkin = value;
}

void Window::on_SetSeedButton_clicked()
{
    if (blackjack) {
        int seed = seedSpinBox->value();
        blackjack->SetCustomSeed(seed);
    }
}