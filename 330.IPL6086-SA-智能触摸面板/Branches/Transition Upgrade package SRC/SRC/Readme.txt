********************************************************************************
							6按键方形面板版本说明
							 IPL6086-SA  源程序
                    A地址  0x8000800
                    B地址  0x800C400
********************************************************************************

================================================================================
V2.10.08	时间：2015.09.16	硬件：IPL6086 V1.0.1		修改：周雁

烧录文件：IPL6086-SA_V2.10.08_burn.hex
升级文件：IPL6086-SA_V2.10.08_update.dat
说    明：
库版本  : V1.01.16
修改内容：
		1. 修复在单键配置多条联动消息时后面消息被覆盖问题；；
================================================================================

================================================================================
V2.10.07	时间：2015.08.27	硬件：IPL6086 V1.0.1		修改：周雁

烧录文件：IPL6086-SA_V2.10.07_burn.hex
升级文件：IPL6086-SA_V2.10.07_update.dat
说    明：
库版本  : V1.01.16
修改内容：
		1、发送增加优先级操作；
================================================================================

================================================================================
V2.10.06	时间：2015.08.19	硬件：IPL6086 V1.0.1		修改：周雁

烧录文件：IPL6086-SA_V2.10.06_burn.hex
升级文件：IPL6086-SA_V2.10.06_update.dat
说    明：
库版本  : V1.01.16
修改内容：
		修改用户体验：
		1. 第一次触摸默认点亮6个背景灯，不往外发送联动消息，屏幕显示上一次休眠前空调状态。
		2. 按键休眠时间由10秒延长20秒，时间到后休眠。
		3. 开机时在关机状态，按其他按键也会开空调;
		4. 修正一些逻辑错误；
		5. 修改首次唤醒时，按键检测方式（手不拿开不执行的方式）；
		6. 修改密码界面时密码输入正确后直接进入上次状态，而不显示“欢迎使用”提示；
		7. 首次上电后直接显示上次状态；
		8. 增加电量低于8%的电量时，提示用户充电，同时3s后休眠，而不执行其他操作；
================================================================================

================================================================================
V2.10.05	时间：2015.08.11	硬件：IPL6086 V1.0.1		修改：周雁

烧录文件：IPL6086-SA_V2.10.05_burn.hex
升级文件：IPL6086-SA_V2.10.05_update.dat
说    明：
库版本  : V1.01.16
修改内容：
	1、修改代码逻辑与IPL6086-SC相同；
	2、库版本升级至1.01.16；

================================================================================
================================================================================
V2.10.01	时间：2015.01.20	硬件：IPL6086 V1.0.1		修改：李偌淮

烧录文件：IPL6086-SA_V2.10.01_burn.hex
升级文件：IPL6086-SA_V2.10.01_update.dat
说    明：
    1.修改因单元属性排列问题导致的属性不能操作BUG
    2.更新库 V1.01.03
================================================================================
================================================================================
V2.10.00	时间：2015.01.06	硬件：FC-ME1-V1 V1.0.2		修改：李偌淮

烧录文件：IPL6086-SA_V2.10.00_burn.hex
升级文件：IPL6086-SA_V2.10.00_update.dat
说    明：
    1.添加电池图标/充电图标功能
    2.优化密码初始化，恢复默认设置时，清除用户密码
    3.更新库 V1.01.02，存储地址空间扩大至47K
    
    4.优化空调初始温度值不在设定范围内
    5.优化上电时 背光灯不同步
    6.长按键时间改短（参数调整为100）
    
    7.添加扫风功能及图标
================================================================================

================================================================================
V2.00.00
	功能：
	1.V2.00.00发布
	2.图片Flash下载地址（下载地址0x8019000）（134320128）
================================================================================