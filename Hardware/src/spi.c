#include "spi.h"

typedef struct{
	short Accel[3];//Accel X,Y,Z
	short Gyro[3];//Gyro X,Y,Z
	short Mag[3];	//Mag X,Y,Z
}MPU_source_value;

MPU_source_value mpu_source_value;          //9������
MPU_value mpu_value;
unsigned char BUF[6];       //�������ݻ�����
/***************************************************************/
void spi_Init()
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1|RCC_APB2Periph_GPIOA, ENABLE);
	//GPIO����������//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
    /*Configure PA.4(NSS)--------------------------------------------*/
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	 /* SPI1 configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λ����
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//SPI_CPOL_High=ģʽ3��ʱ�ӿ���Ϊ�� //SPI_CPOL_Low=ģʽ0��ʱ�ӿ���Ϊ��
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//SPI_CPHA_2Edge;//SPI_CPHA_1Edge, SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//SPI_NSS_Soft;//SPI_NSS_Hard
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//SPI_BaudRatePrescaler_2=32M;//SPI_BaudRatePrescaler_4=18MHz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ݴӸ�λ��ʼ����
  SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
}
/***************************************************************/
//SPIx
//TxData:����һ���ֽ�
//����ֵ:data
/***************************************************************/
static u8 SPI1_ReadWriteByte(u8 TxData)
{
	u8 retry=0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //�ȴ�SPI���ͱ�־λ��
		{
		retry++;
		if(retry>200)return 0;
		}
	SPI_I2S_SendData(SPI1, TxData); //��������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //�ȴ�SPI���ձ�־λ��
		{
		retry++;
		if(retry>200)return 0;
		}
	return SPI_I2S_ReceiveData(SPI1); //��������
}
/***************************************************************/
//SPI����
//reg: addr
//value:����
/***************************************************************/
u8 MPU9250_Write_Reg(u8 reg,u8 value)
{
	u8 status;
		MPU_9250_ENABLE;   //	MPU9250_CS=0;  //ƬѡMPU9250
	status=SPI1_ReadWriteByte(reg); //����reg��ַ
	SPI1_ReadWriteByte(value);//��������
	MPU_9250_DISENABLE;//	MPU9250_CS=1;  //ʧ��MPU9250
	return(status);//
}
//---------------------------------------------------------------//
//SPI��ȡ
//reg: addr
u8 MPU9250_Read_Reg(u8 reg)
{
	  u8 reg_val;
		MPU_9250_ENABLE;//	MPU9250_CS=0;  //ƬѡMPU9250
	  SPI1_ReadWriteByte(reg|0x80); //reg��ַ+������
	  reg_val=SPI1_ReadWriteByte(0xff);//��������
		MPU_9250_DISENABLE;//	MPU9250_CS=1;  //ʧ��MPU9250
	return(reg_val);
}
/***************************************************************/
// MPU�ڲ�i2c д��
//I2C_SLVx_ADDR:  MPU9250_AK8963_ADDR
//I2C_SLVx_REG:   reg
//I2C_SLVx_Data out:  value
/***************************************************************/
static void i2c_Mag_write(u8 reg,u8 value)
{
	u16 j=500;
	MPU9250_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR);//���ô����Ƶ�ַ,mode: write
	MPU9250_Write_Reg(I2C_SLV0_REG ,reg);//set reg addr
	MPU9250_Write_Reg(I2C_SLV0_DO ,value);//send value
	while(j--);//�˴���ΪMPU�ڲ�I2C��ȡ�ٶȽ�������ʱ�ȴ��ڲ�д����
}
/***************************************************************/
// MPU�ڲ�i2c ��ȡ
//I2C_SLVx_ADDR:  MPU9250_AK8963_ADDR
//I2C_SLVx_REG:   reg
//return value:   EXT_SENS_DATA_00 register value
/***************************************************************/
static u8 i2c_Mag_read(u8 reg)
{
	u16 j=5000;
	MPU9250_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR|0x80); //���ô����Ƶ�ַ��mode��read
	MPU9250_Write_Reg(I2C_SLV0_REG ,reg);// set reg addr
	MPU9250_Write_Reg(I2C_SLV0_DO ,0xff);//read
	while(j--);//�˴���ΪMPU�ڲ�I2C��ȡ�ٶȽ�����������ʱ�ȴ��ڲ���ȡ����
	return MPU9250_Read_Reg(EXT_SENS_DATA_00);
}

//****************��ʼ��MPU9250��������Ҫ���ο�pdf�����޸�************************
void Init_MPU9250(void)
{
	MPU9250_Write_Reg(PWR_MGMT_1, 0x00);	//��������״̬
	MPU9250_Write_Reg(CONFIG, 0x07);      //��ͨ�˲�Ƶ�ʣ�����ֵ��0x07(3600Hz)�˼Ĵ����ھ���Internal_Sample_Rate==8K

/**********************Init SLV0 i2c**********************************/
//Use SPI-bus read slave0
	MPU9250_Write_Reg(INT_PIN_CFG ,0x30);// INT Pin / Bypass Enable Configuration
	MPU9250_Write_Reg(I2C_MST_CTRL,0x4d);//I2C MAster mode and Speed 400 kHz
	MPU9250_Write_Reg(USER_CTRL ,0x20); // I2C_MST _EN
	MPU9250_Write_Reg(I2C_MST_DELAY_CTRL ,0x01);//��ʱʹ��I2C_SLV0 _DLY_ enable
	MPU9250_Write_Reg(I2C_SLV0_CTRL ,0x81); //enable IIC	and EXT_SENS_DATA==1 Byte

/*******************Init GYRO and ACCEL******************************/
	MPU9250_Write_Reg(SMPLRT_DIV, 0x07);  //�����ǲ����ʣ�����ֵ��0x07(1kHz) (SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV) )
	MPU9250_Write_Reg(GYRO_CONFIG, 0x18); //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	MPU9250_Write_Reg(ACCEL_CONFIG, 0x18);//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x18(���Լ죬16G)
	MPU9250_Write_Reg(ACCEL_CONFIG_2, 0x08);//���ټƸ�ͨ�˲�Ƶ�� ����ֵ ��0x08  ��1.13kHz��

/**********************Init MAG **********************************/
	i2c_Mag_write(AK8963_CNTL2_REG,AK8963_CNTL2_SRST); // Reset AK8963
	i2c_Mag_write(AK8963_CNTL1_REG,0x12); // use i2c to set AK8963 working on Continuous measurement mode1 & 16-bit output
}

//************************���ٶȶ�ȡ**************************/
void READ_MPU9250_ACCEL(void)//
{

   BUF[0]=MPU9250_Read_Reg(ACCEL_XOUT_L);
   BUF[1]=MPU9250_Read_Reg(ACCEL_XOUT_H);
   mpu_source_value.Accel[0]=	(BUF[1]<<8)|BUF[0];
   mpu_value.Accel[0] = mpu_source_value.Accel[0] / 164.0; 						   //��ȡ����X������
   BUF[2]=MPU9250_Read_Reg(ACCEL_YOUT_L);
   BUF[3]=MPU9250_Read_Reg(ACCEL_YOUT_H);
   mpu_source_value.Accel[1]=	(BUF[3]<<8)|BUF[2];
   mpu_value.Accel[1] = mpu_source_value.Accel[1] / 164.0; 						   //��ȡ����Y������
   BUF[4]=MPU9250_Read_Reg(ACCEL_ZOUT_L);
   BUF[5]=MPU9250_Read_Reg(ACCEL_ZOUT_H);
   mpu_source_value.Accel[2]= (BUF[5]<<8)|BUF[4];
   mpu_value.Accel[2] = mpu_source_value.Accel[2] / 164.0; 					      //��ȡ����Z������
}
/**********************�����Ƕ�ȡ*****************************/
void READ_MPU9250_GYRO(void)
{

   BUF[0]=MPU9250_Read_Reg(GYRO_XOUT_L);
   BUF[1]=MPU9250_Read_Reg(GYRO_XOUT_H);
   mpu_source_value.Gyro[0]= (BUF[1]<<8)|BUF[0];
   mpu_value.Gyro[0] = mpu_source_value.Gyro[0] / 16.4 * PI / 180; 						   //��ȡ����X������

   BUF[2]=MPU9250_Read_Reg(GYRO_YOUT_L);
   BUF[3]=MPU9250_Read_Reg(GYRO_YOUT_H);
   mpu_source_value.Gyro[1]= (BUF[3]<<8)|BUF[2];
   mpu_value.Gyro[1] = mpu_source_value.Gyro[1] / 16.4 * PI / 180;  						   //��ȡ����Y������
   BUF[4]=MPU9250_Read_Reg(GYRO_ZOUT_L);
   BUF[5]=MPU9250_Read_Reg(GYRO_ZOUT_H);
   mpu_source_value.Gyro[2]=	(BUF[5]<<8)|BUF[4];
   mpu_value.Gyro[2] = mpu_source_value.Gyro[2] / 16.4 * PI / 180;  					       //��ȡ����Z������
}


/**********************�����ƶ�ȡ***************************/
//i2c_Mag_read(AK8963_ST2_REG) �˲���ȡ����ʡ��
//���ݶ�ȡ�����Ĵ�����reading this register means data reading end
//AK8963_ST2_REG ͬʱ�������ݷ������������⹦��
//�����ο� MPU9250 PDF
/**********************************************************/
void READ_MPU9250_MAG(void)
{
	u8 x_axis,y_axis,z_axis;

	x_axis=i2c_Mag_read(AK8963_ASAX);// X�������ȵ���ֵ
	y_axis=i2c_Mag_read(AK8963_ASAY);
	z_axis=i2c_Mag_read(AK8963_ASAZ);

	if((i2c_Mag_read(AK8963_ST1_REG)&AK8963_ST1_DOR)==0)//data ready
	{
			//��ȡ����X������
		 BUF[0]=i2c_Mag_read(MAG_XOUT_L); //Low data
		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register & check Magnetic sensor overflow occurred
		 {
			 BUF[0]=i2c_Mag_read(MAG_XOUT_L);//reload data
		 }
		 BUF[1]=i2c_Mag_read(MAG_XOUT_H); //High data
		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
		 {
			 BUF[1]=i2c_Mag_read(MAG_XOUT_H);
		 }
		 mpu_source_value.Mag[0]=((BUF[1]<<8)|BUF[0])*(((x_axis-128)>>8)+1);		//�����Ⱦ��� ��ʽ��/RM-MPU-9250A-00 PDF/ 5.13
		 mpu_value.Mag[0] = ((float)mpu_source_value.Mag[0]);

		//��ȡ����Y������
			BUF[2]=i2c_Mag_read(MAG_YOUT_L); //Low data
		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
		 {
			 BUF[2]=i2c_Mag_read(MAG_YOUT_L);
		 }
		 BUF[3]=i2c_Mag_read(MAG_YOUT_H); //High data
		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
		 {
			 BUF[3]=i2c_Mag_read(MAG_YOUT_H);
		 }
		  mpu_source_value.Mag[1]=((BUF[3]<<8)|BUF[2])*(((y_axis-128)>>8)+1);
			mpu_value.Mag[1] = (float)mpu_source_value.Mag[1];



		//��ȡ����Z������
		 BUF[4]=i2c_Mag_read(MAG_ZOUT_L); //Low data
		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
		 {
			 BUF[4]=i2c_Mag_read(MAG_ZOUT_L);
		 }
		 BUF[5]=i2c_Mag_read(MAG_ZOUT_H); //High data
		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
		 {
			 BUF[5]=i2c_Mag_read(MAG_ZOUT_H);
		 }
		  mpu_source_value.Mag[2]=((BUF[5]<<8)|BUF[4])*(((z_axis-128)>>8)+1);
			mpu_value.Mag[2] = (float)mpu_source_value.Mag[2];
	}
}
