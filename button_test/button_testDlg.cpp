
// button_testDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "button_test.h"
#include "button_testDlg.h"
#include<math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cbutton_testDlg �Ի���


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


// Cbutton_testDlg ��Ϣ�������

BOOL Cbutton_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	flag_motor_dirction_option=NONE_OPTION;
	flag_thread_continue=FALSE;
	m_RADIO_STOP.SetCheck(TRUE);
	m_Speed=0.01;
	UpdateData(FALSE);
	//������ڴ򿪣���رմ���
	if(m_MsComm.get_PortOpen())
		m_MsComm.put_PortOpen(FALSE);
	//������ڻ�û�йرգ��򱨴���ֹ����
	if(m_MsComm.get_PortOpen())
	{
		AfxMessageBox("���ڴ���");
		AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
		return FALSE;//???��ֹ����
	}
	//��������ǰʹ��ť�͸�ѡ����Ч
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
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cbutton_testDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cbutton_testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// //�ҳ����ô���
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
		&hKey) == ERROR_SUCCESS) // �򿪴���ע���
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
				&dwSize ) == ERROR_NO_MORE_ITEMS ) // ö�ٴ���
				break;
			pComboBox->AddString( commName ); // commName���Ǵ�������
			AfxMessageBox(commName);
			i++;
		}

		if( pComboBox->GetCount() == 0 )
		{
			::AfxMessageBox("��HKEY_LOCAL_MACHINE:Hardware\\DeviceMap\\SerialComm���Ҳ�������!!!" );
		}
		RegCloseKey(hKey);
	}
}


HBRUSH Cbutton_testDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_Port_State && m_brush.m_hObject != NULL)
	{
		hbr = m_brush;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void Cbutton_testDlg::OnBnClickedButtonPortConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//�жϴ����Ƿ��
	if(!m_MsComm.get_PortOpen())
	{	//�����û�д򿪴��ڣ���
		//���ж��Ƿ�ѡ���˴��ںţ�
			//��ûѡ���ںŻ���û�п�ѡ�Ĵ��ںţ�
			//�򾯸沢��������
		CComboBox *pointer_port = (CComboBox *)GetDlgItem(IDC_COMBO_Port);
		char port_count=pointer_port->GetCount();
		if((port_count==0) | (pointer_port->GetCurSel()==CB_ERR))
		{
			AfxMessageBox("��δѡ�񴮿ںţ��޷���������");
			return;
		}

		//��ȡҪ���ӵĴ��ںţ���char����port_num��ʾ
		char port_num;
		CString port_name;
		GetDlgItemText(IDC_COMBO_Port,port_name);
		port_name.Replace("COM", "");
		port_num = atoi(port_name.GetBuffer(0));
		port_name.ReleaseBuffer();
		//���ô�������
		m_MsComm.put_CommPort(port_num); //ѡ��COM1�˿�
		m_MsComm.put_InputMode(1); //���뷽ʽΪ�����Ʒ�ʽ
		m_MsComm.put_InBufferSize(1024); //�������뻺����
		m_MsComm.put_OutBufferSize(512); //�������������
		m_MsComm.put_Settings(TEXT("57600,n,8,1"));//�����ʣ���У�飬������λ����ֹͣλ
		//�򿪴���
		m_MsComm.put_PortOpen(TRUE);
		m_MsComm.put_RThreshold(1); //ÿ�����ջ�������1���ַ�����մ�������
		m_MsComm.put_InputLen(0); //���õ�ǰ����������Ϊ
		m_MsComm.get_Input(); //Ԥ���������������������
		//�ٴ��жϴ����Ƿ�򿪣�
		if(m_MsComm.get_PortOpen())
		{
			//������̬�ؼ���ʾ���Ѵ򿪡���
			m_port_state.SetWindowText("������");
			GetDlgItem(IDC_BUTTON_Port_Connect)->SetWindowText("�Ͽ�");
			//�������Ӻ�ʹ��ť�͸�ѡ����Ч
			GetDlgItem(IDC_BUTTON_Speed_Confirm)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_Forward)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_Back)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_GoOrStop)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_Command_Input_Mode)->EnableWindow(TRUE);
			//���ŷ��������·��������������
			//��������ÿ���ϵ綼Ҫ������
			//�ڲ��ٶ�ģʽ(PA11=0)  	��01 06 00 11 00 00 D9 CF��
			UCHAR PA11[]={0x01,0x06,0x00,0x11,0x00,0x00,0xD9,0xCF};
			SendCommand(PA11,8);
			TimeDelay(1000);
			//���ת��PA66�趨5100  ��01 06 00 66 13 EC 65 68��
			UCHAR PA66[]={0x01,0x06,0x00,0x66,0x13,0xEC,0x65,0x68};
			SendCommand(PA66,8);
			TimeDelay(1000);
			//ʹ��CWL��CCWL����λ����PA(7D)=00��01 06 00 7D 00 00 19 D2 ��
			UCHAR PA7D[]={0x01,0x06,0x00,0x7D,0x00,0x00,0x19,0xD2};
			SendCommand(PA7D,8);
			TimeDelay(1000);
		}
		else
		{
			//���򱨴���������
			AfxMessageBox("���ڴ���");
			return;
		}
	}
	else
	{
		//�رմ���
		m_MsComm.put_PortOpen(FALSE);
		//�ٴ��жϴ����Ƿ�رգ�
		if(!m_MsComm.get_PortOpen())
		{
			//���ر���̬�ؼ���ʾ��δ�򿪡���
			m_port_state.SetWindowText("δ����");
			GetDlgItem(IDC_BUTTON_Port_Connect)->SetWindowText("����");
			m_CHECK_Input_Mode.SetCheck(FALSE);
			//�Ͽ����Ӻ�ʹ��ť�͸�ѡ����Ч
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
			//���򱨴���������
			AfxMessageBox("���ڴ���");
			return;
		}
	}
}
//���ڷ��ͺ���
//��ڣ�char* char_array_in �����˴�����16��������char������
//���أ�void
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
		varOut=COleVariant(bytMiddleArr); //������ת��Ϊ������������
		m_MsComm.put_Output(varOut); //��������
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

// ����TRUE��ʾת���ɹ���FALSE��ʾʧ��
bool Cbutton_testDlg::string_to_num(CString string_in, UCHAR* char_array)
{
	if(string_in=="") 
	{
		AfxMessageBox("����Ϊ��!");
		return false;
	}//�ַ���Ϊ�գ���������

	//��string_inĩβ�Ŀո�ȥ����Ȼ�����ַ�����
	UCHAR string_char_num=string_in.GetLength();
	int char_index;//���Ҳ���������N���ո����ַ����е�����ֵ
	for (char_index=string_char_num-1;(char_index>=0) & (string_in.GetAt(char_index)==' ');char_index--);
	char_index++;
	if (char_index==0)
	{
		AfxMessageBox("��������ݲ��ɶ��ǿո�!");
		return false;
	}
	string_in=string_in.Left(char_index);//ȥ���Ҳ������Ŀո�
	effective_char_num_from_text_input=(string_in.GetLength()+1)/3;//������16�������ĸ���������01 00 23�ĸñ���ֵ=3

	string_in.MakeLower(); //ȫ��תΪСд

	//�ַ���string_in��	char b[4096]��ȥ����ͷ�ո�;
	//���磺��19 a2 09 8f"  ��{1,9,a,2,0,9,8,f}��
	
	UCHAR b[200]={0};//�м�����
	int mPos=0;
	int i=0;
	CString m="";
	while(1) 
		{
			//ȥ����ͷ�ո�
			UCHAR k=0;
			for(k=0;k<4;k++)
			{
				mPos=string_in.Find(' ');
				if(mPos == 0) string_in=string_in.Right(string_in.GetLength()-1);
				else break;
			}
			if(k>3) //��ͷ�ո����4������������
			{
				AfxMessageBox("�����ʽ����ȷ�������Ƿ��ж���ո�");
				return false;
			}

			//û�з��ֿո�����ѭ����
			//����ʣ�������ַ�ʱ���ô���������ѭ��
			if(mPos==-1) break; 

			m=string_in.Left(mPos);
			for(unsigned char j=0;j<m.GetLength();j++) b[i++]=m.GetAt(j);
			mPos=string_in.GetLength()-mPos;
			string_in=string_in.Right(mPos);
		}

		if(string_in.GetLength() == 2)//��������ʣ�������ַ������
		{
			b[i++]=string_in.GetAt(0);
			b[i++]=string_in.GetAt(1);
		}
		else
		{
			AfxMessageBox("�����˷�ʮ���������ݻ�Ƿ���ʽ!");
			return false;
	  }

		//b[]�ַ�������ת�����ŵ�char������
		//���磺b[]={1,9,a,2,0,9,8,f}��char_array[]={0x19,0xa2,0x09,0x8f}
		UCHAR k=0;
		for(unsigned char n=0;n<i;n+=2)
		  {
			  if(b[n] >= '0' && b[n] <= '9')  b[n]=b[n]-0x30; //�����������
			  else if(b[n] >= 'a' && b[n] <= 'f')  b[n]=b[n]-87; //���������
			  else
			  {
				  AfxMessageBox("�����˷�ʮ���������ݻ�Ƿ���ʽ!");
				  return false;
			  }

			  if(b[n+1] >= '0' && b[n+1] <= '9')  b[n+1]=b[n+1]-0x30; //�����������
			  else if(b[n+1] >= 'a' && b[n+1] <= 'f')  b[n+1]=b[n+1]-87; //���������
			  else
			  {
				  AfxMessageBox("�����˷�ʮ���������ݻ�Ƿ���ʽ!");
				  return false;
			  }
			  char_array[k++] = b[n]*16+b[n+1];
		 }
	return true;
}

void Cbutton_testDlg::OnBnClickedButtonSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CString command_to_send;
	//GetDlgItemText(IDC_EDIT_Send,(LPTSTR)&command_to_send,300);
	UpdateData(true);

	UCHAR command_16_array[100]={0};
	bool str_to_num_ok=	string_to_num(m_StrSend,command_16_array);
	if (!str_to_num_ok)
		return;
	SendCommand(command_16_array);

	CWinThread* pointerThread;
	pointerThread=AfxBeginThread(THREAD_Command_Input_Mode,this); //������ѯ�߳�
}


void Cbutton_testDlg::TimeDelay(int cycle_times)//��֪Ч�����
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
		varOut=COleVariant(bytMiddleArr); //������ת��Ϊ������������
		m_MsComm.put_Output(varOut); //��������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if ((m_Speed<0.009) | (m_Speed>0.41))
		//�߽粻������Ϊ0.01��0.4������߽�ִֵ�в���
		return;
	short motor_speed_n=ceil(4583.65*m_Speed*2);//ȡ���������ر�x�����С����
	UCHAR	n_high=motor_speed_n>>8;
	UCHAR n_low=motor_speed_n & 0x00FF;
	//char  message_show[10];
	//sprintf(message_show, "%7.2f",n_low);
	//AfxMessageBox(message_show);
	//sprintf(message_show, "%7.2f",n_high);
	//AfxMessageBox(message_show);
	UCHAR test_command[11]={0x01,0x10,0x00,0x57,0x00,0x01,0x02,0x00,0x00,0x00,0x00};
	//e.g. 01 10 00 57 00 01 02 00 64 AA 5C 
	//e.g. 0.01m/s���ٶȶ�Ӧָ��
	test_command[7]=n_high;
	test_command[8]=n_low;
	USHORT test_result=usMBCRC161(test_command,9);
	UCHAR	CRC16_low=test_result>>8;//ȡ��֤����ĸ��ֽ�
	UCHAR CRC16_high=test_result & 0x00FF;//ȡ��֤����ĵ��ֽ�
	//CHAR test_show[10];
	//itoa(CRC16_low,test_show,16);
	//AfxMessageBox(test_show);//������5caa
	test_command[9]=CRC16_high;
	test_command[10]=CRC16_low;
	SendCommand(test_command,11);
}

// CRC16��֤�����ɺ���
//�������ֽڣ��ߵ�λ��Ҫ����
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int if_CWL_limit= m_RADIO_CWL_Limit.GetCheck();
	if (if_CWL_limit==BST_CHECKED)
	{
		AfxMessageBox("�����ת��λ����ʱ�������´���תָ�");
		return;
	}
	flag_motor_dirction_option=CWL;
	//PA��4A��=0; 01 06 00 4A 00 00 A8 1C
	UCHAR x4A[8]={0x01,0x06,0x00,0x4A,0x00,0x00,0xA8,0x1C};
	SendCommand(x4A,8);
}

void Cbutton_testDlg::OnBnClickedButtonBack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int	if_CCWL_limit= m_RADIO_CCWL_Limit.GetCheck();
	if (if_CCWL_limit==BST_CHECKED)
	{
		AfxMessageBox("�����ת��λ����ʱ�������´ﷴתָ�");
		return;
	}
	flag_motor_dirction_option=CCWL;
	//PA��4A��=1;01 06 00 4A 00 01 69 DC
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
	byte rxdata[512]; //����BYTE����
	do 
	{
		pParlPro->m_MsComm.get_Input(); //Ԥ���������������������
		while_continue1=true;
		while_continue2=true;
		if (pParlPro->flag_motor_dirction_option==CWL)
		{
			//��ѯCWL��λ
			//��ȡ��IN5 CWL 133��01 01 00 85 00 01 EC 23��
			UCHAR d133[8]={0x01,0x01,0x00,0x85,0x00,0x01,0xEC,0x23};
			pParlPro->SendCommand(d133,8);
			Sleep(10);
			do 
			{
				switch (pParlPro->m_MsComm.get_CommEvent())
				{
				case 2://���ݽ����¼�
					Sleep(10);//��֤������������
					variant_inp=pParlPro->m_MsComm.get_Input(); //����������Ϣ
					safearray_inp=variant_inp; ///����ת��
					len=safearray_inp.GetOneDimSize(); //�õ���Ч�����ݳ���
					for(k=0;k<len;k++)
						safearray_inp.GetElement(&k,rxdata+k);
					for(k=0;k<len;k++) //������ת��ΪCString�ͱ���
					{
						UCHAR bt=*(UCHAR*)(rxdata+k);//�ַ���
						strtemp.Format("%.2X",bt); //���ַ�������ʱ����strtemp���
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
				//CWL����
				/*
				����RADIOΪCWL��λ
				�´����������ŷ�ֹͣ��ָ��
				�´�������IN8=1
				�´�������IN8=0
				*/
				pParlPro->m_RADIO_CWL_Limit.SetCheck(TRUE);
				pParlPro->m_RADIO_CCWL_Limit.SetCheck(FALSE);
				pParlPro->m_RADIO_CWL.SetCheck(FALSE);
				pParlPro->m_RADIO_CCWL.SetCheck(FALSE);
				pParlPro->m_RADIO_STOP.SetCheck(FALSE);

				pParlPro->flag_motor_dirction_option=NONE_OPTION;
				pParlPro->flag_thread_continue=FALSE;
				pParlPro->m_BUTTON_GoOrStop.SetWindowText("�ŷ���");
				//PA��x6��=1;01 05 00 06 00 00 2D CB
				UCHAR x6[8]={0x01,0x05,0x00,0x06,0x00,0x00,0x2D,0xCB};
				pParlPro->SendCommand(x6,8);
				Sleep(30);
				//pParlPro->flag_motor_dirction_option=CCWL;
				////PA��4A��=1;01 06 00 4A 00 01 69 DC
				//UCHAR x4A[8]={0x01,0x06,0x00,0x4A,0x00,0x01,0x69,0xDC};
				//pParlPro->SendCommand(x4A,8);
				//Sleep(30);
				//PA��x8_set_1��=1;01 05 00 08 FF 00 0D F8
				UCHAR x8_set_1[8]={0x01,0x05,0x00,0x08,0xFF,0x00,0x0D,0xF8};
				pParlPro->SendCommand(x8_set_1,8);
				Sleep(30);
				//PA��x8_set_0��=1;01 05 00 08 00 00 4C 08
				UCHAR x8_set_0[8]={0x01,0x05,0x00,0x08,0x00,0x00,0x4C,0x08};
				pParlPro->SendCommand(x8_set_0,8);
				Sleep(30);
				return 0;
			} 
			outtemp="";
		} 
		if (pParlPro->flag_motor_dirction_option==CCWL)
		{
			//step2: ��ѯCCWL��λ
			//��ȡ��IN7 CWL 135��01 01 00 87 00 01 4D E3��
			UCHAR d135[8]={0x01,0x01,0x00,0x87,0x00,0x01,0x4D,0xE3};
			pParlPro->SendCommand(d135,8);
			Sleep(10);
			do 
			{
				switch (pParlPro->m_MsComm.get_CommEvent())
				{
				case 2://���ݽ����¼�
					Sleep(10);//��֤������������
					variant_inp=pParlPro->m_MsComm.get_Input(); //����������Ϣ
					safearray_inp=variant_inp; ///����ת��
					len=safearray_inp.GetOneDimSize(); //�õ���Ч�����ݳ���
					for(k=0;k<len;k++)
						safearray_inp.GetElement(&k,rxdata+k);
					for(k=0;k<len;k++) //������ת��ΪCString�ͱ���
					{
						UCHAR bt=*(UCHAR*)(rxdata+k);//�ַ���
						strtemp.Format("%.2X",bt); //���ַ�������ʱ����strtemp���
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
				//CCWL����
				/*
				����RADIOΪCCWL��λ
				�´����������ŷ�ֹͣ��ָ��
				�´�������IN8=1
				�´�������IN8=0
				*/
				pParlPro->m_RADIO_CCWL_Limit.SetCheck(TRUE);
				pParlPro->m_RADIO_CWL_Limit.SetCheck(FALSE);
				pParlPro->m_RADIO_CWL.SetCheck(FALSE);
				pParlPro->m_RADIO_CCWL.SetCheck(FALSE);
				pParlPro->m_RADIO_STOP.SetCheck(FALSE);

				pParlPro->m_BUTTON_GoOrStop.SetWindowText("�ŷ���");
				pParlPro->flag_motor_dirction_option=NONE_OPTION;
				pParlPro->flag_thread_continue=FALSE;
				//PA��x6��=1;01 05 00 06 00 00 2D CB
				UCHAR x6[8]={0x01,0x05,0x00,0x06,0x00,0x00,0x2D,0xCB};
				pParlPro->SendCommand(x6,8);
				Sleep(30);
				//pParlPro->flag_motor_dirction_option=CWL;
				////PA��4A��=0; 01 06 00 4A 00 00 A8 1C
				//UCHAR x4A[8]={0x01,0x06,0x00,0x4A,0x00,0x00,0xA8,0x1C};
				//pParlPro->SendCommand(x4A,8);
				//Sleep(30);
				//PA��x8_set_1��=1;01 05 00 08 FF 00 0D F8
				UCHAR x8_set_1[8]={0x01,0x05,0x00,0x08,0xFF,0x00,0x0D,0xF8};
				pParlPro->SendCommand(x8_set_1,8);
				Sleep(30);
				//PA��x8_set_0��=1;01 05 00 08 00 00 4C 08
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
	byte rxdata[512]; //����BYTE����
	do 
	{
		switch (pParlPro->m_MsComm.get_CommEvent())
		{
		case 2://���ݽ����¼�
			Sleep(10);//��֤������������
			variant_inp=pParlPro->m_MsComm.get_Input(); //����������Ϣ
			safearray_inp=variant_inp; ///����ת��
			len=safearray_inp.GetOneDimSize(); //�õ���Ч�����ݳ���
			for(k=0;k<len;k++)
				safearray_inp.GetElement(&k,rxdata+k);
			for(k=0;k<len;k++) //������ת��ΪCString�ͱ���
			{
				UCHAR bt=*(UCHAR*)(rxdata+k);//�ַ���
				strtemp.Format("%.2X",bt); //���ַ�������ʱ����strtemp���
				if(k<len-1)
					strtemp+=" ";
				outtemp+=strtemp; //������ձ༭���Ӧ�ַ���
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
	//pParlPro->UpdateData(FALSE);�����߳�ʱ��Ҫʹ�ø÷�������
	pParlPro->m_edit_MsgRec.SetWindowText(outtemp);
	outtemp="";
	return 0;
}

void Cbutton_testDlg::OnBnClickedButtonGoorstop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (flag_thread_continue)
	{
		flag_thread_continue=false;
		flag_motor_dirction_option=NONE_OPTION;
		GetDlgItem(IDC_BUTTON_GoOrStop)->SetWindowText("�ŷ���");
		//�ŷ�ʹ�ܣ�x6����01 05 00 06 00 00 2D CB��
		UCHAR x6[8]={0x01,0x05,0x00,0x06,0x00,0x00,0x2D,0xCB};
		SendCommand(x6,8);
	} 
	else
	{
		if (flag_motor_dirction_option==NONE_OPTION)
		{
			AfxMessageBox("����ѡ����ת������");
			return;
		}
		flag_thread_continue=true;
		GetDlgItem(IDC_BUTTON_GoOrStop)->SetWindowText("�ŷ���");
		//�ŷ�ʹ�ܣ�x6����01 05 00 06 FF 00 6C 3B��
		UCHAR x6[8]={0x01,0x05,0x00,0x06,0xFF,0x00,0x6C,0x3B};
		SendCommand(x6,8);
		m_MsComm.get_Input(); //Ԥ���������������������
		//������ѯ�߳�
		CWinThread* pointerThread;
		pointerThread=AfxBeginThread(InquiryThread,this); 
	}
}

void Cbutton_testDlg::OnBnClickedCheckCommandInputMode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�������Զ���ѡ��ѡ��Ȼ����ִ������Ĵ���
	if (m_RADIO_STOP.GetCheck()==BST_UNCHECKED)
	{
		m_CHECK_Input_Mode.SetCheck(BST_UNCHECKED);
		AfxMessageBox("ֹͣ�ŷ���ſ���ʹ�ø�ģʽ��");
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
		AfxMessageBox("�˳���ģʽǰ����ȷ���������ֹͣ״̬����û����λ����������");
		break;
	case BST_UNCHECKED:
		CString mode_msg="�˳���ģʽǰ����ȷ���������ֹͣ״̬����û����λ�����������Ƿ�ȷ���˳���";
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT_Send)->SetWindowText("");
}
