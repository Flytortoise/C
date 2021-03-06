/***********************************
*函数名：Select_func
*功能：各功能入口函数
*时间：2017年2月25日 14:31:00
************************************/

#include <stdio.h>
#include <strings.h>
#include <pthread.h>

#include "func.h"
#include "Pass.h"

int Select_func(int client_sock,int *flag)
{
	Node user;
	read(client_sock,&user,sizeof(user));		//获取具体操作

	switch(user.action)
	{
		case -1:		//被踢出则直接返回
		{
		    return 0;
		}break;

	    case 1:
		{
		    Look(client_sock);		//查看当前用户
		}break;

	    case 2:
		{
		    Chat(client_sock);		//私聊
		}break;

	    case 3:
		{
		    All_Chat(client_sock);	//群聊
		}break;
	    
		case 4:
		{
		    Fast(client_sock);		//发送快捷消息
		}break;

	    case 5:
		{
		    Face(client_sock);		//发送表情
		}break;

	    case 6:
		{
		    Change_name(client_sock);		//更改用户名
		}break;

	    case 7:
		{
		    Change_passwd(client_sock);		//更改密码
		}break;

	    case 8:
		{
		    			//查看帮助，服务器端无需操作
		}break;

	    case 9:
		{
			File(client_sock);
		}break;

	    case 10:
		{
		    return Exit_Log(client_sock);		//退出登录
		}break;

	    case 11:
		{
		    Exit(client_sock,flag);		//退出聊天室
		}break;

		default :
		{
		}break;
	}

	return 0;
}


//超级用户功能
int Select_func2(int client_sock,int *flag)
{
	Node user;
	read(client_sock,&user,sizeof(user));
	switch(user.action)
	{
	    case 1:
		{
		    Look(client_sock);
		}break;

	    case 2:
		{
		    Chat(client_sock);
		}break;

	    case 3:
		{
		    All_Chat(client_sock);
		}break;
	    
		case 4:
		{
		    Fast(client_sock);
		}break;

	    case 5:
		{
		    Face(client_sock);
		}break;

	    case 6:
		{
		    Change_name(client_sock);
		}break;

	    case 7:
		{
		    Change_passwd(client_sock);
		}break;

	    case 8:
		{
		    
		}break;

	    case 9:
		{
			File(client_sock);
		}break;

	    case 10:
		{
		    return Exit_Log(client_sock);
		}break;

	    case 11:
		{
		    Exit(client_sock,flag);
		}break;

		case 12:
		{

		}break;

	    case 13:
		{
		    Remove(client_sock);		//踢出用户
		}break;

	    case 14:
		{
		    OffChat(client_sock);		//禁言
		}break;

	    case 15:
		{
		    OnChat(client_sock);		//解除禁言
		}break;

		default :
		{
		}break;
	}

	return 0;
}
