/***********************************
*函数名：Pass
*功能：用户登录
*时间：2017年2月25日 13:52:45
************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <sqlite3.h>

#include "Pass.h"

extern pNode head;

void Num2Str(int Num, char Str[])		//数字转换冲字符串
{
    int i = 0;
	char temp;
    while(Num != 0)
	{
	    Str[i] = Num % 10 + '0';
		Num = Num / 10;
		i++;
	}
	Str[i] = '\0';
	for(i = 0; i < ID_SIZE / 2;i++)
	{
	    temp = Str[i];
		Str[i] = Str[ID_SIZE - 2 - i];
		Str[ID_SIZE - 2 - i] = temp;
	}
}

int Callback1(void *para, int count, char **c_value,char **c_name)	//检测有无信息
{
    if(count != 0)
	{
	    return 1;
	}

	return 0;
}

int Callback(void *para, int count, char **c_value,char **c_name)	//检测有无信息，并获取用户昵称
{
    pNode pass = (pNode)para;
    if(count != 0)
	{
	    strcpy(pass->name,c_value[0]);
	    return 1;
	}

	return  0;
}

Node Pass(int client_sock)
{
    sqlite3 *db;	
	char *msg;		//数据库的返回信息
	char *etc = "etc/data.db";		//帐号密码数据库
	char buffer[BUFF_SIZE];		//缓存区
	int id_;		//随机生成的ID
	int stime;
	int id_flag = 0;		//是否生成可用ID的标志位
	int select_flag = 0;	//是否有查询结果的标志位

	Node pass;		//返回的信息

	while(1)
	{
		bzero(&pass,sizeof(pass));
		read(client_sock,&pass,sizeof(pass));	//读取客户端操作

		sqlite3_open(etc,&db);
		//创建帐号密码数据库
		sqlite3_exec(db,"create table user(id text primary key, name text,password text)",NULL,NULL,&msg);
		if(pass.action == 1)	//登录操作
		{
			pNode temp = head->next;
			while(temp != NULL)		//检查是否已经登录
			{
			    if(strcmp(temp->id,pass.id) == 0)
				{
				    pass.action = 6;		//已经登录，置标志位，发送给客户端
					write(client_sock,&pass,sizeof(pass));	
				    break;
				}
			    temp = temp->next;
			}

			if(temp == NULL)	//非在线状态，则正常登录
			{
		    	memset(buffer,0,sizeof(buffer));
				sprintf(buffer,"select name from user where id = '%s' AND password = '%s'",pass.id,pass.password);
				select_flag = sqlite3_exec(db,buffer,Callback,(void *)(&pass),&msg);
				if(select_flag != 0)		//存在该信息
				{
				    pass.action = 1;
					write(client_sock,&pass,sizeof(pass));	//登录成功
					sqlite3_close(db);
					return pass;
				}
				else
				{
				    memset(buffer,0,sizeof(buffer));
					sprintf(buffer,"select * from user where id = '%s'",pass.id);
					select_flag = sqlite3_exec(db,buffer,Callback1,NULL,&msg);	//检查有无该帐号
					if(select_flag != 0)
					{
					    pass.action = 2;	//密码错误
						write(client_sock,&pass,sizeof(pass));	
					}
					else
					{
					    pass.action = 3;	//帐号未注册
						write(client_sock,&pass,sizeof(pass));
					}
				}
			}	
		}
		else
		{
		    if(pass.action == 2)		//注册操纵
			{
				memset(buffer,0,sizeof(buffer));
				sprintf(buffer,"select * from user where name = '%s'",pass.name);	//查找用户名是否重复
				select_flag = sqlite3_exec(db,buffer,Callback,(void *)(&pass),&msg);
				if(select_flag == 0)		//该用户名未注册
				{
				    do
					{
				   	 	stime = (unsigned int)(time(NULL)) / 2;
						srand(stime);
						id_ = rand() % 9000 + 1000;
						Num2Str(id_,pass.id);
						memset(buffer,0,sizeof(buffer));
						sprintf(buffer,"select * from user where id = '%s'",pass.id);
						id_flag = sqlite3_exec(db,buffer,Callback,NULL,&msg);
					}while(id_flag != 0);	//生成随机ID，并检测数据库中是否有相同ID，相同则重新生成

			   		memset(buffer,0,sizeof(buffer));
					sprintf(buffer,"insert into user (id,name,password) values ('%s','%s','%s')",pass.id,pass.name,pass.password);
					sqlite3_exec(db,buffer,NULL,NULL,&msg);		//将信息插入到数据库中

					pass.action = 5;	//注册成功
					write(client_sock,&pass,sizeof(pass));
				}
				else
				{
				    pass.action = 4; 	//帐户已存在
					write(client_sock,&pass,sizeof(pass));
				}
			}
			else		//退出操作
			{
				pass.action = -1;
				sqlite3_close(db);
			    return pass;
			}
		}
	}
}
