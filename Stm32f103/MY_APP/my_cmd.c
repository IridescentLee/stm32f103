#include <stdio.h>
#include <string.h>
#include <math.h>

#include "my_cmd.h"
#include "stm32f10x.h"
#include "USART\usart.h"

MyCmdStruct myCmdStrct;

/*****************************************************************************************************/
/* USART_CMD 串口输入的一些指令 */
#define CMD_NUM         10 //总的指令数量, 整个指令系统中最多有CMD_NUM个指令
#define CMD_PARA_NUM    10 //总的指令参数数量, 一个指令最多有CMD_PARA_NUM个参数
#define CMD_PARA_LEN    20 //指令参数长度, 指令参数的长度最大为CMD_PARA_LEN个字节
#define CMD_HELP_LEN    100 //指令说明的长度, 说明最大长CMD_HELP_LEN字节

u8      cmdParaNum = 0; //存储指令参数个数的变量
char    cmdParaString[CMD_PARA_NUM][CMD_PARA_LEN]; //存储指令参数的变量


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
Description     :比较字符串, 字符串结束标记为'\0'
para1           :str1 字符指针
para2           :str2 字符指针
return          :result 对比结果, 字符串相同返回1, 不同返回0
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
Description     :字符串转换为数字
para1           :None
return          :数值
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
Description     :解析和执行串口指令
para1           :None
return          :None
Author/Ver/Date :LY/1.01/20180316
*/
/************************************************************************/
/************************************************************************/
/*
FunctionName    :MyCommand
Description     :解析和执行串口指令
para1           :None
return          :None
Author/Ver/Date :LY/2.00/20180429
Note            :修改函数名，同时整理参数变量名，具体功能未改变
*/
/************************************************************************/
void MyCommand( void)
{
/* 串口指令解析 */
// 指令 SetLocalIP 192 168 1 10
// 指令参数个数5个, 其中指令名SetLocalIP也算一个参数
    uint32_t i = 0;
    uint8_t j = 0, k = 0;

    if( myCmdStrct.myCmdAck == 1)
    {
        cmdParaNum = 1;
        j = 0;
        for( i = 0; i < myCmdStrct.myCmdRxNum; i++) //串口数据以字符形式存入缓存, 参数以空格隔开
        {
            if( myCmdStrct.myCmdBuff[i] != ' ')
            {
                cmdParaString[cmdParaNum-1][j] = myCmdStrct.myCmdBuff[i];
                j++;
            }
            else
            {
                cmdParaString[cmdParaNum-1][j] = '\0'; //每个参数末尾补'\0'
                cmdParaNum++;
                j = 0;
            }
            cmdParaString[cmdParaNum-1][j] = '\0'; //最后一个参数末尾补'\0'
        }

        j = 0;
        k = 0;
        for( i = 0; i < CMD_NUM; i++) //查表检查指令代码是否已经被定义
        {
            if( StringCompare( RS232_CMD[i].name, cmdParaString[0]) == 1)
            {
                if( cmdParaNum == RS232_CMD[i].paraNum) //检查参数数量
                {
                    for( j = 0; j < cmdParaNum; j++)
                    {
                        while( cmdParaString[j][k] != '\0')
                        {
                            RS232_CMD[i].para[j][k] = cmdParaString[j][k];
                            k++;
                        }
                        RS232_CMD[i].para[j][k] = '\0'; //末尾加'\0'
                        k = 0;
                    }
                    
                    (*RS232_CMD[i].func)(); //执行指令内容
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
Description     :打印指令信息
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
