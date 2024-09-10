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

    // 创建 QDateTimeEdit 控件
    dateTimeEdit = ui->dateTimeEdit; // 确保在 UI 中有这个控件
    dateTimeEdit->setDisplayFormat("yyyy/MM/dd HH:mm:ss"); // 设置显示格式

    // 创建并启动定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::updateTime);
    timer->start(1000); // 每秒更新一次时间

    // 初始化时间
    updateTime();

    // 创建 QMediaPlayer 实例
    player = new QMediaPlayer(this); // 使用 this 作为父对象
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &Widget::changeVolume);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap(":/1.bmp")); // 绘制背景图像
}

void Widget::updateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    // 调整为 UTC+8（中国标准时间）
    currentTime = currentTime.addSecs(0 * 3600); // 转换为 UTC+8
    dateTimeEdit->setDateTime(currentTime); // 更新 QDateTimeEdit 的显示值
}

void Widget::changeVolume(int value)
{
    // 将滑块值 (0-100) 转换为音量 (0.0-1.0)
    float volume = value / 100.0f;

    // 初始化 COM 库
    CoInitialize(nullptr);

    // 获取默认音频设备
    IMMDeviceEnumerator* deviceEnumerator = nullptr;
    IMMDevice* device = nullptr;
    IAudioEndpointVolume* endpointVolume = nullptr;

    // 创建设备枚举器
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (void**)&deviceEnumerator);

    if (FAILED(hr)) {
        qDebug() << "Failed to create device enumerator.";
        return;
    }

    // 获取默认音频终端
    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
    if (FAILED(hr)) {
        qDebug() << "Failed to get default audio endpoint.";
        deviceEnumerator->Release();
        return;
    }

    // 获取音量控制接口
    hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&endpointVolume);
    if (FAILED(hr)) {
        qDebug() << "Failed to activate audio endpoint volume.";
        device->Release();
        deviceEnumerator->Release();
        return;
    }

    // 设置音量
    hr = endpointVolume->SetMasterVolumeLevelScalar(volume, nullptr);
    if (FAILED(hr)) {
        qDebug() << "Failed to set master volume level.";
    }

    // 释放资源
    endpointVolume->Release();
    device->Release();
    deviceEnumerator->Release();
    CoUninitialize();
}
