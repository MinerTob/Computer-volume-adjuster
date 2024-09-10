#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QDateTime>
#include <QDebug>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // ���� QDateTimeEdit �ؼ�
    dateTimeEdit = ui->dateTimeEdit; // ȷ���� UI ��������ؼ�
    dateTimeEdit->setDisplayFormat("yyyy/MM/dd HH:mm:ss"); // ������ʾ��ʽ

    // ������������ʱ��
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::updateTime);
    timer->start(1000); // ÿ�����һ��ʱ��

    // ��ʼ��ʱ��
    updateTime();

    // ���� QMediaPlayer ʵ��
    player = new QMediaPlayer(this); // ʹ�� this ��Ϊ������
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &Widget::changeVolume);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap(":/1.bmp")); // ���Ʊ���ͼ��
}

void Widget::updateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    // ����Ϊ UTC+8���й���׼ʱ�䣩
    currentTime = currentTime.addSecs(0 * 3600); // ת��Ϊ UTC+8
    dateTimeEdit->setDateTime(currentTime); // ���� QDateTimeEdit ����ʾֵ
}

void Widget::changeVolume(int value)
{
    // ������ֵ (0-100) ת��Ϊ���� (0.0-1.0)
    float volume = value / 100.0f;

    // ��ʼ�� COM ��
    CoInitialize(nullptr);

    // ��ȡĬ����Ƶ�豸
    IMMDeviceEnumerator* deviceEnumerator = nullptr;
    IMMDevice* device = nullptr;
    IAudioEndpointVolume* endpointVolume = nullptr;

    // �����豸ö����
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (void**)&deviceEnumerator);

    if (FAILED(hr)) {
        qDebug() << "Failed to create device enumerator.";
        return;
    }

    // ��ȡĬ����Ƶ�ն�
    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
    if (FAILED(hr)) {
        qDebug() << "Failed to get default audio endpoint.";
        deviceEnumerator->Release();
        return;
    }

    // ��ȡ�������ƽӿ�
    hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&endpointVolume);
    if (FAILED(hr)) {
        qDebug() << "Failed to activate audio endpoint volume.";
        device->Release();
        deviceEnumerator->Release();
        return;
    }

    // ��������
    hr = endpointVolume->SetMasterVolumeLevelScalar(volume, nullptr);
    if (FAILED(hr)) {
        qDebug() << "Failed to set master volume level.";
    }

    // �ͷ���Դ
    endpointVolume->Release();
    device->Release();
    deviceEnumerator->Release();
    CoUninitialize();
}
