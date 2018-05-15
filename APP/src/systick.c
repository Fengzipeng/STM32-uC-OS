#include "systick.h"
#include "os_cpu.h"
#include "ucos_ii.h"
#include "os_cfg.h"
static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������

//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void SysTick_Init(void)	 
{
	u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	fac_us=SystemCoreClock/8000000;	//Ϊϵͳʱ�ӵ�1/8  
	 
	reload=SystemCoreClock/8000000;	//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/OS_TICKS_PER_SEC;//����OS_TICKS_PER_SEC�趨���ʱ��
									//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	fac_ms=1000/OS_TICKS_PER_SEC;//����ucos������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 	//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    
}								    

//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void Delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD��ֵ	    	 
	ticks=nus*fac_us; 			//��Ҫ�Ľ�����	  		 
	tcnt=0;
	told=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}; 									    
}
//��ʱnms
//nms:Ҫ��ʱ��ms��
void Delay_ms(u32 nms)
{	
	if(OSRunning==TRUE)//���os�Ѿ�������	    
	{		  
		if(nms>=fac_ms)//��ʱ��ʱ�����ucos������ʱ������ 
		{
   			OSTimeDly(nms/fac_ms);//ucos��ʱ
		}
		nms%=fac_ms;				//ucos�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	Delay_us((u32)(nms*1000));	//��ͨ��ʽ��ʱ,��ʱucos�޷���������.
}
void SysTick_Handler(void)
{
	  OSIntEnter();		      //�����ж�
    OSTimeTick();       //����ucos��ʱ�ӷ������               
    OSIntExit();        //���������л����ж�
}
/****************************************/
#ifdef OS_DISABLE
void Delay_ms(u32 x)
{
	 u32 temp;
	SysTick->CTRL=0x01;
	SysTick->LOAD=9000*x;
	SysTick->VAL=0;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));
	SysTick->VAL=0;
	SysTick->CTRL=0;
}
void Delay_us(u32 x)
{
	 u32 temp;
	SysTick->CTRL=0x01;
	SysTick->LOAD=9*x;
	SysTick->VAL=0;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));
	SysTick->VAL=0;
	SysTick->CTRL=0;
}
#endif