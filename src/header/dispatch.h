#ifndef _DISPATCH_H
#define _DISPATCH_H


struct Vehicle_t {
    char Vehicle_ID[10]; 
    int  VehicleType; 
    int  CapacityWeight;
    int  CapacityVolume;
    int  VehicleStatus; //0->空闲 1->运输中 2->修理中 
};


struct Dispatch_t {
    char Dispatch_ID[20]; //DD+时间+06序号 
    
    char Order_ID[20];      // 订单号
    char Driver_ID[10];     // 司机ID
    char Vehicle_ID[10];    // 车牌号
    
    char RouteInfo[100];    // 线路信息
    char Est_Departure[20]; // 预计出发时间
    char Est_Arrival[20];   // 预计到达时间
    
    int  DispatchStatus;    // 0->已派车(待发), 1->已出发, 2->中转站, 3->已送达, 4->异常                  
    char ExceptionMsg[100]; // 异常原因
};



#endif
