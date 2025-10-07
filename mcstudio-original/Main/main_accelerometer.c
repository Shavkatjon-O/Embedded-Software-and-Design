#include "config.h"

#ifdef ACCELEROMETER
	
void main_accelerometer (void)
{
	unsigned int Data_ADC5 = 0;
	unsigned int Data_ADC6 = 0;
	unsigned int Data_ADC7 = 0;
	init_devices();
	while (1){
		Data_ADC5 = Read_Adc_Data(5)-352+32;	// �Ƴ��α� 5�� ��Ʈ �б�
		Data_ADC6 = Read_Adc_Data(6)-358+62;	// �Ƴ��α� 6�� ��Ʈ �б�
		Data_ADC7 = Read_Adc_Data(7);		// �Ƴ��α� 7�� ��Ʈ �б�
		_delay_ms(300);			// ������ 300ms
		
		lcd_clear();				// �׷��� LCD Ŭ����
		ScreenBuffer_clear();			// ��ũ�� ���� Ŭ����

		lcd_xy(1,0);	GLCD_4DigitDecimal(Data_ADC5);
		lcd_xy(2,0);	GLCD_4DigitDecimal(Data_ADC6);
		lcd_xy(3,0);	GLCD_4DigitDecimal(Data_ADC7);
		
		lcd_string(0,0,"ADC5,ADC6 Acceleration");	// ADC0 Potentiometer ���
		GLCD_Line(32,0,31,127);
		GLCD_Line(0,63,63,63);
		GLCD_Circle(Data_ADC5,Data_ADC6,5);
		GLCD_Circle(32,62,5);
	}

}
#endif	