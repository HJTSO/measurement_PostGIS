/*********************************************************************/
/*                       Keitai/Police                               */
/*                                                                   */
/*     ALL RIGHTS RESERVED.                                          */
/*     COPYRIGHT (C) 2010,  HITACHI SOFTWARE ENGINEERING CO.,LTD.    */
/*     Hitachi Software Engineering Co., Ltd. Confidential           */
/*********************************************************************/

/**
 * @file	WndLogin.cpp
 * @brief	���O�C�����
 * 
 * PSD/�f�[�^�[���A�v���̃��O�C����ʃN���X�ł��B<BR>
 * 
 * 
 * $Author: kenji.kikuchi.jj@hitachi-solutions.com $
 * $Date:: 2011-07-21 16:24:53#$
 */

#include "WndLogin.h"

#include <AEESecurity.h>
#include <util/multitask.hxx>
#include <tool/stringutil.hpp>
#include <tool/juliandate.hpp>
#include <HSFImage.hpp>
#include "PsdUtil.hpp"
#include "WndSetDep.h"
#include "WndSetJob.h"
#include "WndSetJobDetail.h"
#include "WndSetActCond.h"
#include "WndPairing.h"
#include "PsdWorkMstData.h"
#include "wndchgpwd.h"
#include "MessageBox.h"
#include "LoginManager.h"
#include "ServerCommandLogin.hpp"
#include "ServerCommandSendStatusInit.hpp"
#include "ServerCommandSendStatus.hpp"
#include "ServerCommandGetMaster.hpp"  
#include "psdcodedefine.hpp"
#include "PositioningHistory.hpp"
#include "PsdHttpUtil.h"
#include "PsdSendRecoveryInfo.h"
#include "appverchk.h"

#define PASSWORD_MINLEN 8

#define CMD_PASSWORD_CHANGE     CMD_PSD_DIALOG_PREFIX "_PS_CHG"      //!< �p�X���[�h�ύX��ʑJ��
#define CMD_NEXT_WINDOW         CMD_PSD_DIALOG_PREFIX "_NEXT_WND"    //!< ����ʑJ��

WndLogin::WndLogin()
 : WndPsdBase(), inputmode_(TMJ_HANNUM), login_result_(NULL), foucusIndex_(1), nextwnd_task_(NULL)
 , m_modeUserId(TMJ_HANNUM), m_modePassword(TMJ_HANNUM), ResSuujiImg_(0), ResEijiImg_(0)
 , m_AppVerMng(NULL)
{
    txtMemberId_    = NULL;
    txtPassword_    = NULL;
    stsVersion_     = NULL;
    imgLabelUserID_ = NULL;
    imgLabelPass_   = NULL;
    this->SetWndLock(false);    // �X�N���[�����b�N�s��
    initailed_=false;
	SendStatusInitFinish_=false;
}

WndLogin::~WndLogin()
{
    SAFE_DELETE(nextwnd_task_);
    SAFE_DELETE(m_AppVerMng);
}


bool WndLogin::InitializeControl()
{
    // �㉺���E�̃L�[�����ɂ��t�H�[�J�X�J�ڂ����ׂĕ����ď�������ݒ�
    SetHandlePartsStrategy(HANDLE_PARTS_LEGACY);

    // �\�t�g�L�[�E�{�^���̉摜�擾
    // ����
    if(!ResSuujiImg_.get())
        ResSuujiImg_ = smart_ptr<HSFImage>(HSFImage::LoadResImage(FILE_GMK_BAR, IDI_IMG_SOFTKEY_R_SUUJI));
    // �p��
    if(!ResEijiImg_.get())
        ResEijiImg_  = smart_ptr<HSFImage>(HSFImage::LoadResImage(FILE_GMK_BAR, IDI_IMG_SOFTKEY_R_EIJI));

    SrvWndManager* swm = GmkUtil::GetWndManager();
    if (NULL == swm) {
        return false;
    }
    else
    {
        swm->ClearHistory();
        swm->AddHistory(WNDID_LOGIN);
    }

	int row = GetTitleRowSize() + 1;

	// ���[�UID���x��

    Property& layout = GET_PROPERTY(PropertyManager::LAYOUT);
	row = row + 2;

	// ���[�UIID
	txtMemberId_ = GetDirectText();
    if (NULL == txtMemberId_) {
		return false;
	}

    Property& session = GET_PROPERTY(PropertyManager::SESSION);

	txtMemberId_->SetText(session.get("%s.USERID", SESSIONID_0001010 ));
	txtMemberId_->SetCuiLocation(1, row);
	txtMemberId_->SetCuiSize(txtNumColumns - 2, 1);
    txtMemberId_->SetInputMode(TMJ_HANNUM);
    txtMemberId_->SetModeChange(true);
    txtMemberId_->SetAutoMove(true);
    txtMemberId_->SetFocusInNotifyListner(new DirectTextFocusInNotify<WndLogin>(this, &WndLogin::TextFocusInNotify));
    if( session.contains( SESSIONID_0001010 ".USERID.POS" ) ){
        txtMemberId_->SetCursorPos((int16)ATOI(session.get("%s.USERID.POS", SESSIONID_0001010 )));
    }

    const char* pwdMaxSize = layout.get("%s.textCtl.id.maxSize", GetFunctionId());
	txtMemberId_->SetMaxSize((uint16)ATOI(pwdMaxSize));

	row = row + 2;
	row = row + 1;

	// �p�X���[�h���͗�
	txtPassword_ = GetDirectText();
    if (NULL == txtPassword_) {
		return false;
	}

	txtPassword_->SetText(session.get("%s.PASSWORD", SESSIONID_0001010));
	txtPassword_->SetCuiLocation(1, row);
	txtPassword_->SetCuiSize(txtNumColumns - 2, 1);
	txtPassword_->SetProperties(TP_PASSWORD);
    txtPassword_->SetInputMode(TMJ_HANNUM);
    txtPassword_->SetModeChange(true);
    txtPassword_->SetFocusInNotifyListner(new DirectTextFocusInNotify<WndLogin>(this, &WndLogin::TextFocusInNotify));
    if( session.contains(SESSIONID_0001010 ".PASSWORD.POS" ) ){
        txtPassword_->SetCursorPos((int16)ATOI(session.get("%s.PASSWORD.POS", SESSIONID_0001010 )));
    }


	pwdMaxSize = PsdUtil::GetSysMsg("%s.textCtl.pwd.maxSize", GetFunctionId());
	txtPassword_->SetMaxSize((uint16)ATOI(pwdMaxSize));

	row = row + 3;

    AEERect rect;
	// ���O�C���{�^��
	btnLogin_ = GetPictButtonCtl();
    if (NULL == btnLogin_) {
		return false;
	}
    btnLogin_->SetResImage(FILE_GMK_BAR, IDI_IMG_LOGIN_N, IDI_IMG_LOGIN_S, IDI_IMG_LOGIN_N);
	btnLogin_->SetCuiLocation(1, row);
    SETAEERECT(&rect, 20, 240, 90, 30);
    btnLogin_->SetRect(&rect);
	btnLogin_->SetCmdStr(PsdUtil::GetSysMsg("%s.btn.cmd.0", GetFunctionId()));
	
	// �y�A�����O�{�^��
	btnPairing_ = GetPictButtonCtl();
    if (NULL == btnPairing_) {
		return false;
	}
    btnPairing_->SetResImage(FILE_GMK_BAR, IDI_IMG_PAIR_N, IDI_IMG_PAIR_S, IDI_IMG_PAIR_N);
	btnPairing_->SetCuiLocation(13, row);
    SETAEERECT(&rect, 130, 240, 90, 30);
    btnPairing_->SetRect(&rect);
	btnPairing_->SetCmdStr(PsdUtil::GetSysMsg("%s.btn.cmd.1", GetFunctionId()));

	// focus
	HSFFocusUtil::SetFocusOrder(txtMemberId_, txtPassword_, HSFFocusUtil::VERTICAL);
	HSFFocusUtil::SetFocusOrder(txtPassword_, btnLogin_, HSFFocusUtil::VERTICAL);
	HSFFocusUtil::SetFocusOrder(btnLogin_, btnPairing_, HSFFocusUtil::VERTICAL);
    HSFFocusUtil::SetFocusOrder(btnLogin_, btnPairing_, HSFFocusUtil::HORIZONTAL);
    btnPairing_->SetUpObject(txtPassword_);

    SetFoucusPos(foucusIndex_);
    //SetFirstFocusable(txtMemberId_);

	row = row + 6;

	AECHAR achar[32];
	ShellUtil::GetAppVersion(achar, sizeof(achar));
	char* verNum(StringUtil::ToChar(achar));
	cstring strVer("Ver:");
	strVer += verNum;

	stsVersion_ = GetStaticCtl();
	stsVersion_->SetText("", strVer.c_str());
    stsVersion_->SetCuiLocation(24 - (strVer.Length()+1), txtNumRows-1);
	stsVersion_->SetCuiSize(strVer.Length()+1, 1);

    setInputMode(inputmode_, false);

	delete [] verNum;
    verNum = NULL;

    SAFE_DELETE(m_AppVerMng);
    const char* version = layout.get("gyomuapp.version");
    m_AppVerMng = new AppVerChk(version);

    return true;
}

boolean WndLogin::OnBeginWindow(){

    Property& layout = GET_PROPERTY(PropertyManager::LAYOUT);

    if (!initailed_ && PsdUtil::IsSendStatusInit()) {
        // �A�v���o�[�W�����`�F�b�N
        if(m_AppVerMng != NULL)
        {
            cstring message_id = "";

            if(m_AppVerMng->IsOldMajorVer())
            {
                // ���W���[�o�[�W�������Â��ꍇ�̃|�b�v�A�b�v
                message_id = "%s.common.msg.majorver.error";
            }
            else if (m_AppVerMng->IsOldMinorVer())
            {
                // �}�C�i�[�o�[�W�������Â��ꍇ�̃|�b�v�A�b�v
                message_id = "%s.common.msg.minorver.error";
            }

            SrvWndManager* swm = GmkUtil::GetWndManager();
            if( (swm && swm->IsRefresh()) || message_id.isEmpty())
            {
                // GPS���ʑO�ɃT�X�y���h�����ꍇ��
                // �A�v���o�[�W�����`�F�b�N���s��Ȃ�(���{�ςȂ̂�)
                HandleCommand(CMD_FINISHED_VER_CHK, 0);
            }
            else
            {
                // �|�b�v�A�b�v�\��
                ShowInfoCommonMessage(message_id.c_str(), 8, CMD_FINISHED_VER_CHK);
            }
            return WndBase::OnBeginWindow();
        }
    } else {
        SetButtonEnable(true);


        // �������O�I�t��
        char keyBuf[64];
        ZERO_CLEAR(keyBuf);
        Property& session = GET_PROPERTY(PropertyManager::SESSION);
        int forceLogoutFlg = ATOI(session.get("COMMON.logoutflg"));
        session.clear();

        if (forceLogoutFlg == CodeLogoutFlg::DOUBLE_LOGIN)
        {
            // ��d���O�C���ɂ�郍�O�I�t�|�b�v�A�b�v
            SNPRINTF(keyBuf, sizeof(keyBuf), "%s.msg.duplicatelogin", GetFunctionId());
            ShowInfoMessage(layout.get(keyBuf), "cmdDoubleLogoff");
			session.put("COMMON.logoutflg",LOGOUTFLG_DOUBLE_LOGIN);
        }
        else if (forceLogoutFlg == CodeLogoutFlg::PAIRING_OFF)
        {
            // �y�A�����O�ؒf�ɂ�郍�O�I�t�|�b�v�A�b�v
            SNPRINTF(keyBuf, sizeof(keyBuf), "%s.msg.pairlingoff", GetFunctionId());
            ShowInfoMessage(layout.get(keyBuf), "cmdPairingLogoff");
			session.put("COMMON.logoutflg",LOGOUTFLG_PAIRING_OFF);
        }
        else if (forceLogoutFlg == CodeLogoutFlg::INITIALIZING)
        {
            // �C�j�V�����C�Y�v�����ɂ�郍�O�I�t�|�b�v�A�b�v
            SNPRINTF(keyBuf, sizeof(keyBuf), "%s.msg.initializing", GetFunctionId());
            ShowInfoMessage(layout.get(keyBuf), "cmdInitializing");
			session.put("COMMON.logoutflg",LOGOUTFLG_INITIALIZING);
        }
		else if (forceLogoutFlg == CodeLogoutFlg::USER_LOCK)
		{
            // ���[�U���b�N�ɂ�郍�O�I�t(�|�b�v�A�b�v�\���͂Ȃ�)
            session.put("COMMON.logoutflg",LOGOUTFLG_USER_LOCK);
		}
    }
    initailed_ = true;
    return WndBase::OnBeginWindow();
}

boolean WndLogin::OnDrawOtherAfter()
{
    drawInputMode();
    AEEImageInfo	imageInfo;
    imgLabelUserID_->GetInfo(&imageInfo);
    imgLabelUserID_->SetOffset(0,0);
    imgLabelUserID_->SetDrawSize(imageInfo.cx,imageInfo.cy);
    imgLabelUserID_->Draw(11, 70);

    imgLabelPass_->GetInfo(&imageInfo);
    imgLabelPass_->SetOffset(0,0);
    imgLabelPass_->SetDrawSize(imageInfo.cx,imageInfo.cy);
    imgLabelPass_->Draw(11, 139);

    return TRUE;
}

boolean WndLogin::HookKeyEvent(void *individualObject, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    UNUSED_ALWAYS(individualObject);
    UNUSED_ALWAYS(eCode);
    UNUSED_ALWAYS(dwParam);

    // �ʐM���̃L�[���얳����
    if (IsLoacalHttps() || PsdUtil::IsRequestGps()) {
        // �N���A�L�[�A�d���L�[�A�T�C�h�L�[�i���j�ȊO�𖳌�������
        if (    wParam != AVK_CLR     
             && wParam != AVK_POWER
             && wParam != AVK_OEM_SP1 )
        {
            return TRUE;
        }
    }

    return FALSE;
}

boolean WndLogin::EventKey(uint16 wParam, uint32 dwParam) {
    UNUSED_ALWAYS(dwParam);
    UNUSED_ALWAYS(wParam);

    // �{�^���̂ǂ��炩�Ƀt�H�[�J�X���������Ă���ꍇ�A�E�\�t�g�L�[���\��
    if( btnLogin_->IsSelect() || btnPairing_->IsSelect() ){
        GetMenuFunction()->SetHide(HSFMenuFunction::btnRight,true);
        GetMenuFunction()->Redraw(true);
    }

    return TRUE;
}


void WndLogin::HandleCommand(const char* cmdStr, uint16 wParam)
{
    Property& session   = GET_PROPERTY(PropertyManager::SESSION     );

    if (IsLoacalHttps() || PsdUtil::IsRequestGps()) {
        return;
    }

    if ( NULL == STRCMP("CmdLogin", cmdStr)) {
        if(m_AppVerMng != NULL)
        {
            if (m_AppVerMng->IsOldMajorVer())
            {
                // ���W���[�o�[�W�������Â��ꍇ�̃|�b�v�A�b�v
                ShowInfoCommonMessage("%s.common.msg.majorver.error");
            }
            else
            {
                session.put("%s.PUSH_PAIRLING_BUTTON", LOGIN_PUSH, SESSIONID_0001010);
                LoginButtonPush();
            }
        }
    } else if ( NULL == STRCMP("CmdPairing", cmdStr)) {
        if(m_AppVerMng != NULL)
        {
            if (m_AppVerMng->IsOldMajorVer())
            {
                // ���W���[�o�[�W�������Â��ꍇ�̃|�b�v�A�b�v
                ShowInfoCommonMessage("%s.common.msg.majorver.error");
            }
            else
            {
                session.put("%s.PUSH_PAIRLING_BUTTON", PAILING_PUSH, SESSIONID_0001010);
                LoginButtonPush();
            }
        }
    }

    if (NULL == STRCMP(CMD_PASSWORD_CHANGE, cmdStr) ) {
        // �p�X���[�h�ύX��ʂɑJ��
        PsdUtil::ShowWindow(WNDID_CHG_PWD);
    }

    if (NULL == STRCMP(CMD_NEXT_WINDOW, cmdStr) ) {
        // �p�X���[�h�L�������؂꒼�O�̃t���O�ݒ���폜
        Property& loginuser = GET_PROPERTY(PropertyManager::LOGINUSER);
        loginuser.remove("PASSWORD_OUT_NEAR");
        NextWindowCommand();
    }

    if (NULL == STRCMP("CmdAlphabet", cmdStr)) {
        setInputMode(TMJ_HANROMA, true);
        return;
    }

    if (NULL == STRCMP("CmdNumber", cmdStr)) {
        setInputMode(TMJ_HANNUM, true);
        return;
    }

    if (0 == STRCMP("cmdDoubleLogoff", cmdStr))
    {
        SetFirstFocusable(txtMemberId_);
    }

    if (0 == STRCMP("cmdUserLockLogoff", cmdStr))
    {
        SetFirstFocusable(txtMemberId_);
    }

    if (0 == STRCMP("cmdPairingLogoff", cmdStr))
    {
        SetFirstFocusable(txtMemberId_);
    }

    if (0 == STRCMP("cmdInitializing", cmdStr))
    {
        SetFirstFocusable(txtMemberId_);
    }

    if (0 == STRCMP(CMD_FINISHED_VER_CHK, cmdStr))
    {
        // ���񊈓��󋵒ʒm���ʊJ�n
        PsdUtil::GpsServiceStart();
        SetButtonEnable(false);
        // ServerCommandSendStatusInit���M�����܂ł̊Ԃɒ��M���ŃL�����Z�����ꂽ�ꍇ���đ��ΏۂƂ���
        SendStatusInitFinish_ = false;
        if ( !GET_GPSSERVICE()->MeasureRequest(new GPSRequestNotifyEvent<WndLogin>(this, &WndLogin::SendRequest_SendStatusInit) ) ) {
            // �ʐM���s���Ƀ_�C�A���O��\�����Ȃ�
            SetButtonEnable(true);
        }
        initailed_ = true;
    }

    if (NULL == STRCMP(DIALOG_OK_CMD_NAME, cmdStr)) {
        SetButtonEnable(true);
    }

    WndPsdBase::HandleCommand(cmdStr, wParam);
}

void WndLogin::LoginButtonPush() {
   switch (InputDataCheck()) {
        case CHK_NULL_ERROR:
        case CHK_SIZE_ERROR:
            break;
        case CHK_SUCCESS:
            {
                foucusIndex_ = this->GetFoucusPos();
                SetButtonEnable(false);

				//���O�A�E�g�t���O������
				Property& session = GET_PROPERTY(PropertyManager::SESSION);
				session.remove("COMMON.logoutflg");

                if (this->m_SendStatusInitCommand.get() == NULL)
                {
					CreateRequest_SendStatusInit(NULL);
                }

                if (m_SendStatusInitCommand->Send_Retry()) {
                    // ���O�C���F�؂����{�i���O�C���j
                    if (!SendRequest_ReSendStatusInit())
                    {
                        ShowInfoCommonMessage("%s.common.http.connection.error");
                        SetButtonEnable(true);
                    }
                } else {
                    
                    ;
                    // ���O�C���F�؂����{�i���O�C���j
                    if (!GET_GPSSERVICE()->MeasureRequest(new GPSRequestNotifyEvent<WndLogin>(this, &WndLogin::SendRequest_Login)))
                    {
                        ShowInfoCommonMessage("%s.common.http.connection.error");
                        SetButtonEnable(true);
                    }
                }

            break;
            }
        default:
            break;
    }
}


void WndLogin::NextWindowCommand() {

    GET_GPSSERVICE()->Start();

    PsdWorkMstData* mstData = PsdUtil::GetPsdWorkMstData();
    if (IsPairingMode()) {
        PsdUtil::ShowWindow(WNDID_PAIRING);
    } else  {
        // ����ʂ̃��X�g�������擾���A�P���̏ꍇ�͎���ʂɃX�L�b�v
        int Kind = mstData->CheckOneDataList();
        switch(Kind)
        {
        case PsdWorkMstData::EN_LIST_DIVISION:
            PsdUtil::ShowWindow(WNDID_SET_DEP);
            break;
        case PsdWorkMstData::EN_LIST_WORKTYPE:
            PsdUtil::ShowWindow(WNDID_SET_JOB);
            break;
        case PsdWorkMstData::EN_LIST_WORKDETAIL:
            PsdUtil::ShowWindow(WNDID_SET_JOB_DETAIL);
            break;
        case PsdWorkMstData::EN_LIST_WORKPTN:
        case PsdWorkMstData::EN_LIST_TRANSPORT:
        case PsdWorkMstData::EN_LIST_MAX:
            // �Ƒԓ��͉�ʈȍ~��1���f�[�^�ł��A�X�L�b�v�͋Ƒԓ��͉�ʂ܂�
            PsdUtil::ShowWindow(WNDID_SET_ACT_COND);
            break;
        default:
            ASSERT(false);
            break;
        }
    }
}


void WndLogin::setInputMode(InputMode mode, bool bUpdate)
{
    if( btnLogin_->IsSelect() || btnPairing_->IsSelect() ){
        GetMenuFunction()->SetHide(HSFMenuFunction::btnRight,true);
    }
    else{
        GetMenuFunction()->SetHide(HSFMenuFunction::btnRight,false);
        inputmode_ = mode;
       if (mode == TMJ_HANNUM) {
            GetMenuFunction()->SetImgButton(HSFMenuFunction::btnRight,
                ResEijiImg_.get(), 0, 0, "CmdAlphabet", 0);
        } else {
            GetMenuFunction()->SetImgButton(HSFMenuFunction::btnRight,
                ResSuujiImg_.get(), 0, 0, "CmdNumber", 0);
        }
    }

    if (bUpdate) {
        UpdateWindow();
    }
}

void WndLogin::drawInputMode() 
{
    if( btnLogin_->IsSelect() || btnPairing_->IsSelect() ){
        return;
    }

    IDisplay* display = GETDISPLAY();

    wstring label;
    const AECHAR* msg =0;
    if (inputmode_ == TMJ_HANNUM) {
        label = "����Ӱ�ށF����";
    } else {
        label = "����Ӱ�ށF�p��";
    }
    msg = label;

	RGBVAL preColor = IDisplay_SetColor(display, CLR_USER_TEXT, HSF_STATIC_TEXT_COLOR);
	IDisplay_DrawText(display, HSF_FONT_NORMAL, msg , -1,
        90, 32, NULL, IDF_TEXT_TRANSPARENT);
    IDisplay_SetColor(display, CLR_USER_TEXT, preColor);
}


void WndLogin::TextFocusInNotify(HSFDirectText* dt)
{
    AEETextInputMode mode = TMJ_HANNUM;
    if(dt)
        mode = dt->GetInputMode();
    else
        return;

    if(mode==TMJ_HANNUM)
        inputmode_ = TMJ_HANNUM;
    else
        inputmode_ = TMJ_HANROMA;

    //setInputMode(inputmode_, false);
    setInputMode(inputmode_, true);
}

// 
// �ȍ~�ʐM���䏈��
//

bool WndLogin::SendRequest_SendStatusInitImpl(GpsStatus* param, bool reSendCommand)
{
    LOG_DEBUG(" WndLogin::SendRequest_SendStatus");
    bool bRet = false;

    SetButtonEnable(false);

    if (m_SendStatusInitCommand.isNull()) {
        m_SendStatusInitCommand = auto_ptr<ServerCommandSendStatusInit>(new ServerCommandSendStatusInit());
    } else {
        if (m_SendStatusInitCommand->IsExecute()) {
            SetButtonEnable(true);
            return bRet;
        }
    }
    if (m_SendStatusInitCommand.get())
    {
        // GPS�̑��ʏ��̎擾(�đ��M�̎��͎g�p���Ȃ�)
        if (!reSendCommand) {
			CreateRequest_SendStatusInit(param);
        }

        // �R�[���o�b�N��ݒ肵�A���N�G�X�g���M
        bRet = PsdHttpUtil::Send(false, m_SendStatusInitCommand, new EventHandler<WndLogin>(this, &WndLogin::OnResNotify_SendStatusInit));

        if (!bRet)
        {
            m_SendStatusInitCommand->SetSend_Retry(true);
        }
    }

    if (!bRet) {
        SetButtonEnable(true);
    }
    return bRet;
}


bool WndLogin::SendRequest_LoginImpl(GpsStatus* param)
{
    LOG_DEBUG(" WndLogin::SendRequest_Login");

	bool bRet = false;

    SetButtonEnable(false);

    m_LoginCommand = auto_ptr<ServerCommandLogin>(new ServerCommandLogin());

    if (m_LoginCommand.get())
    {
        // ���[�UID�p�X���[�h�̐ݒ�
        char buf[64] = {NULL};
        if (txtMemberId_->GetText(buf, sizeof(buf)) || STRLEN(buf) != 0 ) {
            m_LoginCommand->SetUser_id(buf);
        }
        if (txtPassword_->GetText(buf, sizeof(buf)) || STRLEN(buf) != 0 ) {
            m_LoginCommand->SetPassword(MakeKeyHash(buf));
        }

        if (param == NULL || param->GetPsdAccuracy() == CodeAccuracy::E_NO_POSITIONING ) {
            PositioningHistory* phistory = GET_GPSSERVICE()->GetPositioningHistory();
            param = const_cast<GpsStatus*>(phistory->LatestPosition());
        }

        //GPS���ʏ��̐ݒ�
        if (param) {
            cstring datetime;
            param->GetTimeStamp(datetime);
            m_LoginCommand->SetLatitude             ( param->GetLatWGS84());
            m_LoginCommand->SetLongitude            ( param->GetLonWGS84());
            m_LoginCommand->SetAccuracy             ( StrToX::ToStr(param->GetPsdAccuracy()));
            m_LoginCommand->SetPositioning_datetime ( datetime);
        }
        else
        {
            m_LoginCommand->SetAccuracy(NO_POSITIONING);
        }

        // �R�[���o�b�N��ݒ肵�A���N�G�X�g���M
        bRet = PsdHttpUtil::Send(false, m_LoginCommand, new EventHandler<WndLogin>(this, &WndLogin::OnResNotify_Login));
    }

    if (!bRet) {
        ShowInfoCommonMessage("%s.common.http.connection.error");
        SetButtonEnable(true);
    }

    return bRet;
}

bool WndLogin::SendRequest_GetMaster()
{
    LOG_DEBUG(" WndLogin::SendRequest_GetMaster");

	bool bRet = false;

    SetButtonEnable(false);
    
    m_SendGetMasterCommand = auto_ptr<ServerCommandGetMaster>(new ServerCommandGetMaster());
    if (m_SendGetMasterCommand.get())
    {

        // �F�ؗv������̃}�X�^�擾�v�����ɂ́A���[�UID���܂����O�C�����[�U
        // ���ɐݒ肳��Ă��Ȃ����߁A��ʑ��œ��͒l��ݒ�
        char buf[64] = {NULL};
        if (txtMemberId_->GetText(buf, sizeof(buf)) || STRLEN(buf) != 0 ) {
            m_SendGetMasterCommand->SetUser_id(buf);
        }

        // ���O�C�����͏�ɐV�����}�X�^�f�[�^���擾����K�v�����邽�߁A�X�V���t�͋󕶎��ő��M����B
        //Property& infomanage = GET_PROPERTY(PropertyManager::INFOMANAGE);
        //m_SendGetMasterCommand->SetMaster_update_date(infomanage.get("MASTER_UPDATE_DATE"));
        m_SendGetMasterCommand->SetMaster_update_date("");

        PsdWorkMstData* mstData = PsdUtil::GetPsdWorkMstData();
        m_SendGetMasterCommand->SetPsdWorkMstData(*mstData);

        // �R�[���o�b�N��ݒ肵�A���N�G�X�g���M
        bRet = PsdHttpUtil::Send(false, m_SendGetMasterCommand, new EventHandler<WndLogin>(this, &WndLogin::OnResNotify_GetMaster));
    }

    if (!bRet) {
        SetButtonEnable(true);
    }
	SendStatusInitFinish_=false;
    return bRet;
}


boolean WndLogin::OnResNotify_SendStatusInit(uint16 wParam, uint32 dwParam)
{
    LOG_DEBUG(" WndLogin::OnResNotify_SendStatusInit");
    LOG_DEBUG("  @wParam = 0x%04x", wParam);
    LOG_DEBUG("  @dwParam = 0x%08x", dwParam);

    if (WndPsdBase::GetStatusSt()!=stStart) {
        LOG_DEBUG("  wnd is not active!");
        SetButtonEnable(true);
        return FALSE;
    }

    this->SetButtonEnable(false);
	SendStatusInitFinish_ = true;

    //�F�،��ʂ����
    ServerCommandSendStatusInit* command = reinterpret_cast<ServerCommandSendStatusInit*>(dwParam);

    if (wParam && command)
    {
        // �ʐM������
        switch ((int)command->Result_flg())
        {
        case CodeResultFlg::RESULT_TRUE:  // ���ʃt���O.����
            if (m_SendStatusInitCommand->Send_Retry()) {
                SendRequest_LoginImpl(NULL);
            }
            m_SendStatusInitCommand->SetSend_Retry(false);
            (GET_LOGINMANAGER()).DeleteLoginHistoryFile();

            break;
        case CodeResultFlg::RESULT_ABEND:
            // ���ʃt���O.���s
            if (m_SendStatusInitCommand->Send_Retry()) {
                ShowInfoCommonMessage("%s.common.http.connection.error");
            }
            m_SendStatusInitCommand->SetSend_Retry(true);
            break;
        }
    }
    else
    {
        // �ʐM���s��
        if (dwParam == (uint32)SGA_STATUS_CANCEL)
        {
            // �L�����Z����
            if (m_SendStatusInitCommand->Send_Retry()) {
                ShowInfoCommonMessage("%s.common.http.connection.cancel");
            }
        } 
        else
        {
            // �ʐM���s��
            if (m_SendStatusInitCommand->Send_Retry()) {
                ShowInfoCommonMessage("%s.common.http.connection.error");
            }
        }
        m_SendStatusInitCommand->SetSend_Retry(true);
    }

    SetButtonEnable(true);

    return TRUE;
}


boolean WndLogin::OnResNotify_Login(uint16 wParam, uint32 dwParam)
{
    LOG_DEBUG(" WndLogin::OnResNotify_Login");
    LOG_DEBUG("  @wParam = 0x%04x", wParam);
    LOG_DEBUG("  @dwParam = 0x%08x", dwParam);

    if (WndPsdBase::GetStatusSt()!=stStart) {
        LOG_DEBUG("  wnd is not active!");
        SetButtonEnable(true);
        return FALSE;
    }

    this->SetButtonEnable(false);

    //�F�،��ʂ����
    ServerCommandLogin* command = reinterpret_cast<ServerCommandLogin*>(dwParam);

    if (wParam && command)
    {
        // �ʐM������
        switch ((int)command->Result_flg())
        {
        case CodeResultFlg::RESULT_TRUE:  // ���ʃt���O.����
            login_result_ = (int)command->Login_result();
            if (login_result_ == CodeLoginResult::RECOGNITION_OK             ||      // ���O�C���F��OK
                login_result_ == CodeLoginResult::RECOGNITION_OK_NEARDATE ) {        // ���O�C���F��OK(�L�������؂�ԋ�) 
                // �}�X�^���̎擾
                if ( SendRequest_GetMaster() ) {
                    LoginManager& login = GET_LOGINMANAGER();
                    if (!login.LogIn(command)) {
                        //���O�C���ݒ�Ɏ��s
                        ASSERT(false);
                    }

                    ((PsdApplet*)GETAPP())->GetSendRecoveryInfo()->SetNoOperationInterval((int)command->NoOperationTime());
                } else {
                    ShowInfoCommonMessage("%s.common.http.connection.error");
                }

            } else if (login_result_ == CodeLoginResult::RECOGNITION_NG_MISMATCH) { // ���O�C���F��NG(���O�C���F�؏��s��v)
                ClearInputFeild();
                ShowInfoMessage("%s.msg.recvError");
            } else if (login_result_ == CodeLoginResult::RECOGNITION_NG_PERMISSION) { // ���O�C���F��NG(�g�p�����Ȃ�)
                ClearInputFeild();
                ShowInfoMessage("%s.msg.permissionError");
            } else if (login_result_ == CodeLoginResult::RECOGNITION_NG_OUTOFDATE) { // ���O�C���F��NG(�L�������؂�)
                Property& loginuser = GET_PROPERTY(PropertyManager::LOGINUSER);
                loginuser.put("PASSWORD_VALID_PERIOD"   ,command->Password_valid_period());

                // ���O�C���F��NG(�L�������؂�)�̏ꍇ�́A���O�C�����[�U���
                // �Ƀ��[�UID���ݒ肳��Ȃ����߁A�p�X���[�h�ύX��ʂœ��͂��ꂽ
                // ���[�UID���Q�Ƃ��邽�߂ɃZ�b�V�������ɐݒ肷��B
                // (�p�X���[�h�ύX�ʒm�ւ̃��N�G�X�g�ݒ�ɕK�v)
                Property& session = GET_PROPERTY(PropertyManager::SESSION);
                session.put("COMMON.NOOUTOFDATE.USERID", command->User_id().c_str());

                MessageBox::ShowInfo( PsdUtil::GetSysMsg("%s.msg.loginPwdOut", this->GetFunctionId()), 8, "����",CMD_PASSWORD_CHANGE );
            } else if (login_result_ == CodeLoginResult::RECOGNITION_OK_FIRSTTIME) { // ���O�C���F��NG(���񃍃O�C��) 
                // ���񃍃O�C�����͔F��NG�̈����ƂȂ������߁A�}�X�^�擾�v����
                // �s�킸�ɁA�p�X���[�h�ύX��ʂɑJ�ڂ̂ݍs��
                Property& loginuser = GET_PROPERTY(PropertyManager::LOGINUSER);
                loginuser.put("PASSWORD_VALID_PERIOD"   ,command->Password_valid_period());

                // ���񃍃O�C���������̓��O�C�����[�U���Ƀ��[�UID��
                // �ݒ肳��Ȃ����߁A�p�X���[�h�ύX��ʂœ��͂��ꂽ
                // ���[�UID���Q�Ƃ��邽�߂ɃZ�b�V�������ɐݒ肷��B
                // (�p�X���[�h�ύX�ʒm�ւ̃��N�G�X�g�ݒ�ɕK�v)
                Property& session = GET_PROPERTY(PropertyManager::SESSION);
                session.put("COMMON.NOOUTOFDATE.USERID", command->User_id().c_str());

                // ���񃍃O�C�����̃��b�Z�[�W�\��
                MessageBox::ShowInfo( PsdUtil::GetSysMsg("%s.msg.passwordchange", this->GetFunctionId()), 8, "����", CMD_PASSWORD_CHANGE );
            } else {
                // ���O�C���F�،��ʂ��s���l
                ClearInputFeild();
                ShowInfoMessage("%s.msg.loginError");
            }

            break;
        case CodeResultFlg::USER_LOCK:  // ���ʃt���O.���[�U���b�N
            ClearInputFeild();
            ShowInfoMessage("%s.msg.userlock");
            break;
        case CodeResultFlg::RESULT_ABEND:
            SetFirstFocusable(txtMemberId_);
            // ���ʃt���O.���s
            ShowInfoCommonMessage("%s.common.http.connection.error");
            break;
        }
    }
    else
    {
        // �ʐM���s��
        if (dwParam == (uint32)SGA_STATUS_CANCEL)
        {
            // �L�����Z����
            ShowInfoCommonMessage("%s.common.http.connection.cancel");
        } 
        else
        {
            SetFirstFocusable(txtMemberId_);
            // �ʐM���s��
            ShowInfoCommonMessage("%s.common.http.connection.error");
        }
    }
    SetButtonEnable(true);
    return TRUE;
}

boolean WndLogin::OnResNotify_GetMaster(uint16 wParam, uint32 dwParam)
{
    LOG_DEBUG(" WndLogin::OnResNotify_GetMaster");
    LOG_DEBUG("  @wParam = 0x%04x", wParam);
    LOG_DEBUG("  @dwParam = 0x%08x", dwParam);
    if (WndPsdBase::GetStatusSt()!=stStart) {
        LOG_DEBUG("  wnd is not active!");
        SetButtonEnable(true);
        return FALSE;
    }

    this->SetButtonEnable(false);

    //�F�،��ʂ����
    ServerCommandGetMaster* command = reinterpret_cast<ServerCommandGetMaster*>(dwParam);

    LoginManager& login = GET_LOGINMANAGER();

    if (wParam && command)
    {
        // �ʐM������
        switch ((int)command->Result_flg())
        {
        case CodeResultFlg::RESULT_TRUE:  // ���ʃt���O.����
            {
                if (login_result_ == CodeLoginResult::RECOGNITION_OK) {                     // ���O�C���F��OK

                    //this->NexWindowCommand();

                    // ��ʑJ�ڃ^�X�N
                    SAFE_DELETE(nextwnd_task_);
                    nextwnd_task_ = new Multitask<WndLogin>(this, &WndLogin::NextWindowCommand);
                    if (nextwnd_task_) {
                        nextwnd_task_->Nexttask();
                    }

                } else if (login_result_ == CodeLoginResult::RECOGNITION_OK_NEARDATE) {     // ���O�C���F��OK(�L�������؂�ԋ�)
                    Property& loginuser = GET_PROPERTY(PropertyManager::LOGINUSER);
                    loginuser.put("PASSWORD_OUT_NEAR"       ,PASSWORD_OUT_NEAR);
                    MessageBox::ShowConf( PsdUtil::GetSysMsg("%s.msg.loginPwdOutNear", this->GetFunctionId()), 8, "�͂�","������", CMD_PASSWORD_CHANGE, CMD_NEXT_WINDOW );
                } else {
                    ASSERT(false);
                }
                break;
            }
        case CodeResultFlg::USER_LOCK:  // ���ʃt���O.���[�U���b�N
            ClearInputFeild();
            login.LogoffByServer();
            ShowInfoMessage("%s.msg.userlock");
            break;
        case CodeResultFlg::RESULT_ABEND:
            SetFirstFocusable(txtMemberId_);
            login.LogoffByServer();
            // �G���[�_�C�A���O
            PsdWorkMstData* mstData = PsdUtil::GetPsdWorkMstData();
            if(mstData->GetCmdMasterError()){
                // �}�X�^�擾�ŃG���[
                ShowInfoCommonMessage("%s.common.getmaster.01.error");
            }else{
                // ���ʃt���O.���s
                ShowInfoCommonMessage("%s.common.http.connection.error");
                break;
            }
        }
    }
    else
    {
        // �ʐM���s��
        if (dwParam == (uint32)SGA_STATUS_CANCEL)
        {
            login.LogoffByServer();
            // �L�����Z����
            ShowInfoCommonMessage("%s.common.http.connection.cancel");
        } 
        else
        {
            SetFirstFocusable(txtMemberId_);
            login.LogoffByServer();
            // �ʐM���s��
            ShowInfoCommonMessage("%s.common.http.connection.error");
        }
    }

    SetButtonEnable(true);
    return TRUE;
}

WndLogin::INPUT_CHECK WndLogin::InputDataCheck() {
    
    char buf[64] ={NULL};
    if (!txtMemberId_->GetText(buf, sizeof(buf)) || STRLEN(buf) == 0 ) {
        SetFirstFocusable(txtMemberId_);
        ShowInfoMessage("%s.msg.loginErrorIdEmpty");
        return CHK_NULL_ERROR;
    }
    if (!txtPassword_->GetText(buf, sizeof(buf)) || STRLEN(buf) == 0 ) {
        SetFirstFocusable(txtPassword_);
        ShowInfoMessage("%s.msg.loginErrorPwdEmpty");
        return CHK_NULL_ERROR;
    }
    if (STRLEN(buf) < PASSWORD_MINLEN ) {
        SetFirstFocusable(txtPassword_);
        ShowInfoMessage("%s.msg.loginErrorPwdLength");
        return CHK_SIZE_ERROR;
    }

    return CHK_SUCCESS;
}

void WndLogin::ClearInputFeild() {

    txtMemberId_->SetText("");
    txtPassword_->SetText("");
    SetFirstFocusable(txtMemberId_);
}

void WndLogin::OnOpenWindow()
{
    WndPsdBase::OnOpenWindow();

    // �����폜
    PsdUtil::RemoveHistory((const char*)(*this));

    // �W���{�^���摜
    imgLabelUserID_ = smart_ptr<HSFWImage>(HSFWImage::LoadResImage(FILE_GMK_BAR, IDI_IMG_USERID));
    imgLabelPass_   = smart_ptr<HSFWImage>(HSFWImage::LoadResImage(FILE_GMK_BAR, IDI_IMG_PASS));
}

void WndLogin::OnCloseWindow()
{
    m_SendStatusInitCommand.Delete();
    m_LoginCommand.Delete();
    m_SendGetMasterCommand.Delete();

    ResEijiImg_.release();
    ResSuujiImg_.release();

    imgLabelUserID_     = NULL;
    imgLabelPass_       = NULL;
    txtMemberId_        = NULL;
    txtPassword_        = NULL;
    btnLogin_           = NULL;
    btnPairing_         = NULL;
    foucusIndex_        = 1;
    m_modeUserId        = TMJ_HANNUM;
    m_modePassword      = TMJ_HANNUM;
    inputmode_          = TMJ_HANNUM;

    SAFE_DELETE(nextwnd_task_);
    SAFE_DELETE(m_AppVerMng);

    WndPsdBase::OnCloseWindow();
}

bool WndLogin::IsPairingMode() {
    return btnPairing_->IsSelect();
}

void WndLogin::SetButtonEnable(bool enable) {
    if (btnLogin_)
        btnLogin_->SetEnable(enable);
    if (btnPairing_)
        btnPairing_->SetEnable(enable);    
}

bool WndLogin::IsButtonEnable() {
    if (    (btnLogin_ && btnLogin_->IsEnable())
        ||  (btnPairing_ && btnPairing_->IsEnable()) )
    {
        return true;
    }
    return false;
}

const cstring WndLogin::MakeKeyHash(const char* key)
{  
    // IHash �C���X�^���X�쐬
    IShell* shell = GETSHELL();
    IHash* hash = NULL;
    ISHELL_CreateInstance(shell, AEECLSID_MD5, (void**)&hash);

    // Hash ���s
    int keySize = STRLEN(key);
    IHASH_Update(hash, (byte*)key, keySize);
    
    // ���ʎ擾
    byte hashKey[MD5_LENGTH + 1] = {NULL};
    int hashSize = MD5_LENGTH;
    IHASH_GetResult(hash, hashKey, &hashSize);

    // ���ʕ\��
    char buffer[256] = {NULL};
    int i = SPRINTF(buffer, "");
    int j = 0;
    while (j < MD5_LENGTH) {
        i += SPRINTF(buffer + i, "%02x", hashKey[j++]);
    }
    LOG_DEBUG("WndChgPwd::MakeKeyHash() %s", buffer);
    IHASH_Release(hash);

    const char* ret = buffer;
    return cstring(ret);
}

void WndLogin::OnSuspendWindow() {

    Property& session = GET_PROPERTY(PropertyManager::SESSION);

    foucusIndex_ = this->GetFoucusPos();
    m_modeUserId = txtMemberId_->GetInputMode();
    m_modePassword = txtPassword_->GetInputMode();

    char keyBuf[32];
    char buf[64];
    ZERO_CLEAR(keyBuf);
    ZERO_CLEAR(buf);
    if (txtMemberId_->GetText(buf, sizeof(buf))) {
        if (0 != STRCMP("", buf))
        {
            session.put("%s.USERID", buf, SESSIONID_0001010 );
        }
        else
        {
            SNPRINTF(keyBuf, sizeof(keyBuf), "%s.USERID", SESSIONID_0001010);
            session.remove(keyBuf);
        }
    }
    if (txtPassword_->GetText(buf, sizeof(buf))) {
        if (0 != STRCMP("", buf))
        {
            session.put("%s.PASSWORD", buf, SESSIONID_0001010);
        }
        else
        {
            SNPRINTF(keyBuf, sizeof(keyBuf), "%s.PASSWORD", SESSIONID_0001010);
            session.remove(keyBuf);
        }
    }

    int16 CursorPos;
    // ���O�C��ID �J�[�\���ʒu
    CursorPos = txtMemberId_->GetCursorPos();
    SNPRINTF(buf, sizeof(buf), "%d", CursorPos);
    session.put("%s.USERID.POS", buf, SESSIONID_0001010 );

    // �p�X���[�h �J�[�\���ʒu
    CursorPos = txtPassword_->GetCursorPos();
    SNPRINTF(buf, sizeof(buf), "%d", CursorPos);
    session.put("%s.PASSWORD.POS", buf, SESSIONID_0001010 );
	if(!SendStatusInitFinish_)
	{
		if (m_SendStatusInitCommand.isNull()) {
			CreateRequest_SendStatusInit(NULL);
		}
		m_SendStatusInitCommand->SetSend_Retry(true);
	}

}


int WndLogin::GetFoucusPos() {
    m_modeUserId = txtMemberId_->GetInputMode();
    m_modePassword = txtPassword_->GetInputMode();

    if (txtPassword_    && txtPassword_->IsSelect())
    {
        inputmode_ = txtPassword_->GetInputMode();
        return 2;
    }
    else if (btnLogin_       && btnLogin_->IsSelect())
        return 3;
    else if (btnPairing_     && btnPairing_->IsSelect())
        return 4;
    else
    {
        inputmode_ = txtMemberId_->GetInputMode();
        return 1;
    }
}


void WndLogin::SetFoucusPos(int index) {

    txtMemberId_->SetInputMode(m_modeUserId);
    txtPassword_->SetInputMode(m_modePassword);

    if (txtPassword_    && index == 2) 
    {
        txtPassword_->SetInputMode(inputmode_);
        SetFirstFocusable(txtPassword_);
    }
    else if (btnLogin_       && index == 3)
    {
        SetFirstFocusable(btnLogin_);
    }
    else if (btnPairing_     && index == 4)
    {
        SetFirstFocusable(btnPairing_);
    }
    else 
    {
        txtMemberId_->SetInputMode(inputmode_);
        SetFirstFocusable(txtMemberId_);
    }
}

bool WndLogin::IsLoacalHttps() {
    bool bRet = false;
    if (!m_LoginCommand.isNull())
        bRet |= m_LoginCommand->IsExecute();
    if (!m_SendStatusInitCommand.isNull())
        bRet |= m_SendStatusInitCommand->IsExecute();
    if (!m_SendGetMasterCommand.isNull())
        bRet |= m_SendGetMasterCommand->IsExecute();
    return bRet;
}


void WndLogin::CreateRequest_SendStatusInit(GpsStatus* param)
{
	if (m_SendStatusInitCommand.isNull()) {
		m_SendStatusInitCommand = auto_ptr<ServerCommandSendStatusInit>(new ServerCommandSendStatusInit());
	}
    m_SendStatusInitCommand->Initialize();
    m_SendStatusInitCommand->SetStatus_pairing      (NO_PAIRING);                   // "0":�y�A�����O�Ȃ�
    m_SendStatusInitCommand->SetReason              (REASON_START_APLI);            // "01":���󋵎擾���R.�A�v���N��
    m_SendStatusInitCommand->SetStatus_inservice    (NO_INSERVICE);                 // "0":�Ɩ����J�n
    m_SendStatusInitCommand->SetStatus_forced_lock  (NO_LOCKING);                   // "0":�񃍃b�N
    m_SendStatusInitCommand->SetStatus_notice       (STATUS_NORMAL);                // "0":��щz���񍐏�.�ʏ���
    m_SendStatusInitCommand->SetMatter_status       (NO_RESPOND_MATTER);            // "0":���Ė��Ή����

    // �d�r�c��
    m_SendStatusInitCommand->SetStatus_battery(StrToX::ToStr(PsdUtil::GetPsdSendStatusBattery()));

    // �d�g���
    m_SendStatusInitCommand->SetStatus_signal(StrToX::ToStr(PsdUtil::GetPsdSendStatusSignal()));

    m_SendStatusInitCommand->SetSend_datetime       (JulianDate::NowToString());

    if (param == NULL || param->GetPsdAccuracy() == CodeAccuracy::E_NO_POSITIONING ) {
        PositioningHistory* phistory = GET_GPSSERVICE()->GetPositioningHistory();
        param = const_cast<GpsStatus*>(phistory->LatestPosition());
    }

    if (param) {
        cstring datetime;
        param->GetTimeStamp(datetime);
        // GPS���
        m_SendStatusInitCommand->SetLatitude            (param->GetLatWGS84());
        m_SendStatusInitCommand->SetLongitude           (param->GetLonWGS84());
        m_SendStatusInitCommand->SetAccuracy            (StrToX::ToStr(param->GetPsdAccuracy()));
        m_SendStatusInitCommand->SetPositioning_datetime(datetime);
    }
    else
    {
        m_SendStatusInitCommand->SetAccuracy(NO_POSITIONING);
    }

    if ((GET_LOGINMANAGER()).IsAbnormalTermination()) {
        m_SendStatusInitCommand->SetStatus_app_failure(StrToX::ToStr((int)CodeStatusAppFailure::ILLEGALEND));
    }
}
