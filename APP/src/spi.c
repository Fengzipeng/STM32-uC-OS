#include "spi.h"
void SPI1_Init()  //SPI1��ʼ��
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef	 SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//��SPI1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;		  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex; //	SPI ����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;	//����SPIΪ��ģʽ
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;	//����SPI���ݴ�СΪ8λ
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;	//����SPIĬ��ʱ������Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;	//����SPI���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;	//����SPI NSS�ܽ�����������
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;	//����SPI ������Ԥ��ƵֵΪ 256
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;	//����SPI ���ݴӸ�λ����
	SPI_InitStructure.SPI_CRCPolynomial=7;	//����SPI CRCֵ����Ķ���ʽ
	SPI_Init(SPI1,&SPI_InitStructure);	
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1		
}

u8 SPI1_ReadWriteByte(u8 dat)		 //SPI1��дһ���ֽ�
{
  u8 t,data;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)	//�ȴ����ͻ�������
	{
		t++;
		if(t>=200)return 0;	//��ʱ���ش����־	
	}
	SPI_I2S_SendData(SPI1,dat); //��������
	t=0;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)	//�ȴ�����
	{
		t++;
		if(t>=200)return 0;	//��ʱ���ش����־	
	}
	data=SPI_I2S_ReceiveData(SPI1);
	return data; //�������SPI2���յ�����			
}
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)	//����SPI2���ٶ�
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	
	SPI1->CR1&=0XFFC7;		 //�޸�BR[2:0]ֵ
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
	SPI_Cmd(SPI1,ENABLE);			
}

