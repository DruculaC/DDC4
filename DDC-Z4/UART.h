


#define FOSC 13500000L
#define BAUD 600




//����ͨ������





extern unsigned char code PassWord[1000];






/*
�������� void InitUART()
���ܣ� 	��ʼ��UART
˵����
��ڲ�������
����ֵ��  ��
*/
void InitUART();




/*
�������� void SendData(BYTE dat)
���ܣ� 	���ͺ���
˵����
��ڲ�����Ҫ���͵��ֽ�
����ֵ��  ��
*/
void SendData(unsigned char dat);




/*
��������void SendNByte(BYTE *buf,BYTE len)
���ܣ� ���Ͷ���ֽ�
˵����
��ڲ�����Ҫ���͵����ݵ�ַ�����͵��ֽڳ���
����ֵ�� ��
*/
void SendNByte(unsigned char *buf,unsigned char len);














