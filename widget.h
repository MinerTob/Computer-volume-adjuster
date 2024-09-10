#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSlider> // 包含滑块
#include <QDateTimeEdit> // 引入 QDateTimeEdit
#include <QTimer> // 引入 QTimer
#include <QMediaPlayer> // 我们也保留音频播放器的声明
#include <Windows.h> // 用于音量控制
#include <audioclient.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void changeVolume(int value); // 声明槽函数，用于改变音量
    void updateTime(); // 实时更新时间

private:
    Ui::Widget* ui;
    QDateTimeEdit* dateTimeEdit; // 用于显示时间的 QDateTimeEdit
    QTimer* timer;     // 定时器以实时更新时间
    QMediaPlayer* player; // 对象用于音频播放器
};

#endif // WIDGET_H
