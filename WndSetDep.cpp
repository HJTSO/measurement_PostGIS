/*********************************************************************/
/*                           GeoMation                               */
/*                                                                   */
/*     ALL RIGHTS RESERVED.                                          */
/*     COPYRIGHT (C) 2010,  HITACHI SOFTWARE ENGINEERING CO.,LTD.    */
/*     Hitachi Software Engineering Co., Ltd. Confidential           */
/*********************************************************************/

/**
 * @file    WndSetDep.cpp
 * @brief   �{���I�����
 *
 * $Author: tomohiro.kurokawa.ev@ps.hitachi-solutions.com $
 * $Date:: 2010-10-21 15:27:17#$
 */

#include "WndSetDep.h"
#include "MessageBox.h"
#include "PsdUtil.hpp"
#include "CmdLogoff.h"

#include "wndforcelock.h"

#define HEADQUATER_MAX       53     //47�s���{��+��

#define HEADQUATER_DEFAULT   12  //����

#define ADSINFO_FILE         "start.dat"

#define ADSINFO_MAX_STRINGS  64

#include "../../gmkeitai/psdbg.bid"
#include "../../gmkeitai/psd.bid"
#include "./bg/psdbgevttype.hpp"

#define PSDLAUNCH_DEBUG_IP_FLAG "0"
#include "../../gmkeitai/psdlaunch.brh"
#include <tool/stringutil.hpp>


/**
 * @brief WndSetDep �R���X�g���N�^
 */
WndSetDep::WndSetDep()
{

	const char* msg = StringUtil::Load(FILE_GMK_BAR, IDS_STRING_PSDLAUNCH_DEBUG_IP);
	
		    headq_data da[HEADQUATER_MAX] = 
	    {
			{"�k�C���x�@�{��","https://172.26.120.1","EPSG:32654"},
			{"���ٕ��ʖ{��","https://172.26.120.4","EPSG:32654"},
			{"������ʖ{��","https://172.26.120.9","EPSG:32654"},
			{"���H���ʖ{��","https://172.26.120.12","EPSG:32655"},
			{"�k�����ʖ{��","https://172.26.120.17","EPSG:32654"},
			{"�X���x�@�{��","https://172.26.120.20","EPSG:32654"},
			{"��茧�x�@�{��","https://172.26.120.25","EPSG:32654"},
			{"�{�錧�x�@�{��","https://172.26.120.28","EPSG:32654"},
			{"�H�c���x�@�{��","https://172.26.120.33","EPSG:32654"},
			{"�R�`���x�@�{��","https://172.26.120.36","EPSG:32654"},
			{"�������x�@�{��","https://172.26.120.41","EPSG:32654"},
			{"�x�����{��","https://172.26.120.49","EPSG:32654"},
			{"�x���������{��","https://172.26.120.52","EPSG:32654"},
			{"��錧�x�@�{��","https://172.26.120.57","EPSG:32654"},
			{"�Ȗ،��x�@�{��","https://172.26.120.60","EPSG:32654"},
			{"�Q�n���x�@�{��","https://172.26.120.65","EPSG:32654"},
			{"��ʌ��x�@�{��","https://172.26.120.68","EPSG:32654"},
			{"��t���x�@�{��","https://172.26.120.73","EPSG:32654"},
			{"�_�ސ쌧�x�@�{��","https://172.26.120.76","EPSG:32654"},
			{"�V�����x�@�{��","https://172.26.120.81","EPSG:32654"},
			{"�R�����x�@�{��","https://172.26.120.84","EPSG:32654"},
			{"���쌧�x�@�{��","https://172.26.120.89","EPSG:32654"},
			{"�É����x�@�{��","https://172.26.120.92","EPSG:32654"},
			{"�x�R���x�@�{��","https://172.26.120.129","EPSG:32653"},
			{"�ΐ쌧�x�@�{��","https://172.26.120.132","EPSG:32653"},
			{"���䌧�x�@�{��","https://172.26.120.137","EPSG:32653"},
			{"�򕌌��x�@�{��","https://172.26.120.140","EPSG:32653"},
			{"���m���x�@�{��","https://172.26.120.145","EPSG:32653"},
			{"�O�d���x�@�{��","https://172.26.120.148","EPSG:32653"},
			{"���ꌧ�x�@�{��","https://172.26.120.153","EPSG:32653"},
			{"���s�{�x�@�{��","https://172.26.120.156","EPSG:32653"},
			{"���{�x�@�{��","https://172.26.120.161","EPSG:32653"},
			{"���Ɍ��x�@�{��","https://172.26.120.164","EPSG:32653"},
			{"�ޗǌ��x�@�{��","https://172.26.120.169","EPSG:32653"},
			{"�a�̎R���x�@�{��","https://172.26.120.172","EPSG:32653"},
			{"���挧�x�@�{��","https://172.26.120.177","EPSG:32653"},
			{"�������x�@�{��","https://172.26.120.180","EPSG:32653"},
			{"���R���x�@�{��","https://172.26.120.185","EPSG:32653"},
			{"�L�����x�@�{��","https://172.26.120.188","EPSG:32653"},
			{"�R�����x�@�{��","https://172.26.120.193","EPSG:32652"},
			{"�������x�@�{��","https://172.26.120.196","EPSG:32653"},
			{"���쌧�x�@�{��","https://172.26.120.201","EPSG:32653"},
			{"���Q���x�@�{��","https://172.26.120.204","EPSG:32653"},
			{"���m���x�@�{��","https://172.26.120.209","EPSG:32653"},
			{"�������x�@�{��","https://172.26.120.212","EPSG:32652"},
			{"���ꌧ�x�@�{��","https://172.26.120.217","EPSG:32652"},
			{"���茧�x�@�{��","https://172.26.120.220","EPSG:32652"},
			{"�F�{���x�@�{��","https://172.26.120.225","EPSG:32652"},
			{"�啪���x�@�{��","https://172.26.120.228","EPSG:32652"},
			{"�{�茧�x�@�{��","https://172.26.120.233","EPSG:32652"},
			{"���������x�@�{��","https://172.26.120.236","EPSG:32652"},
			{"���ꌧ�x�@�{��","https://172.26.120.241","EPSG:32652"},
			{"�x�@���ʐM�w�Z","https://172.26.121.1","EPSG:32654"}	
		};


	if(0 == STRCMP(msg, PSDLAUNCH_DEBUG_IP_FLAG)){

		//�{�ԗp�T�[�oIP�A�h���X

			for(int num = 0; num <HEADQUATER_MAX; num ++ ){
				headqNamelist.push_back(da[num].headq_name);
				headqIplist.push_back(da[num].headq_ip);
				headqSRSlist.push_back(da[num].headq_srs);
		    }

	}else{

		//�J���p�T�[�oIP�A�h���X
	    headq_data da2[6] = 
	    {
			{"gm-police","https://gm-police.hitachi-solutions.co.jp","EPSG:32654"},
			{"gm-ibp","https://gm-ibp.hitachi-solutions.co.jp","EPSG:32654"},
			{"gm-psdap","https://gm-psdap.hitachi-solutions.co.jp","EPSG:32654"},
			{"gm-psdapdb1","https://gm-psdapdb1.hitachi-solutions.co.jp","EPSG:32654"},
			{"���l�r�s","https://172.26.112.236","EPSG:32654"},
			{"133.108.100.224","http://133.108.100.224","EPSG:32654"}
		};

		for(int num = 0; num < 6; num ++ ){
			headqNamelist.push_back(da2[num].headq_name);
			headqIplist.push_back(da2[num].headq_ip);
			headqSRSlist.push_back(da2[num].headq_srs);
		}
		for(int num = 6; num <HEADQUATER_MAX; num ++ ){
			headqNamelist.push_back(da[num].headq_name);
			headqIplist.push_back(da[num].headq_ip);
			headqSRSlist.push_back(da[num].headq_srs);
		}
	}
}

/**
 * @brief WndSetDep �f�X�g���N�^
 */
WndSetDep::~WndSetDep()
{
	//�f�X�g���N�^
}

/**
 * @brief �R���g���[���̏����� WndBase::InitializeControl()�I�[�o�[���C�h
 */

bool WndSetDep::InitializeControl()
{
	LOG_DEBUG("WndSetDep::InitializeControl() %s", "");

	// title
	SetWindowName("�{���I��");

	// ���X�g�{�b�N�X�I��ۑ����p�t���O�ݒ�(LOAD)
	WndList::SetSaveSelectItem(true);

    listItems_.clear();


	listbox_ = GetListBox();
	if (0 == listbox_)
		return false;

    if(FALSE ==  IsExistFile( (const char*)ADSINFO_FILE )){

		// (�^�C�g�� + ��s��2�s�����J�E���g)
		int row = GetTitleRowSize() + 1;

		SetPageLabel(txtNumColumns/2 + 2 ,row);

		row ++;

		// ���X�g�ő�\�������擾
		InitListItemSize();
	    row += SetListPosition(1, row, txtNumColumns - 2);
		SetNumKeyExeType(KEY_NUMKEY_DIRECT);
		SetViewHeight(row);

	    // ListBox�쐬
	    SetPagingList();

	    // �����ListBox��ԂŃy�[�W���̐ݒ�
		InitPageListEx(GetSelectIndexParam());

		// �I���󋵕���
    	 WndList::LoadSelectItem();

	}

	return true;

}


/**
 * @brief �_�C�A���O�̕\������
 */

void WndSetDep::ShowComfarmDialog()
{
	LOG_DEBUG("WndSetDep::ShowComfarmDialog() %s", "");

    char Name[256];
    SNPRINTF(Name, sizeof(Name), "%s���I������܂����B�X�����ł����H",
                                 headqNamelist.at(current_headq_index).c_str());

	LOG_DEBUG("WndSetDep::ShowComfarmDialog() headq     name = %s index = %d",
	                                           headqNamelist.at(current_headq_index).c_str(),
	                                           current_headq_index);

    MessageBox::ShowConf( Name, 8, "�͂�","������");
    viewing_dialog_flag = TRUE;


}

/**
 * @brief FG�ABG�A�v���N������
 */
void WndSetDep::StartApple( const char* args )
{
	LOG_DEBUG("WndSetDep::StartApple().args = %s", "");
	LOG_DEBUG("%s", args);

	// PlaseWait��ʕ`��
	SrvWndManager* swm = GmkUtil::GetWndManager();
    if(NULL == swm){
		LOG_ERROR("WndSetDep::StartApple(). Can't get SrvWndManager");
	}else{
		
		swm->Show(WNDID_FORCE_LOCK);
		// ���X�g��ʃN���[�Y
		WndPsdPageList::OnCloseWindow();
	}

	//BG�A�v���ɏI���ʒm
	if(ISHELL_PostEvent(GETSHELL(), AEECLSID_PSDBG, EVT_PSDBG_FG_STOP, 0, 0)){
		LOG_DEBUG("WndSetDep::StartApple(). Post EVT_PSDBG_FG_STOP to AEECLSID_PSDBG app");
	}else{
		LOG_ERROR("WndSetDep::StartApple(). error. Can't post EVT_PSDBG_FG_STOP ");
	}

	//�A�v���P�[�V���������쐬
	char Buf[ADSINFO_MAX_STRINGS] = {};
    int read_size = sizeof(args);

	if((read_size <= 0) || (read_size > ADSINFO_MAX_STRINGS)){
		LOG_ERROR("WndSetDep::StartApple(). Can't set parameter");
	}else{
		STRNCPY(Buf, args , sizeof(Buf));
		LOG_DEBUG("WndSetDep::StartApple().ADS info =");
		LOG_DEBUG("%s", &Buf);
	}

	//���A�v���P�[�V�����N��

	//Bluetooth�ڑ��A�v��
	#if 1
	int bg_ret = ECLASSNOTSUPPORT;

	bg_ret = ISHELL_StartBackgroundApplet(GETSHELL(),AEECLSID_PSDBG,(const char*)&Buf);
	if(SUCCESS == bg_ret || EALREADY == bg_ret){
		LOG_DEBUG("ISHELL_StartBackgroundApplet(AEECLSID_PSDBG) bg_ret = %d");
	}else{
		LOG_ERROR("ISHELL_StartBackgroundApplet(AEECLSID_PSDBG) error.bg_ret = %d");
	}
	#endif

	//�f�[�^�[���A�v���N��
	#if 1
	int fg_ret = ECLASSNOTSUPPORT;
	
	fg_ret = ISHELL_StartAppletArgs(GETSHELL(),AEECLSID_PSD,(const char*)&Buf);
	if(SUCCESS == fg_ret || EALREADY == fg_ret){
		LOG_DEBUG("ISHELL_StartAppletArgs(AEECLSID_GMKEITAI) fg_ret = %d");
	}else{
		LOG_ERROR("ISHELL_StartAppletArgs(AEECLSID_GMKEITAI) error.fg_ret = %d");
	}
	#endif
}

/**
 * @brief �E�C���h�E�̊J�n����
 */
boolean WndSetDep::OnBeginWindow()
{
	LOG_DEBUG("WndSetDep::OnBeginWindow()");

	//�t�@�C���L���m�F���s���B
	if( TRUE ==  IsExistFile( (const char*)ADSINFO_FILE ) ){
		StartApple("");						//�A�v���P�[�V�����N��
	}
	else
	{
		WndPsdPageList::OnBeginWindow();	//�{���I����ʋN��
	}
	return TRUE;
}

/**
 * @brief �E�B���h�E���W���[���C�x���g
 */
void WndSetDep::OnResumeWindow()
{
	LOG_DEBUG("WndSetDep::OnResumeWindow()");

	if( viewing_dialog_flag ){
		// �m�F�_�C�A���O�`��
		ShowComfarmDialog();
	}

}

/**
 * @brief �E�B���h�E�T�X�y���h�C�x���g
 */
void WndSetDep::OnSuspendWindow(){
	LOG_DEBUG("WndSetDep::OnSuspendWindow()");
}

/**
 * @brief �E�C���h�E�̊J�n����
 */
void WndSetDep::OnOpenWindow(){
	LOG_DEBUG("WndSetDep::OnOpenWindow()");
}

/**
 * @brief �E�C���h�E�̏I������
 */
void WndSetDep::OnCloseWindow(){
	LOG_DEBUG("WndSetDep::OnCloseWindow()");
}


/**
 * @brief ����N������t�@�C���ǂݏo������
 */
int32 WndSetDep::ReadFile(char* pPath, unsigned char* pBuf, int iBufSize)
{
	LOG_DEBUG("WndSetDep::ReadFile() Name = %s pBuf = %s",pPath,pBuf);

	if (NULL == pPath || NULL == pBuf) {
		return 0;
	}

    int32 ret = 0;
    IFileMgr* fmgr = NULL;
    if (SUCCESS != ISHELL_CreateInstance(GETSHELL(), AEECLSID_FILEMGR, (void **)&fmgr)) 
    {
        LOG_ERROR("AEECLSID_FILEMGR was failed");
        fmgr = 0;
    }
    else
    {
        IFile* pF = IFILEMGR_OpenFile(fmgr, pPath, _OFM_READ);
        if (pF) {
            int read_size = IFILE_Read(pF, pBuf, iBufSize);
            
            ret = read_size;
            IFILE_Release(pF);
            pF = NULL;
        }
        IFILEMGR_Release(fmgr);
        fmgr = NULL;
    }

	LOG_DEBUG("WndSetDep::ReadFile() ret(%d)",ret);

    return ret;
}

/**
 * @brief ����N������t�@�C�������ݏ���
 */
uint32 WndSetDep::WriteFile(char* pPath, unsigned char* pBuf, int iBufSize)
{
	LOG_DEBUG("WndSetDep::WriteFile() Name = %s pBuf = %s",pPath,pBuf);

	if (NULL == pPath || NULL == pBuf) {
		return 0;
	}

    int32 ret = 0;
    IFileMgr* fmgr = NULL;
 
    if (SUCCESS != ISHELL_CreateInstance(GETSHELL(), AEECLSID_FILEMGR, (void **)&fmgr)) 
    {
        LOG_ERROR("AEECLSID_FILEMGR was failed");
        fmgr = 0;
    }
    else
    {
        IFile* pF = IFILEMGR_OpenFile(fmgr, pPath, _OFM_READWRITE );

        if( NULL == pF ){
            pF = IFILEMGR_OpenFile(fmgr, pPath,_OFM_CREATE  );
        }

        if (pF) {
            ret = IFILE_Write(pF, pBuf, iBufSize);
            IFILE_Release(pF);
            pF = NULL;
        }

        IFILEMGR_Release(fmgr);
        fmgr = NULL;
    }

	LOG_DEBUG("WndSetDep::WriteFile() ret(%d)",ret);

    return ret;
}

/**
 * @brief ����N������t�@�C���L�����菈��
 */
int32 WndSetDep::IsExistFile(const char* pPath)
{
	LOG_DEBUG("WndSetDep::IsExistFile() Name = %s",pPath);

	if (NULL == pPath ) {
		return 0;
	}

    int32 ret = FALSE;
    IFileMgr* fmgr = NULL;
 
    if (SUCCESS != ISHELL_CreateInstance(GETSHELL(), AEECLSID_FILEMGR, (void **)&fmgr)) 
    {
        LOG_ERROR("AEECLSID_FILEMGR was failed");
        fmgr = 0;
    }
    else
    {
        IFile* pF = IFILEMGR_OpenFile(fmgr, pPath, _OFM_READWRITE );

        if (pF) {
            ret = TRUE;
            IFILE_Release(pF);
            pF = NULL;
        }

        IFILEMGR_Release(fmgr);
        fmgr = NULL;
    }

	LOG_DEBUG("WndSetDep::IsExistFile() ret(%d)",ret);

    return ret;
}

/**
 * @brief �R�}���h���������� WndBase::HandleCommand()�I�[�o�[���C�h
 */
void WndSetDep::HandleCommand(const char* cmdStr, uint16 wParam)
{

	LOG_DEBUG("WndSetDep::HandleCommand()");

    // �m�F�_�C�A���O�{�^���������̃_�C�A���O�Łu�͂��v�I��
    if(0 == STRCMP(CMD_PSD_DIALOG_YES, cmdStr))
    {

		LOG_DEBUG("WndSetDep::HandleCommand(CMD_PSD_DIALOG_YES)");

		//�_�C�A���O��ʕ\�����t���O�폜
		viewing_dialog_flag = FALSE;
		

		// ADS��񕶎���쐬
		// "IP�A�h���X" + "SRS���"
		cstring name = headqIplist.at(current_headq_index)
						+ "," + headqSRSlist.at(current_headq_index);
		
		LOG_DEBUG("�{���� = %s",headqNamelist.at(current_headq_index).c_str());
		LOG_DEBUG("IP�A�h���X + SRS��� = %s",name.c_str());

		//���L�t�H���_�t�@�C���ɏ���ۑ�
		uint32 write_size;
		cstring empty = "";
		write_size = WriteFile( (char*)ADSINFO_FILE,(unsigned char*)empty.c_str(), empty.Length());

		//�������ݐ���/�s�����ɂ�����炸�A�A�v���P�[�V�����N��
		StartApple(name.c_str());

    }
        // �m�F�_�C�A���O�{�^���������̃_�C�A���O�Łu�������v�I��
    if(0 == STRCMP(CMD_PSD_DIALOG_NO, cmdStr))
    {
		LOG_DEBUG("WndSetDep::HandleCommand(CMD_PSD_DIALOG_NO)");

		//�_�C�A���O��ʕ\�����t���O�폜
		viewing_dialog_flag = FALSE;

	}

	WndPsdPageList::HandleCommand(cmdStr, wParam);
}

/**
 * @brief �C�x���g���������� WndBase::HandleEvent()�I�[�o�[���C�h
 */
boolean WndSetDep::HandleEvent(AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    UNUSED_ALWAYS(wParam);
    UNUSED_ALWAYS(dwParam);
    UNUSED_ALWAYS(eCode);
    return FALSE;
}

/**
 * @brief ���X�g�R���g���[���Ƀf�[�^��ݒ�
 */
bool WndSetDep::SetPagingList()
{
	LOG_DEBUG("WndSetDep::SetPagingList()");

    int ListSize = headqNamelist.size();

    int i = 0;
    int j = 1;
    int OutPutSize = GetListItemSize();
    for(; i < ListSize ; i++, j++){
        if( OutPutSize < j) j = 1;

        char Name[256];
        SNPRINTF(Name, sizeof(Name), "%d.%s",j,headqNamelist.at(i).c_str());
        AddPageingItem(Name , "dummy");
    }
    return true;
}

/**
 * @brief ���X�g�̍��ڂ�I�������Ƃ��ɌĂ΂��R�}���h
 */
bool WndSetDep::SelectItemCommand(int index)
{
	LOG_DEBUG("WndSetDep::SelectItemCommand()");

	if ( FALSE == viewing_dialog_flag ){
		// �|�b�v�A�b�v���܂��\������Ă��Ȃ��ꍇ
		
	    // ���[�U���I�������{���ԍ����擾
		int OutPutSize = GetListItemSize();
		int PageNum = GetPageNum();

		current_headq_index = 0;
		current_headq_index = OutPutSize * (PageNum - 1) + index;

		LOG_DEBUG("WndSetDep::SelectItemCommand() headq_index = %d",current_headq_index);

		if(current_headq_index < 0 || current_headq_index >= HEADQUATER_MAX )
		{
			//�ُ�l�̏ꍇ��"����"�ɃZ�b�g���Ȃ���
			current_headq_index = HEADQUATER_DEFAULT;
			LOG_ERROR("WndSetDep::SelectItemCommand() change headq_index = %d",current_headq_index);
		}

		// �m�F�_�C�A���O�`��
		ShowComfarmDialog();
	}
	else{
		// �|�b�v�A�b�v�����łɕ\������Ă���ꍇ
		LOG_DEBUG("WndSetDep::SelectItemCommand() Already show popup.");
	}
    return true;
}

bool WndSetDep::NextMoveWnd()
{
    return true;
}

/**
 * @brief �\�t�g�L�[������
 */
bool WndSetDep::InitializeSoftkey()
{
	if (!SetSoftkeyCenter("C_KETTEI","CmdNonAction","")) return false;

	return true;
}

