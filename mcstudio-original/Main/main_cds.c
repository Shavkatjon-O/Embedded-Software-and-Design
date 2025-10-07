#include "config.h"

#ifdef CDS
	
void main_cds (void)
{
	unsigned int Data_ADC2 = 0;
	init_devices();
	while (1){
		Data_ADC2 = Read_Adc_Data(2) / 10;		// �Ƴ��α� 0�� ��Ʈ �б�
		_delay_ms(200);				// ������ 200ms
		
		lcd_clear();				// �׷��� LCD Ŭ����
		ScreenBuffer_clear();			// ��ũ�� ���� Ŭ����
		lcd_string(0,0,"ADC2 CDS");			// ADC0 Potentiometer ���
		GLCD_Circle(35,55,Data_ADC2);		// ���� ������ ���
		lcd_xy(1,0); GLCD_4DigitDecimal(Data_ADC2); 	// ADC2�� ���� ���

	}
}
#endif
	