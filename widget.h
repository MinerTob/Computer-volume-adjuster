#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSlider> // ��������
#include <QDateTimeEdit> // ���� QDateTimeEdit
#include <QTimer> // ���� QTimer
#include <QMediaPlayer> // ����Ҳ������Ƶ������������
#include <Windows.h> // ������������
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
    void changeVolume(int value); // �����ۺ��������ڸı�����
    void updateTime(); // ʵʱ����ʱ��

private:
    Ui::Widget* ui;
    QDateTimeEdit* dateTimeEdit; // ������ʾʱ��� QDateTimeEdit
    QTimer* timer;     // ��ʱ����ʵʱ����ʱ��
    QMediaPlayer* player; // ����������Ƶ������
};

#endif // WIDGET_H
