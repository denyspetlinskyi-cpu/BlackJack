#pragma once

#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSlider>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "BlackJack.h"
#include <vector>

class Window : public QMainWindow
{
    Q_OBJECT

public:
    BlackJack* blackjack = nullptr;
    Window(QWidget *parent = nullptr);
    ~Window();
    void SetBalance(int balance);
    void SetDealerHand(std::vector<Card> DealerHand, bool hideSecond);
    void SetPlayerHands(std::vector<std::vector<Card>> PlayerHands);
    void ShowHandValues(std::vector<std::vector<int>> handValues);
    void ShowDealerValue(int value);
    void EnableGameButtons(bool enabled);
    void ShowGameOver(QString message);
    void ClearCards();

private slots:
    void on_BetButton_clicked();
    void on_HitButton_clicked();
    void on_StandButton_clicked();
    void on_DoubleDownButton_clicked();
    void on_SplitButton_clicked();
    void UpdateDealerCards(std::vector<Card> DealerHand, bool hideSecond);
    void UpdatePlayerCards(std::vector<std::vector<Card>> PlayerHands);
    void DisplayHandValues(std::vector<std::vector<int>> handValues);
    void DisplayDealerValue(int value);
    void on_DoubleDownCheckBox_changed(Qt::CheckState state);
    void on_BlackjackPayoutCheckBox_changed(Qt::CheckState state);
    void on_PerfectPairsCheckBox_changed(Qt::CheckState state);
    void on_SkinSlider_changed(int value);
    void on_SetSeedButton_clicked();

private:
    void CreateUI();
    void DrawCard(int x, int y, Card card, QWidget* parent);
    void AnimateCardAppearance(QLabel* cardLabel, int startX, int startY, int endX, int endY);

    QLabel* balanceLabel;
    QLabel* betLabel;
    QSpinBox* betSpinBox;
    QPushButton* betButton;
    QPushButton* hitButton;
    QPushButton* standButton;
    QPushButton* doubleDownButton;
    QPushButton* splitButton;

    QCheckBox* doubleDownCheckBox;
    QCheckBox* blackjackPayoutCheckBox;
    QCheckBox* perfectPairsCheckBox;
    QSlider* skinSlider;
    QSpinBox* seedSpinBox;
    QPushButton* setSeedButton;

    int currentSkin = 2;

    QWidget* dealerArea;
    QWidget* playerArea;

    std::vector<QLabel*> dealerCards;
    std::vector<std::vector<QLabel*>> playerCards;
    std::vector<QLabel*> handValueLabels;
    QLabel* dealerValueLabel;

    size_t prevDealerCardCount = 0;
    std::vector<size_t> prevPlayerCardCounts;

    std::vector<int> dealerCardSkins;
    std::vector<std::vector<int>> playerCardSkins;

    QMediaPlayer* bgMusic;
    QAudioOutput* bgAudio;

    QSoundEffect* buttonSound;
    QSoundEffect* cardSound;
};

