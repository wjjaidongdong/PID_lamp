#include "bh1750.h"

uint8_t mcy=0;
uint8_t BUF[3];
/***��ʼ�ź�**/
void BH1750_Start()
{
    HAL_GPIO_WritePin(GPIO, sda,GPIO_PIN_SET);                    //����������
    HAL_GPIO_WritePin(GPIO, scl,GPIO_PIN_SET);                   //����ʱ����
    delay_nus(5);
    HAL_GPIO_WritePin(GPIO, sda,GPIO_PIN_RESET);                    //�����½���
    delay_nus(5);
    HAL_GPIO_WritePin(GPIO, scl,GPIO_PIN_RESET);                    //����ʱ����
}

/*****ֹͣ�ź�******/
void BH1750_Stop()
{
    HAL_GPIO_WritePin(GPIO, sda,GPIO_PIN_RESET);                   //����������
    HAL_GPIO_WritePin(GPIO, scl,GPIO_PIN_SET);                      //����ʱ����
    delay_nus(5);
    HAL_GPIO_WritePin(GPIO, sda,GPIO_PIN_SET);                    //����������
    delay_nus(5);
}
/*****��ʼ��BH1750******/
void Init_BH1750()
{
    BH1750_Start();                                                 //��ʼ�ź�
    BH1750_SendByte(SlaveAddress);                                  //�����豸��ַ+д�ź�
    BH1750_SendByte(0x01);                                  //�ڲ��Ĵ�����ַ
    BH1750_Stop();                                                  //ֹͣ�ź�

}

//��������BH1750�ڲ�����
void mread(void)
{
    uint8_t i;
    BH1750_Start();                          //��ʼ�ź�
    BH1750_SendByte(SlaveAddress+1);         //�����豸��ַ��+���ź�

    for (i=0; i<3; i++)                      //������ȡ6����ַ���ݵ�BUF
    {
        BUF[i] = BH1750_RecvByte();
        if (i == 3)
        {
            BH1750_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {
            BH1750_SendACK(0);                //��ӦACK
        }
    }

    BH1750_Stop();                          //ֹͣ�ź�
    Delay_mms(5);

}

uint32_t Value_GY30(void)
{
    uint16_t dis_data;
    uint16_t Value_GY_30;
    Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode
    HAL_Delay(180);            //��ʱ180ms
    mread();       //�����������ݣ��洢��BUF��
    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];//�ֽںϳ�����
    Value_GY_30=(float)dis_data/1.2;
    return Value_GY_30;
}
//ϵͳ��Ƶ72MHZ
void delay_nus(uint16_t us)
{
    while(us--)
    {
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();
    }
}
void Delay_mms(uint16_t tmp)
{
    uint16_t i=0;
    while(tmp--)
    {
        i=12000;
        while(i--);
    }
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(int ack)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = scl|sda;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIO, &GPIO_InitStruct);
    if(ack == 1)   //дӦ���ź�
        HAL_GPIO_WritePin(GPIO, sda,GPIO_PIN_SET);
    else if(ack == 0)
        HAL_GPIO_WritePin(GPIO, sda,GPIO_PIN_RESET);
    else
        return;

    HAL_GPIO_WritePin(GPIO, scl,GPIO_PIN_SET);
    delay_nus(5);
    HAL_GPIO_WritePin(GPIO, scl,GPIO_PIN_RESET);
    delay_nus(5);
}

/**************************************
����Ӧ���ź�
**************************************/
int BH1750_RecvACK()
{

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  /*��������*/
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = sda;
    HAL_GPIO_Init(GPIO, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIO, scl,GPIO_PIN_SET);            //����ʱ����
    delay_nus(5);

    if(HAL_GPIO_ReadPin( GPIO, sda ) == 1 )//��Ӧ���ź�
        mcy = 1 ;
    else
        mcy = 0 ;

    HAL_GPIO_WritePin(GPIO, scl,GPIO_PIN_RESET);                    //����ʱ����
    delay_nus(5);

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init( GPIO, &GPIO_InitStruct );

    return mcy;
}

/**************************************
��iic���߷���һ���ֽ�����
**************************************/
void BH1750_SendByte(uint8_t dat)
{
    uint8_t i;

    for (i=0; i<8; i++)         //8λ������
    {
        if( 0X80 & dat )
            HAL_GPIO_WritePin(GPIO, sda,GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(GPIO, sda,GPIO_PIN_RESET);

        dat <<= 1;
        HAL_GPIO_WritePin(GPIO, scl,GPIO_PIN_SET);               //����ʱ����
        delay_nus(5);
        HAL_GPIO_WritePin(GPIO, scl,GPIO_PIN_RESET);                //����ʱ����
        delay_nus(5);
    }
    BH1750_RecvACK();
}

//���Ƕ�BH1750��������ʱ��Ҫ�ȷ���������ַ+д��λ��Ȼ����ָ��
//��ȡ���ݵ�ʱ����Ҫ�ȷ���������ַ+����λ��Ȼ���ȡ���ֽ�����

//д��ָ��
void Single_Write_BH1750(uint8_t REG_Address)//REG_Address��Ҫд���ָ��
{
    BH1750_Start();                  //��ʼ�ź�
    BH1750_SendByte(SlaveAddress);  //����������ַ+д�ź�
    BH1750_SendByte(REG_Address);   //д��ָ��ڲ��Ĵ�����ַ
    BH1750_Stop();                   //�����ź�
}
/**************************************
��iic���߶�ȡһ���ֽڵ�ַ
**************************************/
uint8_t BH1750_RecvByte()
{
    uint8_t i;
    uint8_t dat = 0;
    uint8_t bit;

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;   /*��������*/
    GPIO_InitStruct.Pin = sda;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( GPIO, &GPIO_InitStruct );

    HAL_GPIO_WritePin(GPIO, sda,GPIO_PIN_SET);          //׼����ȡ����
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        HAL_GPIO_WritePin(GPIO, scl,GPIO_PIN_SET);               //����ʱ����
        delay_nus(5);

        if( SET == HAL_GPIO_ReadPin( GPIO, sda ) )
            bit = 0X01;
        else
            bit = 0x00;

        dat |= bit;             //������

        HAL_GPIO_WritePin(GPIO, scl,GPIO_PIN_RESET);                //����ʱ����
        delay_nus(5);
    }

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init( GPIO, &GPIO_InitStruct );
    return dat;
}