#ifndef _ORDER_H
#define _ORDER_H


// 订单-货物关联结构体
struct OrderGoods_t {
    char ID[20];
    char Order_ID[20];
    char Goods_ID[20];
    int GoodsAmount; 
};








#endif
