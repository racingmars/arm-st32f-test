#include "stm32f4xx.h"

void delay(int a);

extern int HSEStartupStatus;

int main(void) {

    /* Enable peripheral clocks */
//    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
//    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);

    /* Set D12, D13, D14, D15 to output */
    MODIFY_REG(GPIOD->MODER, GPIO_MODER_MODER12, GPIO_MODER_MODER12_0);
    MODIFY_REG(GPIOD->MODER, GPIO_MODER_MODER13, GPIO_MODER_MODER13_0);
    MODIFY_REG(GPIOD->MODER, GPIO_MODER_MODER14, GPIO_MODER_MODER14_0);
    MODIFY_REG(GPIOD->MODER, GPIO_MODER_MODER15, GPIO_MODER_MODER15_0);

    /* A8, C9 to high speed output */
//    MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEEDR8,
//            GPIO_OSPEEDER_OSPEEDR8_0|GPIO_OSPEEDER_OSPEEDR8_1);
//    MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR9,
//            GPIO_OSPEEDER_OSPEEDR9_0|GPIO_OSPEEDER_OSPEEDR9_1);

    /* A8, C9 to alternate function */
//    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER8, GPIO_MODER_MODER8_1);
//    MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODER9, GPIO_MODER_MODER9_1);

    /* Set D13 to 1 */
    if(!HSEStartupStatus) {
        SET_BIT(GPIOD->ODR, GPIO_ODR_ODR_13);
    }

    /* Setup interrupt EXTI0 on PA0 rising edge */
    MODIFY_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI0, SYSCFG_EXTICR1_EXTI0_PA);
    SET_BIT(EXTI->IMR, EXTI_IMR_MR0);
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR0);
    NVIC_SetPriority(EXTI0_IRQn,4);
    NVIC_EnableIRQ(EXTI0_IRQn);

    /* Setup TIM2 */
    TIM2->PSC = (uint16_t)8399;
    TIM2->ARR = (uint16_t)499;
    SET_BIT(TIM2->CR1, TIM_CR1_URS);
    SET_BIT(TIM2->DIER, TIM_DIER_UIE);
    //SET_BIT(TIM2->EGR, TIM_EGR_UG);
    //CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
    NVIC_SetPriority(TIM2_IRQn, 3);
    NVIC_EnableIRQ(TIM2_IRQn);
    SET_BIT(TIM2->CR1, TIM_CR1_CEN);

    /* Set D15 to 1 */
    //SET_BIT(GPIOD->ODR, GPIO_ODR_ODR_15);

    /* Loop forever */
    while(1) {
        delay(6000000);
        GPIOD->ODR ^= GPIO_ODR_ODR_15;
    }
}

void delay(int a) {
    volatile uint32_t i,j;

    for(i=0; i<a; i++) {
        j++;
    }

    return;
}

void EXTI0_IRQHandler() {
    GPIOD->ODR ^= GPIO_ODR_ODR_14;
    delay(1000000);
    SET_BIT(EXTI->PR, EXTI_PR_PR0);
}

void TIM2_IRQHandler() {
    if(READ_BIT(TIM2->SR, TIM_SR_UIF) != 0) {
        GPIOD->ODR ^= GPIO_ODR_ODR_12;
        CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
    }
}
