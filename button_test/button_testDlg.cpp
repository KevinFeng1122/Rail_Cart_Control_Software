
// button_testDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "button_test.h"
#include "button_testDlg.h"
#include<math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cbutton_testDlg 对话框


Cbutton_testDlg::Cbutton_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cbutton_testDlg::IDD, pParent)
	, m_StrSend(_T(""))
	, effective_char_num_from_text_input(0)
	, m_Speed(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cbutton_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_Port_State, m_port_state);
	DDX_Control(pDX, IDC_MSCOMM1, m_MsComm);
	DDX_Text(pDX, IDC_EDIT_Send, m_StrSend);
	DDX_Control(pDX, IDC_EDIT_MessageReceived, m_edit_MsgRec);
	DDX_Text(pDX, IDC_EDIT1, m_Speed);
	DDV_MinMaxFloat(pDX, m_Speed, 0.01, 0.4);
	DDX_Control(pDX, IDC_RADIO_CWL_LIMIT, m_RADIO_CWL_Limit);
	DDX_Control(pDX, IDC_RADIO_CCWL_LIMIT, m_RADIO_CCWL_Limit);
	DDX_Control(pDX, IDC_RADIO_STOP, m_RADIO_STOP);
	DDX_Control(pDX, IDC_RADIO_CWL, m_RADIO_CWL);
	DDX_Control(pDX, IDC_RADIO_CCWL, m_RADIO_CCWL);
	DDX_Control(pDX, IDC_BUTTON_GoOrStop, m_BUTTON_GoOrStop);
	DDX_Control(pDX, IDC_CHECK_Command_Input_Mode, m_CHECK_Input_Mode);
}

BEGIN_MESSAGE_MAP(Cbutton_testDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_Port_Connect, &Cbutton_testDlg::OnBnClickedButtonPortConnect)
	ON_BN_CLICKED(IDC_BUTTON_Send, &Cbutton_testDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_Speed_Confirm, &Cbutton_testDlg::OnBnClickedButton_Speed_Confirm)
	ON_BN_CLICKED(IDC_BUTTON_Forward, &Cbutton_testDlg::OnBnClickedButton_Forward)
	ON_BN_CLICKED(IDC_BUTTON_Back, &Cbutton_testDlg::OnBnClickedButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_GoOrStop, &Cbutton_testDlg::OnBnClickedButtonGoorstop)
	ON_BN_CLICKED(IDC_CHECK_Command_Input_Mode, &Cbutton_testDlg::OnBnClickedCheckCommandInputMode)
	ON_BN_CLICKED(IDC_BUTTON_MsgSend_Clear, &Cbutton_testDlg::OnBnClickedButtonMsgsendClear)
END_MESSAGE_MAP()


// Cbutton_testDlg 消息处理程序

BOOL Cbutton_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	flag_motor_dirction_option=NONE_OPTION;
	flag_thread_continue=FALSE;
	m_RADIO_STOP.SetCheck(TRUE);
	m_Speed=0.01;
	UpdateData(FALSE);
	//如果串口打开，则关闭串口
	if(m_MsComm.get_PortOpen())
		m_MsComm.put_PortOpen(FALSE);
	//如果串口还没有关闭，则报错并终止程序
	if(m_MsComm.get_PortOpen())
	{
		AfxMessageBox("串口错误！");
		AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
		return FALSE;//???终止程序
	}
	//串口连接前使按钮和复选框无效
	GetDlgItem(IDC_BUTTON_Speed_Confirm)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Forward)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Back)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_GoOrStop)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MsgSend_Clear)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Send)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_Command_Input_Mode)->EnableWindow(FALSE);
	//add existing serial ports to the combo-box
	CComboBox *pointer_port = (CComboBox *)GetDlgItem(IDC_COMBO_Port);
	FindSerialPort(pointer_port);

	m_brush.m_hObject = NULL;
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cbutton_testDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cbutton_testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// //找出可用串口
void Cbutton_testDlg::FindSerialPort(CComboBox * pComboBox)
{
	HKEY hKey;
#ifdef _DEBUG
	ASSERT( pComboBox != NULL );
	pComboBox->AssertValid();
#endif
	if( ::RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		"Hardware\\DeviceMap\\SerialComm",
		NULL,
		KEY_READ,
		&hKey) == ERROR_SUCCESS) // 打开串口注册表
	{
		int i=0;
		char portName[256],commName[256];
		DWORD dwLong,dwSize;
		while(1)
		{
			dwLong = dwSize = sizeof(portName);
			if( ::RegEnumValue( hKey,
				i,
				portName,
				&dwLong,
				NULL,
				NULL,
				(PUCHAR)commName,
				&dwSize ) == ERROR_NO_MORE_ITEMS ) // 枚举串口
				break;
			pComboBox->AddString( commName ); // commName就是串口名字
			AfxMessageBox(commName);
			i++;
		}

		if( pComboBox->GetCount() == 0 )
		{
			::AfxMessageBox("在HKEY_LOCAL_MACHINE:Hardware\\DeviceMap\\SerialComm里找不到串口!!!" );
		}
		RegCloseKey(hKey);
	}
}


HBRUSH Cbutton_testDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_Port_State && m_brush.m_hObject != NULL)
	{
		hbr = m_brush;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void Cbutton_testDlg::OnBnClickedButtonPortConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//判断串口是否打开
	if(!m_MsComm.get_PortOpen())
	{	//如果还没有打开串口，则
		//先判断是否选择了串口号，
			//若没选串口号或者没有可选的串口号，
			//则警告并跳出函数
		CComboBox *pointer_port = (CComboBox *)GetDlgItem(IDC_COMBO_Port);
		char port_count=pointer_port->GetCount();
		if((port_count==0) | (pointer_port->GetCurSel()==CB_ERR))
		{
			AfxMessageBox("尚未选择串口号，无法进行连接");
			return;
		}

		//获取要连接的串口号，用char变量port_num表示
		char port_num;
		CString port_name;
		GetDlgItemText(IDC_COMBO_Port,port_name);
		port_name.Replace("COM", "");
		port_num = atoi(port_name.GetBuffer(0));
		port_name.ReleaseBuffer();
		//设置串口属性
		m_MsComm.put_CommPort(port_num); //选择COM1端口
		m_MsComm.put_InputMode(1); //输入方式为二进制方式
		m_MsComm.put_InBufferSize(1024); //设置输入缓冲区
		m_MsComm.put_OutBufferSize(512); //设置输出缓冲区
		m_MsComm.put_Settings(TEXT("57600,n,8,1"));//波特率，无校验，个数据位，个停止位
		//打开串口
		m_MsComm.put_PortOpen(TRUE);
		m_MsComm.put_RThreshold(1); //每当接收缓冲区有1个字符则接收串口数据
		m_MsComm.put_InputLen(0); //设置当前缓冲区长度为
		m_MsComm.get_Input(); //预读缓冲区以清除残留数据
		//再次判断串口是否打开，
		if(m_MsComm.get_PortOpen())
		{
			//若打开则静态控件显示“已打开”，
			m_port_state.SetWindowText("已连接");
			GetDlgItem(IDC_BUTTON_Port_Connect)->SetWindowText("断开");
			//串口连接后使按钮和复选框有效
			GetDlgItem(IDC_BUTTON_Speed_Confirm)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_Forward)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_Back)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_GoOrStop)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_Command_Input_Mode)->EnableWindow(TRUE);
			//向伺服控制器下发电机传配置命令
			//以下命令每次上电都要操作！
			//内部速度模式(PA11=0)  	【01 06 00 11 00 00 D9 CF】
			UCHAR PA11[]={0x01,0x06,0x00,0x11,0x00,0x00,0xD9,0xCF};
			SendCommand(PA11,8);
			TimeDelay(1000);
			//最高转速PA66设定5100  【01 06 00 66 13 EC 65 68】
			UCHAR PA66[]={0x01,0x06,0x00,0x66,0x13,0xEC,0x65,0x68};
			SendCommand(PA66,8);
			TimeDelay(1000);
			//使能CWL和CCWL的限位功能PA(7D)=00【01 06 00 7D 00 00 19 D2 】
			UCHAR PA7D[]={0x01,0x06,0x00,0x7D,0x00,0x00,0x19,0xD2};
			SendCommand(PA7D,8);
			TimeDelay(1000);
		}
		else
		{
			//否则报错并跳出函数
			AfxMessageBox("串口错误！");
			return;
		}
	}
	else
	{
		//关闭串口
		m_MsComm.put_PortOpen(FALSE);
		//再次判断串口是否关闭，
		if(!m_MsComm.get_PortOpen())
		{
			//若关闭则静态控件显示“未打开”，
			m_port_state.SetWindowText("未连接");
			GetDlgItem(IDC_BUTTON_Port_Connect)->SetWindowText("连接");
			m_CHECK_Input_Mode.SetCheck(FALSE);
			//断开连接后使按钮和复选框无效
			GetDlgItem(IDC_BUTTON_Speed_Confirm)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_Forward)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_Back)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_GoOrStop)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MsgSend_Clear)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_Send)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_Command_Input_Mode)->EnableWindow(FALSE);
		}
		else
		{
			//否则报错并跳出函数
			AfxMessageBox("串口错误！");
			return;
		}
	}
}
//串口发送函数
//入口：char* char_array_in 保存了待发送16进制数的char型数组
//返回：void
void Cbutton_testDlg::SendCommand(UCHAR* char_array_in)
{
	TRY 
	{
		CByteArray bytMiddleArr;
		bytMiddleArr.SetSize(effective_char_num_from_text_input);
		bytMiddleArr.RemoveAll();
		for (UCHAR i=0;i<effective_char_num_from_text_input;i++)
		{
			char num_char=char_array_in[i];
			bytMiddleArr.Add( num_char);
		}

		COleVariant varOut;
		varOut=COleVariant(bytMiddleArr); //将数据转换为变体数据类型
		m_MsComm.put_Output(varOut); //发送数据
	}
	CATCH (CMemoryException, e)
	{
		TCHAR   szCause[255];
		CString strFormatted;

		e->GetErrorMessage(szCause, 255);

		strFormatted = _T("Excpetion: ");
		strFormatted += szCause;

		AfxMessageBox(strFormatted);

	}
	END_CATCH
	
}

// 返回TRUE表示转换成功，FALSE表示失败
bool Cbutton_testDlg::string_to_num(CString string_in, UCHAR* char_array)
{
	if(string_in=="") 
	{
		AfxMessageBox("输入为空!");
		return false;
	}//字符串为空，跳出函数

	//将string_in末尾的空格去掉，然后获得字符个数
	UCHAR string_char_num=string_in.GetLength();
	int char_index;//从右侧数连续第N个空格在字符串中的索引值
	for (char_index=string_char_num-1;(char_index>=0) & (string_in.GetAt(char_index)==' ');char_index--);
	char_index++;
	if (char_index==0)
	{
		AfxMessageBox("输入的内容不可都是空格!");
		return false;
	}
	string_in=string_in.Left(char_index);//去掉右侧连续的空格
	effective_char_num_from_text_input=(string_in.GetLength()+1)/3;//命令中16进制数的个数，例如01 00 23的该变量值=3

	string_in.MakeLower(); //全部转为小写

	//字符串string_in→	char b[4096]，去掉开头空格;
	//例如：“19 a2 09 8f"  →{1,9,a,2,0,9,8,f}　
	
	UCHAR b[200]={0};//中间数组
	int mPos=0;
	int i=0;
	CString m="";
	while(1) 
		{
			//去掉开头空格
			UCHAR k=0;
			for(k=0;k<4;k++)
			{
				mPos=string_in.Find(' ');
				if(mPos == 0) string_in=string_in.Right(string_in.GetLength()-1);
				else break;
			}
			if(k>3) //开头空格多于4个，跳出函数
			{
				AfxMessageBox("输入格式不正确，请检查是否有多余空格！");
				return false;
			}

			//没有发现空格，跳出循环。
			//最终剩余两个字符时利用此最终跳出循环
			if(mPos==-1) break; 

			m=string_in.Left(mPos);
			for(unsigned char j=0;j<m.GetLength();j++) b[i++]=m.GetAt(j);
			mPos=string_in.GetLength()-mPos;
			string_in=string_in.Right(mPos);
		}

		if(string_in.GetLength() == 2)//处理最终剩余两个字符的情况
		{
			b[i++]=string_in.GetAt(0);
			b[i++]=string_in.GetAt(1);
		}
		else
		{
			AfxMessageBox("输入了非十六进制数据或非法格式!");
			return false;
	  }

		//b[]字符向数字转换，放到char数组里
		//例如：b[]={1,9,a,2,0,9,8,f}→char_array[]={0x19,0xa2,0x09,0x8f}
		UCHAR k=0;
		for(unsigned char n=0;n<i;n+=2)
		  {
			  if(b[n] >= '0' && b[n] <= '9')  b[n]=b[n]-0x30; //输入的是数字
			  else if(b[n] >= 'a' && b[n] <= 'f')  b[n]=b[n]-87; //输入的是字
			  else
			  {
				  AfxMessageBox("输入了非十六进制数据或非法格式!");
				  return false;
			  }

			  if(b[n+1] >= '0' && b[n+1] <= '9')  b[n+1]=b[n+1]-0x30; //输入的是数字
			  else if(b[n+1] >= 'a' && b[n+1] <= 'f')  b[n+1]=b[n+1]-87; //输入的是字
			  else
			  {
				  AfxMessageBox("输入了非十六进制数据或非法格式!");
				  return false;
			  }
			  char_array[k++] = b[n]*16+b[n+1];
		 }
	return true;
}

void Cbutton_testDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString command_to_send;
	//GetDlgItemText(IDC_EDIT_Send,(LPTSTR)&command_to_send,300);
	UpdateData(true);

	UCHAR command_16_array[100]={0};
	bool str_to_num_ok=	string_to_num(m_StrSend,command_16_array);
	if (!str_to_num_ok)
		return;
	SendCommand(command_16_array);

	CWinThread* pointerThread;
	pointerThread=AfxBeginThread(THREAD_Command_Input_Mode,this); //创建查询线程
}


void Cbutton_testDlg::TimeDelay(int cycle_times)//不知效果如何
{
	for(int i=0;i<cycle_times;i++);
}

void Cbutton_testDlg::SendCommand(UCHAR* char_array_in, int element_num)
{
	TRY 
	{
		CByteArray bytMiddleArr;
		bytMiddleArr.SetSize(element_num);
		bytMiddleArr.RemoveAll();
		for (UCHAR i=0;i<element_num;i++)
		{
			char num_char=char_array_in[i];
			bytMiddleArr.Add( num_char);
		}

		COleVariant varOut;
		varOut=COleVariant(bytMiddleArr); //将数据转换为变体数据类型
		m_MsComm.put_Output(varOut); //发送数据
	}
	CATCH (CMemoryException, e)
	{
		TCHAR   szCause[255];
		CString strFormatted;

		e->GetErrorMessage(szCause, 255);

		strFormatted = _T("Excpetion: ");
		strFormatted += szCause;

		AfxMessageBox(strFormatted);

	}
	END_CATCH
	
}

void Cbutton_testDlg::OnBnClickedButton_Speed_Confirm()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if ((m_Speed<0.009) | (m_Speed>0.41))
		//边界不可设置为0.01和0.4，否则边界值执行不了
		return;
	short motor_speed_n=ceil(4583.65*m_Speed*2);//取上整，返回比x大的最小整数
	UCHAR	n_high=motor_speed_n>>8;
	UCHAR n_low=motor_speed_n & 0x00FF;
	//char  message_show[10];
	//sprintf(message_show, "%7.2f",n_low);
	//AfxMessageBox(message_show);
	//sprintf(message_show, "%7.2f",n_high);
	//AfxMessageBox(message_show);
	UCHAR test_command[11]={0x01,0x10,0x00,0x57,0x00,0x01,0x02,0x00,0x00,0x00,0x00};
	//e.g. 01 10 00 57 00 01 02 00 64 AA 5C 
	//e.g. 0.01m/s的速度对应指令
	test_command[7]=n_high;
	test_command[8]=n_low;
	USHORT test_result=usMBCRC161(test_command,9);
	UCHAR	CRC16_low=test_result>>8;//取验证结果的高字节
	UCHAR CRC16_high=test_result & 0x00FF;//取验证结果的低字节
	//CHAR test_show[10];
	//itoa(CRC16_low,test_show,16);
	//AfxMessageBox(test_show);//运算结果5caa
	test_command[9]=CRC16_high;
	test_command[10]=CRC16_low;
	SendCommand(test_command,11);
}

// CRC16验证码生成函数
//返回两字节，高低位需要倒置
USHORT Cbutton_testDlg::usMBCRC161(UCHAR * pucFrame, USHORT usLen)
{
	UCHAR ucCRCHi = 0xFF;
	UCHAR ucCRCLo = 0xFF;
	int iIndex;
	while( usLen-- )
	{
		iIndex = ucCRCLo ^ *( pucFrame++ );
		ucCRCLo = ( UCHAR )( ucCRCHi ^ aucCRCHi[iIndex] );
		ucCRCHi = aucCRCLo[iIndex];
	}
	return ( USHORT )( ucCRCHi << 8 | ucCRCLo );
}

void Cbutton_testDlg::OnBnClickedButton_Forward()
{
	// TODO: 在此添加控件通知处理程序代码
	int if_CWL_limit= m_RADIO_CWL_Limit.GetCheck();
	if (if_CWL_limit==BST_CHECKED)
	{
		AfxMessageBox("电机正转限位报警时，不可下达正转指令！");
		return;
	}
	flag_motor_dirction_option=CWL;
	//PA【4A】=0; 01 06 00 4A 00 00 A8 1C
	UCHAR x4A[8]={0x01,0x06,0x00,0x4A,0x00,0x00,0xA8,0x1C};
	SendCommand(x4A,8);
}

void Cbutton_testDlg::OnBnClickedButtonBack()
{
	// TODO: 在此添加控件通知处理程序代码
	int	if_CCWL_limit= m_RADIO_CCWL_Limit.GetCheck();
	if (if_CCWL_limit==BST_CHECKED)
	{
		AfxMessageBox("电机反转限位报警时，不可下达反转指令！");
		return;
	}
	flag_motor_dirction_option=CCWL;
	//PA【4A】=1;01 06 00 4A 00 01 69 DC
	UCHAR x4A[8]={0x01,0x06,0x00,0x4A,0x00,0x01,0x69,0xDC};
	SendCommand(x4A,8);
}

UINT Cbutton_testDlg::InquiryThread(LPVOID pParam)
{
	Cbutton_testDlg* pParlPro=( Cbutton_testDlg*)pParam;
	bool while_continue1=true;
	bool while_continue2=true;
	CString strtemp="";
	CString outtemp="";
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len,k;
	byte rxdata[512]; //设置BYTE数组
	do 
	{
		pParlPro->m_MsComm.get_Input(); //预读缓冲区以清除残留数据
		while_continue1=true;
		while_continue2=true;
		if (pParlPro->flag_motor_dirction_option==CWL)
		{
			//查询CWL限位
			//读取：IN5 CWL 133【01 01 00 85 00 01 EC 23】
			UCHAR d133[8]={0x01,0x01,0x00,0x85,0x00,0x01,0xEC,0x23};
			pParlPro->SendCommand(d133,8);
			Sleep(10);
			do 
			{
				switch (pParlPro->m_MsComm.get_CommEvent())
				{
				case 2://数据接收事件
					Sleep(10);//保证读完所有数据
					variant_inp=pParlPro->m_MsComm.get_Input(); //读缓冲区消息
					safearray_inp=variant_inp; ///变量转换
					len=safearray_inp.GetOneDimSize(); //得到有效的数据长度
					for(k=0;k<len;k++)
						safearray_inp.GetElement(&k,rxdata+k);
					for(k=0;k<len;k++) //将数组转换为CString型变量
					{
						UCHAR bt=*(UCHAR*)(rxdata+k);//字符型
						strtemp.Format("%.2X",bt); //将字符送入临时变量strtemp存放
						if(k<len-1)
							strtemp+=" ";
						outtemp+=strtemp; //
					}
					if (outtemp!="")
						while_continue1=false;
					break;
				default:
					break;
				}
			} while (while_continue1);
			if (outtemp=="01 01 01 01 90 48")//01 01 01 01 90 48
			{			
				//CWL报警
				/*
				更改RADIO为CWL限位
				下传控制器“伺服停止”指令
				下传控制器IN8=1
				下传控制器IN8=0
				*/
				pParlPro->m_RADIO_CWL_Limit.SetCheck(TRUE);
				pParlPro->m_RADIO_CCWL_Limit.SetCheck(FALSE);
				pParlPro->m_RADIO_CWL.SetCheck(FALSE);
				pParlPro->m_RADIO_CCWL.SetCheck(FALSE);
				pParlPro->m_RADIO_STOP.SetCheck(FALSE);

				pParlPro->flag_motor_dirction_option=NONE_OPTION;
				pParlPro->flag_thread_continue=FALSE;
				pParlPro->m_BUTTON_GoOrStop.SetWindowText("伺服开");
				//PA【x6】=1;01 05 00 06 00 00 2D CB
				UCHAR x6[8]={0x01,0x05,0x00,0x06,0x00,0x00,0x2D,0xCB};
				pParlPro->SendCommand(x6,8);
				Sleep(30);
				//pParlPro->flag_motor_dirction_option=CCWL;
				////PA【4A】=1;01 06 00 4A 00 01 69 DC
				//UCHAR x4A[8]={0x01,0x06,0x00,0x4A,0x00,0x01,0x69,0xDC};
				//pParlPro->SendCommand(x4A,8);
				//Sleep(30);
				//PA【x8_set_1】=1;01 05 00 08 FF 00 0D F8
				UCHAR x8_set_1[8]={0x01,0x05,0x00,0x08,0xFF,0x00,0x0D,0xF8};
				pParlPro->SendCommand(x8_set_1,8);
				Sleep(30);
				//PA【x8_set_0】=1;01 05 00 08 00 00 4C 08
				UCHAR x8_set_0[8]={0x01,0x05,0x00,0x08,0x00,0x00,0x4C,0x08};
				pParlPro->SendCommand(x8_set_0,8);
				Sleep(30);
				return 0;
			} 
			outtemp="";
		} 
		if (pParlPro->flag_motor_dirction_option==CCWL)
		{
			//step2: 查询CCWL限位
			//读取：IN7 CWL 135【01 01 00 87 00 01 4D E3】
			UCHAR d135[8]={0x01,0x01,0x00,0x87,0x00,0x01,0x4D,0xE3};
			pParlPro->SendCommand(d135,8);
			Sleep(10);
			do 
			{
				switch (pParlPro->m_MsComm.get_CommEvent())
				{
				case 2://数据接收事件
					Sleep(10);//保证读完所有数据
					variant_inp=pParlPro->m_MsComm.get_Input(); //读缓冲区消息
					safearray_inp=variant_inp; ///变量转换
					len=safearray_inp.GetOneDimSize(); //得到有效的数据长度
					for(k=0;k<len;k++)
						safearray_inp.GetElement(&k,rxdata+k);
					for(k=0;k<len;k++) //将数组转换为CString型变量
					{
						UCHAR bt=*(UCHAR*)(rxdata+k);//字符型
						strtemp.Format("%.2X",bt); //将字符送入临时变量strtemp存放
						if(k<len-1)
							strtemp+=" ";
						outtemp+=strtemp; //
					}
					if (outtemp!="")
						while_continue2=false;
					break;
				default:
					break;
				}
			} while (while_continue2);
			if (outtemp=="01 01 01 01 90 48")
			{
				//CCWL报警
				/*
				更改RADIO为CCWL限位
				下传控制器“伺服停止”指令
				下传控制器IN8=1
				下传控制器IN8=0
				*/
				pParlPro->m_RADIO_CCWL_Limit.SetCheck(TRUE);
				pParlPro->m_RADIO_CWL_Limit.SetCheck(FALSE);
				pParlPro->m_RADIO_CWL.SetCheck(FALSE);
				pParlPro->m_RADIO_CCWL.SetCheck(FALSE);
				pParlPro->m_RADIO_STOP.SetCheck(FALSE);

				pParlPro->m_BUTTON_GoOrStop.SetWindowText("伺服开");
				pParlPro->flag_motor_dirction_option=NONE_OPTION;
				pParlPro->flag_thread_continue=FALSE;
				//PA【x6】=1;01 05 00 06 00 00 2D CB
				UCHAR x6[8]={0x01,0x05,0x00,0x06,0x00,0x00,0x2D,0xCB};
				pParlPro->SendCommand(x6,8);
				Sleep(30);
				//pParlPro->flag_motor_dirction_option=CWL;
				////PA【4A】=0; 01 06 00 4A 00 00 A8 1C
				//UCHAR x4A[8]={0x01,0x06,0x00,0x4A,0x00,0x00,0xA8,0x1C};
				//pParlPro->SendCommand(x4A,8);
				//Sleep(30);
				//PA【x8_set_1】=1;01 05 00 08 FF 00 0D F8
				UCHAR x8_set_1[8]={0x01,0x05,0x00,0x08,0xFF,0x00,0x0D,0xF8};
				pParlPro->SendCommand(x8_set_1,8);
				Sleep(30);
				//PA【x8_set_0】=1;01 05 00 08 00 00 4C 08
				UCHAR x8_set_0[8]={0x01,0x05,0x00,0x08,0x00,0x00,0x4C,0x08};
				pParlPro->SendCommand(x8_set_0,8);
				Sleep(30);
				return 0;
			} 
			outtemp="";
		}
		
		switch(pParlPro->flag_motor_dirction_option)
		{
		case CWL:
			pParlPro->m_RADIO_CWL.SetCheck(TRUE);
			pParlPro->m_RADIO_CCWL_Limit.SetCheck(FALSE);
			pParlPro->m_RADIO_CWL_Limit.SetCheck(FALSE);
			pParlPro->m_RADIO_CCWL.SetCheck(FALSE);
			pParlPro->m_RADIO_STOP.SetCheck(FALSE);
			break;
		case CCWL:
			pParlPro->m_RADIO_CCWL.SetCheck(TRUE);
			pParlPro->m_RADIO_CWL_Limit.SetCheck(FALSE);
			pParlPro->m_RADIO_CCWL_Limit.SetCheck(FALSE);
			pParlPro->m_RADIO_CWL.SetCheck(FALSE);
			pParlPro->m_RADIO_STOP.SetCheck(FALSE);
			break;
		case NONE_OPTION:
			pParlPro->m_RADIO_STOP.SetCheck(TRUE);
			pParlPro->m_RADIO_CWL_Limit.SetCheck(FALSE);
			pParlPro->m_RADIO_CCWL_Limit.SetCheck(FALSE);
			pParlPro->m_RADIO_CWL.SetCheck(FALSE);
			pParlPro->m_RADIO_CCWL.SetCheck(FALSE);
			break;
		}
	} while (pParlPro->flag_thread_continue);
	return 0;
}

UINT Cbutton_testDlg::THREAD_Command_Input_Mode(LPVOID pParam)
{
	Cbutton_testDlg* pParlPro=( Cbutton_testDlg*)pParam;
	bool while_continue=true;
	CString strtemp="";
	CString outtemp="";
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len,k;
	byte rxdata[512]; //设置BYTE数组
	do 
	{
		switch (pParlPro->m_MsComm.get_CommEvent())
		{
		case 2://数据接收事件
			Sleep(10);//保证读完所有数据
			variant_inp=pParlPro->m_MsComm.get_Input(); //读缓冲区消息
			safearray_inp=variant_inp; ///变量转换
			len=safearray_inp.GetOneDimSize(); //得到有效的数据长度
			for(k=0;k<len;k++)
				safearray_inp.GetElement(&k,rxdata+k);
			for(k=0;k<len;k++) //将数组转换为CString型变量
			{
				UCHAR bt=*(UCHAR*)(rxdata+k);//字符型
				strtemp.Format("%.2X",bt); //将字符送入临时变量strtemp存放
				if(k<len-1)
					strtemp+=" ";
				outtemp+=strtemp; //加入接收编辑框对应字符串
			}
			if (outtemp!="")
				while_continue=false;
			break;
		default:
			//AfxMessageBox("case default");
			break;
		}
	} while (while_continue);
	//pParlPro->m_strReceive=outtemp;
	//pParlPro->UpdateData(FALSE);【多线程时不要使用该方法！】
	pParlPro->m_edit_MsgRec.SetWindowText(outtemp);
	outtemp="";
	return 0;
}

void Cbutton_testDlg::OnBnClickedButtonGoorstop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (flag_thread_continue)
	{
		flag_thread_continue=false;
		flag_motor_dirction_option=NONE_OPTION;
		GetDlgItem(IDC_BUTTON_GoOrStop)->SetWindowText("伺服开");
		//伺服使能（x6）【01 05 00 06 00 00 2D CB】
		UCHAR x6[8]={0x01,0x05,0x00,0x06,0x00,0x00,0x2D,0xCB};
		SendCommand(x6,8);
	} 
	else
	{
		if (flag_motor_dirction_option==NONE_OPTION)
		{
			AfxMessageBox("请先选择电机转动方向！");
			return;
		}
		flag_thread_continue=true;
		GetDlgItem(IDC_BUTTON_GoOrStop)->SetWindowText("伺服关");
		//伺服使能（x6）【01 05 00 06 FF 00 6C 3B】
		UCHAR x6[8]={0x01,0x05,0x00,0x06,0xFF,0x00,0x6C,0x3B};
		SendCommand(x6,8);
		m_MsComm.get_Input(); //预读缓冲区以清除残留数据
		//创建查询线程
		CWinThread* pointerThread;
		pointerThread=AfxBeginThread(InquiryThread,this); 
	}
}

void Cbutton_testDlg::OnBnClickedCheckCommandInputMode()
{
	// TODO: 在此添加控件通知处理程序代码
	//单击先自动勾选复选框，然后再执行下面的代码
	if (m_RADIO_STOP.GetCheck()==BST_UNCHECKED)
	{
		m_CHECK_Input_Mode.SetCheck(BST_UNCHECKED);
		AfxMessageBox("停止伺服后才可以使用该模式！");
		return;
	}
	switch(m_CHECK_Input_Mode.GetCheck())
	{
	case BST_CHECKED:
		GetDlgItem(IDC_BUTTON_Speed_Confirm)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_Forward)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_Back)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_GoOrStop)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MsgSend_Clear)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_Send)->EnableWindow(TRUE);
		AfxMessageBox("退出此模式前，请确保电机处于停止状态，且没有限位报警发生！");
		break;
	case BST_UNCHECKED:
		CString mode_msg="退出此模式前，请确保电机处于停止状态，且没有限位报警发生！是否确定退出？";
		if (AfxMessageBox(mode_msg,MB_YESNO)==IDNO)
		{
			m_CHECK_Input_Mode.SetCheck(BST_CHECKED);
			return;
		}
		
		GetDlgItem(IDC_BUTTON_Speed_Confirm)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_Forward)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_Back)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_GoOrStop)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MsgSend_Clear)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_Send)->EnableWindow(FALSE);
		break;
	}
}

void Cbutton_testDlg::OnBnClickedButtonMsgsendClear()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_Send)->SetWindowText("");
}
