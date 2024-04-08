#include "timer.h" 
#include "ht32.h"
#include "ht32_board.h"
#include "ht32f5xxxx_01.h"
#include "sys.h"
#include "usart.h"


void GPTM0_Init(void)
{
  TM_TimeBaseInitTypeDef TM_TimeBaseInitStruct;  // �����ṹ��
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.GPTM0      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
  
  TM_TimeBaseInitStruct.Prescaler = 7199; // Ԥ��Ƶϵ��
  TM_TimeBaseInitStruct.CounterReload = 99; // ��������
  TM_TimeBaseInitStruct.RepetitionCounter = 0;
  TM_TimeBaseInitStruct.CounterMode = TM_CNT_MODE_UP; // ����ģʽ
  TM_TimeBaseInitStruct.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;// ������װ��  
  TM_TimeBaseInit(HT_GPTM0, &TM_TimeBaseInitStruct);  
  
  TM_ClearIntPendingBit(HT_GPTM0, TM_INT_UEV); // ����жϱ�־λ     
  TM_IntConfig(HT_GPTM0, TM_INT_UEV, ENABLE); //����GPTM0��ʱ���ж�
  NVIC_EnableIRQ(GPTM0_IRQn);   
  
  NVIC_SetPriority(GPTM0_IRQn,2);
 
  TM_Cmd(HT_GPTM0, ENABLE); // ����GPTM0
}


void GPTM0_IRQHandler(void)
{
  if( TM_GetIntStatus(HT_GPTM0, TM_INT_UEV) ==SET ) // �ж϶�ʱ�������ж� �Ƿ���  TM_GetFlagStatus (HT_GPTM0,  TM_FLAG_UEV) 
  {

	USART0_RX_STA|=1<<15;
	TM_ClearIntPendingBit(HT_GPTM0, TM_INT_UEV);  
	TM_Cmd(HT_GPTM0, DISABLE); // �ر�GPTM0
                    
  }  
}




//ͨ�ö�ʱ��7�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
//void TIM3_Int_Init(u16 arr,u16 psc)
//{	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//TIM3ʱ��ʹ��    
//	
//	//��ʱ��TIM3��ʼ��
//	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
// 
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
//	
//	TIM_Cmd(TIM3,ENABLE);//������ʱ��7
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
//	
//}

////��ʱ��7�жϷ������		    
//void TIM3_IRQHandler(void)
//{ 	
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//�Ǹ����ж�
//	{	 			   
//		USART3_RX_STA|=1<<15;	//��ǽ������
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIM3�����жϱ�־    
//		TIM_Cmd(TIM3, DISABLE);  //�ر�TIM3 
//	}	    
//}












