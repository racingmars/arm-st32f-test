#include "stm32f4xx.h"

void delay(int a);

int status = 0;
int status1 = 0;

int main(void) {

    /* Enable GPIO peripheral clock */
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);

    /* Set D12, D13, D14, D15 to output */
    MODIFY_REG(GPIOD->MODER, GPIO_MODER_MODER12, GPIO_MODER_MODER12_0);
    MODIFY_REG(GPIOD->MODER, GPIO_MODER_MODER13, GPIO_MODER_MODER13_0);
    MODIFY_REG(GPIOD->MODER, GPIO_MODER_MODER14, GPIO_MODER_MODER14_0);
    MODIFY_REG(GPIOD->MODER, GPIO_MODER_MODER15, GPIO_MODER_MODER15_0);

    /* Set D13 to 1 */
    SET_BIT(GPIOD->ODR, GPIO_ODR_ODR_13);
    SET_BIT(GPIOD->ODR, GPIO_ODR_ODR_12);
    SET_BIT(GPIOD->ODR, GPIO_ODR_ODR_14);
    status1=1;
    status=1;

    /* Setup interrupt EXTI0 on PA0 rising edge */
    MODIFY_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI0, SYSCFG_EXTICR1_EXTI0_PA);
    SET_BIT(EXTI->IMR, EXTI_IMR_MR0);
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR0);
    NVIC_SetPriority(EXTI0_IRQn,1);
    NVIC_EnableIRQ(EXTI0_IRQn);

    /* Set D15 to 1 */
    SET_BIT(GPIOD->ODR, GPIO_ODR_ODR_15);

    /* Loop forever */
    while(1) {
        delay(500000);
        if(status1) {
            CLEAR_BIT(GPIOD->ODR, GPIO_ODR_ODR_12);
            status1 = 0;
        } else {
            SET_BIT(GPIOD->ODR, GPIO_ODR_ODR_12);
            status1 = 1;
        }
    }
}

void delay(int a) {
    volatile int i,j;

    for(i=0; i<a; i++) {
        j++;
    }

    return;
}

void EXTI0_IRQHandler() {
    if(status==1) {
        CLEAR_BIT(GPIOD->ODR, GPIO_ODR_ODR_14);
        status = 0;
    } else {
        SET_BIT(GPIOD->ODR, GPIO_ODR_ODR_14);
        status = 1;
    }
    delay(100000);
    SET_BIT(EXTI->PR, EXTI_PR_PR0);
}
