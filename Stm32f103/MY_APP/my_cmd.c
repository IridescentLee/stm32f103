#include <stdio.h>
#include <string.h>
#include <math.h>

#include "my_cmd.h"
#include "stm32f10x.h"
#include "USART\usart.h"

MyCmdStruct myCmdStrct;

/*****************************************************************************************************/
/* USART_CMD ���������һЩָ�� */
#define CMD_NUM         10 //�ܵ�ָ������, ����ָ��ϵͳ�������CMD_NUM��ָ��
#define CMD_PARA_NUM    10 //�ܵ�ָ���������, һ��ָ�������CMD_PARA_NUM������
#define CMD_PARA_LEN    20 //ָ���������, ָ������ĳ������ΪCMD_PARA_LEN���ֽ�
#define CMD_HELP_LEN    100 //ָ��˵���ĳ���, ˵�����CMD_HELP_LEN�ֽ�

u8      cmdParaNum = 0; //�洢ָ����������ı���
char    cmdParaString[CMD_PARA_NUM][CMD_PARA_LEN]; //�洢ָ������ı���


/*****************************************************************************************************/
/* Private function decleration */
u8 StringCompare( char *str1, char *str2);
uint32_t String2Number( char *str);
void Help( void);
/*****************************************************************************************************/

typedef struct
{
    char name[30];
    u8 paraNum;
    void (*func)( void);
    char help[100];
    char para[CMD_PARA_NUM][CMD_PARA_LEN];
}USART_CMD;

USART_CMD RS232_CMD[CMD_NUM] = { {"Help\0", 1, Help, "Example: Help. This command print all commands examples and info.\0"}};


/*****************************************************************************************************/


/************************************************************************/
/*
FunctionName    :StringCompare
Description     :�Ƚ��ַ���, �ַ����������Ϊ'\0'
para1           :str1 �ַ�ָ��
para2           :str2 �ַ�ָ��
return          :result �ԱȽ��, �ַ�����ͬ����1, ��ͬ����0
Author/Ver/Date :LY/1.00/20180314
*/
/************************************************************************/
u8 StringCompare( char *str1, char *str2)
{
    uint8_t i = 0;
    
    if( str1[0] == '\0')
    {
        return 0;
    }
    
    while( str1[i] != '\0')
    {
        if( str1[i] == str2[i])
        {
            i++;
        }
        else
        {
            return 0;
        }
    }
    
    return 1;
}

/************************************************************************/
/*
FunctionName    :String2Number
Description     :�ַ���ת��Ϊ����
para1           :None
return          :��ֵ
Author/Ver/Date :LY/1.01/20180316
*/
/************************************************************************/
uint32_t String2Number( char *str)
{
    uint8_t i = 0, len = 0;
    uint8_t strTmp[CMD_PARA_LEN];
    uint32_t tmp = 0;

    while( str[i] != '\0')
    {
        strTmp[i] = str[i];
        i++;
        len++;
    }

    for( i = 0; i < len; i++)
    {
        tmp += (strTmp[i]-0x30) * pow( 10, len-i-1);
    }

    return tmp;
}

/************************************************************************/
/*
FunctionName    :RS232_Command
Description     :������ִ�д���ָ��
para1           :None
return          :None
Author/Ver/Date :LY/1.01/20180316
*/
/************************************************************************/
/************************************************************************/
/*
FunctionName    :MyCommand
Description     :������ִ�д���ָ��
para1           :None
return          :None
Author/Ver/Date :LY/2.00/20180429
Note            :�޸ĺ�������ͬʱ������������������幦��δ�ı�
*/
/************************************************************************/
void MyCommand( void)
{
/* ����ָ����� */
// ָ�� SetLocalIP 192 168 1 10
// ָ���������5��, ����ָ����SetLocalIPҲ��һ������
    uint32_t i = 0;
    uint8_t j = 0, k = 0;

    if( myCmdStrct.myCmdAck == 1)
    {
        cmdParaNum = 1;
        j = 0;
        for( i = 0; i < myCmdStrct.myCmdRxNum; i++) //�����������ַ���ʽ���뻺��, �����Կո����
        {
            if( myCmdStrct.myCmdBuff[i] != ' ')
            {
                cmdParaString[cmdParaNum-1][j] = myCmdStrct.myCmdBuff[i];
                j++;
            }
            else
            {
                cmdParaString[cmdParaNum-1][j] = '\0'; //ÿ������ĩβ��'\0'
                cmdParaNum++;
                j = 0;
            }
            cmdParaString[cmdParaNum-1][j] = '\0'; //���һ������ĩβ��'\0'
        }

        j = 0;
        k = 0;
        for( i = 0; i < CMD_NUM; i++) //�����ָ������Ƿ��Ѿ�������
        {
            if( StringCompare( RS232_CMD[i].name, cmdParaString[0]) == 1)
            {
                if( cmdParaNum == RS232_CMD[i].paraNum) //����������
                {
                    for( j = 0; j < cmdParaNum; j++)
                    {
                        while( cmdParaString[j][k] != '\0')
                        {
                            RS232_CMD[i].para[j][k] = cmdParaString[j][k];
                            k++;
                        }
                        RS232_CMD[i].para[j][k] = '\0'; //ĩβ��'\0'
                        k = 0;
                    }
                    
                    (*RS232_CMD[i].func)(); //ִ��ָ������
                    break;
                }
                else
                {
                    printf("\r\nCommand parameter number error.\r\n");
                    break;
                }
            }
        }

        myCmdStrct.myCmdAck     = 0;
        myCmdStrct.myCmdRxNum   = 0;
    }
}

/************************************************************************/
/*
FunctionName    :Help
Description     :��ӡָ����Ϣ
para1           :None
return          :None
Author/Ver/Date :LY/1.01/20180329
*/
/************************************************************************/
void Help( void)
{
    uint8_t i = 0;

    printf("\r\nPrint command information.\r\n\r\n");

    while( RS232_CMD[i].name[0] != '\0')
    {
        printf("Command name: %s.\r\n", RS232_CMD[i].name);
        printf("%s\r\n\r\n", RS232_CMD[i].help);
        i++;
    }
}
