i2c设备树和驱动的使用方法

在IMX6的dtsi文件里面定义I2C label
&i2c3 {
   adxl345@53{                          //主要添加compatible和reg
        compatible = "adxl345";        //driver程序里面的名称要和这个匹配
        reg = <0x53>;                  //reg是I2C从器件的地址,比如ADXL345器件地址是0x53
	以上两项加完了就可以在驱动里面操作I2C设备了
        下面几项可以不加，除非要用另外几个GPIO引脚去控制器件的其他部分，或者需要使用中断，根据使用的器件规格来确定
	interrupts = <9 2>;           //这几项要在驱动里面用of函数来获取，很装逼的
        wakeup-gpios = <&gpio6 7 0>;  这几项要在驱动里面用of函数来获取，很装逼的
         };

};

以上就是设备树定义的过程



驱动的注册方法和标准linux I2C控制器差不多


static const struct i2c_device_id adxl345_id[] = {
         { "adxl345", 0   },        //这个是老版本的设备芯片地址的注册方法，因为设备树已经注册了，所以不需要在这里注册填写0即可，但是前面的名字要和设备树一致
         {   }
 
 
};
 
static struct of_device_id adxl345dt[]={
         { .compatible = "adxl345", },//驱动要靠这个结构来和设备树匹配，这个才是主要的
         {},
 
         
};

MODULE_DEVICE_TABLE(of, adxl345dt); //然后宏定义of表不可少



static struct i2c_driver adxl345_driver = {
         .driver = {
         .name= "adxl345",
         .owner= THIS_MODULE,
         .of_match_table= adxl345dt,
 
         
	},
         .id_table= adxl345_id,
         .probe= adxl345_probe,
         .remove = adxl345_remove,
 
};
先定义了I2C要驱动的芯片设备的结构体

以上步骤完成之后就可以进入probe函数了，在驱动的init和exit入口函数要加上i2c_add_driver(&adxl345_driver);
在驱动exit出口函数要加上i2c_del_driver(&adxl345_driver);


必须要定义struct i2c_client *adxl345_client;才能知道你使用的是哪一个I2C，我们设备树是在I2C2里面添加的设备节点，所以i2c_client自动添加i2c2这条总线，硬件上面芯片引脚也要接在主芯片的i2c2引脚

struct i2c_client *adxl345_client;


static int adxl345_probe(struct i2c_client *client,const struct i2c_device_id *id){
 
   adxl345_client = client;

}

在probe函数里面使用获取i2c_client

i2c_master_send(adxl345_client, *reg_addr, 1);//发送数据给寄存器
i2c_master_recv(adxl345_client, &ID, 1);  // 接收寄存器的值


然后就可以用i2c_master_send和i2c_master_recv来读写芯片寄存器的，这个函数网上很多随便查找。这个比i2c_smbus读写快多了 建议不要用i2c smbus，


这里有一点要注意，在设备模拟I2C的时候地址可能是器件地址，但是在用linux的I2C控制器的时候就可能出现和器件地址不一样的值，所以当用控制器去挂接i2c设备的时候，先在终端执行i2cdecet -y 2 命令来确定i2c器件在linux映射下的地址





