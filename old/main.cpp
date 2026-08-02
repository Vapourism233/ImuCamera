//REQUIRED OPENCV 4.5.0

#include   <stdio.h>
#include   <stdlib.h>
#include   <stdint.h>
#include   <iostream>
#include   <unistd.h>
#include   <sys/types.h>
#include   <string.h>
#include   <unistd.h>
#include   <fcntl.h>
#include   <termios.h>
#include   <string.h>
#include   "HCNetSDK.h"
#include   <pthread.h>
#include   "EasyProfile/EasyObjectDictionary.h"
#include   "EasyProfile/EasyProfile.h"
#include   <vector>
#include   "opencv.hpp"
#include   "DetectRegions.h"
#include   "Plate.h"
#include   "OCR.h"
#include   "imgproc/types_c.h"

using namespace std;
using namespace cv;

#define BUFSIZE 512
#define TRUE 1
#define FALSE 0
#define LEFT PAN_LEFT
#define RIGHT PAN_RIGHT
#define UP TILT_UP
#define DOWN TILT_DOWN
EasyObjectDictionary eOD;
EasyProfile ep(&eOD);
float pit, yaw;

void setTermios(struct termios * pNewtio, int uBaudRate)
{
    bzero(pNewtio, sizeof(struct termios)); /* clear struct for new port settings */
//8N1
    pNewtio->c_cflag = uBaudRate | CS8 | CREAD | CLOCAL;
    pNewtio->c_iflag = IGNPAR;
    pNewtio->c_oflag = 0;
    pNewtio->c_lflag = 0; //non ICANON
    pNewtio->c_cc[VINTR] = 0; /* Ctrl-c */
    pNewtio->c_cc[VQUIT] = 0; /* Ctrl-\ */
    pNewtio->c_cc[VERASE] = 0; /* del */
    pNewtio->c_cc[VKILL] = 0; /* @ */
    pNewtio->c_cc[VEOF] = 4; /* Ctrl-d */
    pNewtio->c_cc[VTIME] = 5; /* inter-character timer, timeout VTIME*0.1 */
    pNewtio->c_cc[VMIN] = 0; /* blocking read until VMIN character arrives */
    pNewtio->c_cc[VSWTC] = 0; /* '\0' */
    pNewtio->c_cc[VSTART] = 0; /* Ctrl-q */
    pNewtio->c_cc[VSTOP] = 0; /* Ctrl-s */
    pNewtio->c_cc[VSUSP] = 0; /* Ctrl-z */
    pNewtio->c_cc[VEOL] = 0; /* '\0' */
    pNewtio->c_cc[VREPRINT] = 0; /* Ctrl-r */
    pNewtio->c_cc[VDISCARD] = 0; /* Ctrl-u */
    pNewtio->c_cc[VWERASE] = 0; /* Ctrl-w */
    pNewtio->c_cc[VLNEXT] = 0; /* Ctrl-v */
    pNewtio->c_cc[VEOL2] = 0; /* '\0' */
}

int set_Parity(int fd, int databits, int stopbits, int parity)

{
    struct termios options;
    if (tcgetattr(fd, &options) != 0)
    {
        perror("SetupSerial 1");
        return (FALSE);
    }
    options.c_cflag &= ~CSIZE;
    switch (databits)
    {
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            fprintf(stderr,"Unsupported data size\n");
            return (FALSE);
    }
    switch (parity)
    {
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB; /* Clear parity enable */
            options.c_iflag &= ~INPCK; /* Enable parity checking */
            break;
        case 'o':
        case 'O':
            options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
            options.c_iflag |= INPCK; /* Disnable parity checking */
            break;
        case 'e':
        case 'E':
            options.c_cflag |= PARENB; /* Enable parity */
            options.c_cflag &= ~PARODD; /* 转换为偶效验*/
            options.c_iflag |= INPCK; /* Disnable parity checking */
            break;
        case 'S':
        case 's': /*as no parity*/
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
        default:
            fprintf(stderr,"Unsupported parity\n");
            return (FALSE);
    }
    /* 设置停止位*/
    switch (stopbits)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
        default:
            fprintf(stderr,"Unsupported stop bits\n");
            return (FALSE);
    }
    /* Set input parity option */
    if (parity != 'n')
        options.c_iflag |= INPCK;
    tcflush(fd, TCIFLUSH);
    options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/
    options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
    if (tcsetattr(fd, TCSANOW, &options) != 0)
    {
        perror("SetupSerial 3");
        return (FALSE);
    }
    return (TRUE);
}

int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300,
                    B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {38400, 19200,  9600,  4800,  2400,  1200,  300, 38400,
                  19200,  9600, 4800, 2400, 1200,  300, };

void set_speed(int fd, int speed){
    int   i;
    int   status;
    struct termios   Opt;
    tcgetattr(fd, &Opt);
    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) {
        if  (speed == name_arr[i]) {
            tcflush(fd, TCIOFLUSH);//设置前flush
            cfsetispeed(&Opt, speed_arr[i]);
            cfsetospeed(&Opt, speed_arr[i]);
            //通过tcsetattr函数把新的属性设置到串口上。
            //tcsetattr(串口描述符，立即使用或者其他标示，指向termios的指针)
            status = tcsetattr(fd, TCSANOW, &Opt);
            if (status != 0) {
                perror("tcsetattr fd1");
                return;
            }
            tcflush(fd, TCIOFLUSH);  //设置后flush
        }
    }
}

//读取IMU线程
void* thread_imu(void* arg)
{
    int fd;
    int n;
    char buff[BUFSIZE];
    Ep_Header header;
    char* dev ="/dev/ttyUSB0";
    char* buf=buff;
    Ep_RPY ep_RPY;
    struct termios oldtio, newtio;
    struct timeval tv;
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    while(TRUE)
    {
        fd = open(dev, O_RDWR | O_NOCTTY);
        n=read(fd, buf, BUFSIZE);
        tcgetattr(fd, &oldtio); /* save current serial port settings */
        setTermios(&newtio, B115200);
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd, TCSANOW, &newtio);
        set_speed(fd,115200);
        set_Parity(fd, 8, 1, 'N');
        ep.On_RecvPkg(buf,n,&header);
        eOD.Read_Ep_RPY(&ep_RPY);
        pit = ep_RPY.pitch;
        yaw = ep_RPY.yaw;
//        printf("pitch=%.1f ",pit);
//        printf("yaw=%.1f\n",yaw);
//        printf("Now:%f\n",pit);
    }
//    char* msg = "thread_imu";
//    return msg;
}

void* thread_recognition(void* arg){
    Mat input_image;
    IplImage* frame;
    LPNET_DVR_JPEGPARA JpegPara = new NET_DVR_JPEGPARA;
    JpegPara->wPicQuality = 0;
    JpegPara->wPicSize = 9;
    char * Jpeg = new char[1920*1024];
    DWORD len = 1920*1024;
    LPDWORD Ret = 0;
    long lUserID = *((long *)arg);
    //login注册相机64
//    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
//    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
//    struLoginInfo.bUseAsynLogin = false;
//
//    struLoginInfo.wPort = 8000;
//    memcpy(struLoginInfo.sDeviceAddress, "192.168.1.8", NET_DVR_DEV_ADDRESS_MAX_LEN);
//    memcpy(struLoginInfo.sUserName, "admin", NAME_LEN);
//    memcpy(struLoginInfo.sPassword, "a1234567", NAME_LEN);
//    //login
//    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    if(!NET_DVR_SetCapturePictureMode(BMP_MODE))
    {
        cout<<"Set Capture Picture Mode error!"<<endl;
        cout<<"The error code is "<<NET_DVR_GetLastError()<<endl;
    }
    vector<char>data(1920*1024);
    FileStorage fs;
    fs.open("/home/jiang/CLionProjects/test1/SVM.xml", FileStorage::READ);
    Mat SVM_TrainingData;  //trainning data
    Mat SVM_Classes;
    fs["TrainingData"] >> SVM_TrainingData;
    fs["classes"] >> SVM_Classes;
    //Set SVM params
    cv::Ptr<cv::ml::SVM> svm;
    svm = ml::SVM::create();
    svm->setType(ml::SVM::C_SVC);
    svm->setKernel(ml::SVM::LINEAR);
    svm->setGamma(0.01);
    svm->setC(10.0);
    svm->setDegree(0);
    svm->setGamma(1);
    svm->setCoef0(0);
    svm->setC(1);
    svm->setNu(0);
    svm->setP(0);
    svm->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01));
    Ptr<ml::TrainData> trainData = ml::TrainData::create(SVM_TrainingData, ml::SampleTypes::ROW_SAMPLE, SVM_Classes);
    //Train SVM
    svm->trainAuto(trainData, 10,
                   ml::SVM::getDefaultGrid(ml::SVM::C), ml::SVM::getDefaultGrid(ml::SVM::GAMMA),
                   ml::SVM::getDefaultGrid(ml::SVM::P), ml::SVM::getDefaultGrid(ml::SVM::NU),
                   ml::SVM::getDefaultGrid(ml::SVM::COEF), ml::SVM::getDefaultGrid(ml::SVM::DEGREE), true);

    while(TRUE) {
        bool capture = NET_DVR_CaptureJPEGPicture_NEW(lUserID, 1, JpegPara, Jpeg, len, Ret);
        if (!capture) {
            printf("Error: NET_DVR_CaptureJPEGPicture_NEW = %d\n", NET_DVR_GetLastError());
        }
        for (int i = 0; i < 1920 * 1024; i++) {
            data[i] = Jpeg[i];
        }
        input_image = imdecode(Mat(data), 1);
        if(!input_image.empty()) {
//            imshow("camera", input_image);
//            waitKey(1);
            DetectRegions detectRegions;
            detectRegions.setFilename("111");
            detectRegions.saveRegions = false;
            detectRegions.showSteps = false;
            vector<Plate> posible_regions = detectRegions.run(input_image);
//
            //SVM for each plate region to get valid car plates
            //Read file storage.

            //For each possible plate, classify with svm if it's a plate or not
            vector<Plate> plates;
            for (int i = 0; i < posible_regions.size(); i++) {
                Mat img = posible_regions[i].plateImg;
                Mat p = img.reshape(1, 1);
                p.convertTo(p, CV_32FC1);
                int response = (int) (svm->predict(p));
                if (response == 1)
                    plates.push_back(posible_regions[i]);
            }
            cout << "Num plates detected: " << plates.size() << "\n";
            //For each plate detected, recognize it with OCR
            OCR ocr("OCR.xml");
            ocr.saveSegments = false;
            ocr.DEBUG = false;
            ocr.filename = "111";
            for (int i = 0; i < plates.size(); i++) {
                Plate plate = plates[i];
                string plateNumber = ocr.run(&plate);
                string licensePlate = plate.str();
//                cout << "================================================\n";
//                cout << "License plate number: " << licensePlate << "\n";
//                cout << "================================================\n";
                rectangle(input_image, plate.position, Scalar(0, 0, 200));
                putText(input_image, licensePlate, Point(plate.position.x, plate.position.y), FONT_HERSHEY_COMPLEX, 1,
                        Scalar(0, 0, 200), 2);
            }
            imshow("Plate Detected", input_image);
            waitKey(1);
        }
    }
}

int main()
{
    pthread_t tid1, tid2;
    int flag = 0;
    long lUserID;
    NET_DVR_Init();
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
//
    //login注册相机64
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
    struLoginInfo.bUseAsynLogin = false;

    struLoginInfo.wPort = 8000;
    memcpy(struLoginInfo.sDeviceAddress, "192.168.1.8", NET_DVR_DEV_ADDRESS_MAX_LEN);
    memcpy(struLoginInfo.sUserName, "admin", NAME_LEN);
    memcpy(struLoginInfo.sPassword, "a1234567", NAME_LEN);
    //login
    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    //读取相机信息
    NET_DVR_PTZPOS m_ptzPosCurrent;
    DWORD dwPTZCommand;
    DWORD dwStop = 0;
    float p2, p;
    float y2, y;
    float t;
    void* a = &lUserID;
//    if(pthread_create(&tid2, NULL, thread_recognition, a) != 0)
//    {
//        printf("pthread_imu error");
//        exit(EXIT_FAILURE);
//    }
//    pthread_detach(tid2);
    while (TRUE)
    {
        DWORD dwSpeed;
        flag++;
        if(pthread_create(&tid1, NULL, thread_imu, (void *) "new thread:") != 0)
        {
            printf("pthread_imu error");
            exit(EXIT_FAILURE);
        }
        pthread_detach(tid1);
        if(flag % 100 == 0)
        {
            printf("Input the terminal:");
            scanf("%f,%f",&p2,&y2);
            if(p2)
            {
                while(TRUE)
                {
                    p = pit - p2;
                    y = yaw - y2;
                    if(abs(yaw - y2) > 2)
                    {
                        dwSpeed = 1;
                        dwStop = 0;
                        if(y < 0){dwPTZCommand = LEFT;}
                        else{dwPTZCommand = RIGHT;}
                    }
                    if(abs(yaw - y2) < 2){dwPTZCommand = 0;}
                    printf("pitch:%f\n",pit);
                    printf("yaw:%f\n",yaw);
                    if(abs(pit - p2) > 2)
                    {
                        if(dwPTZCommand == LEFT){dwPTZCommand = UP_LEFT;}
                        if(dwPTZCommand == RIGHT){dwPTZCommand = UP_RIGHT;}
                        else{dwPTZCommand = UP;}
                        dwStop = 0;
                        dwSpeed = 2;
                        NET_DVR_PTZControlWithSpeed_Other(lUserID, 1, dwPTZCommand, dwStop,dwSpeed);
                        usleep(100000);
                    }
                    if(abs(pit - p2) < 5 && abs(yaw - y2) < 5)
                    {
                        dwStop = 1;
                        NET_DVR_PTZControlWithSpeed_Other(lUserID, 1, dwPTZCommand, dwStop,dwSpeed);
                        break;
                    }
                }
            }
        }
    }
}