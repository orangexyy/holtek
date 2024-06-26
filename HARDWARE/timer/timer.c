#include "timer.h" 
#include "ht32.h"
#include "ht32_board.h"
#include "ht32f5xxxx_01.h"
#include "sys.h"
#include "usart.h"


void GPTM0_Init(void)
{
  TM_TimeBaseInitTypeDef TM_TimeBaseInitStruct;  // 声明结构体
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.GPTM0      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
  
  TM_TimeBaseInitStruct.Prescaler = 7199; // 预分频系数
  TM_TimeBaseInitStruct.CounterReload = 99; // 计数周期
  TM_TimeBaseInitStruct.RepetitionCounter = 0;
  TM_TimeBaseInitStruct.CounterMode = TM_CNT_MODE_UP; // 计数模式
  TM_TimeBaseInitStruct.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;// 立即重装载  
  TM_TimeBaseInit(HT_GPTM0, &TM_TimeBaseInitStruct);  
  
  TM_ClearIntPendingBit(HT_GPTM0, TM_INT_UEV); // 清除中断标志位     
  TM_IntConfig(HT_GPTM0, TM_INT_UEV, ENABLE); //开启GPTM0定时器中断
  NVIC_EnableIRQ(GPTM0_IRQn);   
  
  NVIC_SetPriority(GPTM0_IRQn,2);
 
  TM_Cmd(HT_GPTM0, ENABLE); // 开启GPTM0
}


void GPTM0_IRQHandler(void)
{
  if( TM_GetIntStatus(HT_GPTM0, TM_INT_UEV) ==SET ) // 判断定时器更新中断 是否发生  TM_GetFlagStatus (HT_GPTM0,  TM_FLAG_UEV) 
  {

	USART0_RX_STA|=1<<15;
	TM_ClearIntPendingBit(HT_GPTM0, TM_INT_UEV);  
	TM_Cmd(HT_GPTM0, DISABLE); // 关闭GPTM0
                    
  }  
}




//通用定时器7中断初始化
//这里时钟选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz 
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
//void TIM3_Int_Init(u16 arr,u16 psc)
//{	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//TIM3时钟使能    
//	
//	//定时器TIM3初始化
//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
//	
//	TIM_Cmd(TIM3,ENABLE);//开启定时器7
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//	
//}

////定时器7中断服务程序		    
//void TIM3_IRQHandler(void)
//{ 	
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//是更新中断
//	{	 			   
//		USART3_RX_STA|=1<<15;	//标记接收完成
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIM3更新中断标志    
//		TIM_Cmd(TIM3, DISABLE);  //关闭TIM3 
//	}	    
//}












