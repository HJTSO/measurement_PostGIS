/*********************************************************************/
/*                       Keitai/Police                               */
/*                                                                   */
/*     ALL RIGHTS RESERVED.                                          */
/*     COPYRIGHT (C) 2010,  HITACHI SOFTWARE ENGINEERING CO.,LTD.    */
/*     Hitachi Software Engineering Co., Ltd. Confidential           */
/*********************************************************************/

/**
 * @file    WndLogin.cpp
 * @brief   ���O�C�����
 * 
 * PSD/�f�[�^�[���A�v���̃��O�C����ʃN���X�ł��B<BR>
 * 
 * 
 * $Author: kenichi.nakahara.rz@ps.hitachi-solutions.com $
 * $Date:: 2010-12-23 13:54:29#$
 */
#ifndef WND_LOGIN_H__
#define WND_LOGIN_H__

#include "WndPsdBase.h"
#include <util/smart_ptr.hxx>
//#include "property_base.h"
#include "ServerCommandSendStatusInit.hpp"
#include "ServerCommandLogin.hpp"
#include "ServerCommandGetMaster.hpp"
#include "GpsServiceListener.h"

#define WNDID_LOGIN   WNDID_PREFIX "Login"  //!< ���ID

#define FUNCID_0001010          "DW_0001010"    //!< �t�@���N�V����ID
#define DATATYPE_0001010        "Login"         //!< �f�[�^�^�C�v
#define SESSIONID_0001010       FUNCID_0001010  //!< �Z�b�V����ID

//! �A�v���o�[�W�����`�F�b�N���������s�R�}���h(���񊈓��󋵒ʒm�g���K�[)
#define CMD_FINISHED_VER_CHK    "CmdFinishedVerChk"

class HSFWImage;
class MultitaskInterface;
class AppVerChk;

/**
 * @brief ���O�C�����
 *
 * ���O�C����ʃN���X�ł��B<BR>
 * ���O�C����ʂł́A���ړ��͂ɂ�郆�[�UID/�p�X���[�h���͂����{���܂��B<BR>
 * �y�A�����O�̎��{�ۂ�I�����Ẵ��O�C�����\�ł��B
 *
 */
class WndLogin : public WndPsdBase
{

private:
    HSFDirectText* txtMemberId_;            //!< ���[�UID�e�L�X�g�R���g���[��
    HSFDirectText* txtPassword_;            //!< �p�X���[�h�e�L�X�g�R���g���[��
    HSFStaticCtl* stsVersion_;
    HSFPictButtonCtl* btnLogin_;            //!< ���O�C���{�^���R���g���[��
    HSFPictButtonCtl* btnPairing_;          //!< �y�A�����O�{�^���R���g���[��

    smart_ptr<HSFWImage> imgLabelUserID_ ;  //!< ���[�UID���x���摜
    smart_ptr<HSFWImage> imgLabelPass_ ;    //!< �p�X���[�h���x���摜

    // �t���O�n�p�����[�^
    int     login_result_;                  //!< ���O�C������

    //! �F�ؗv���ʐM�����R�}���h
    auto_ptr<ServerCommandLogin> m_LoginCommand;

    //! �����󋵒ʒm0001�ʐM�����R�}���h
    auto_ptr<ServerCommandSendStatusInit> m_SendStatusInitCommand;

    //! �}�X�^�擾�v���ʐM�����R�}���h
    auto_ptr<ServerCommandGetMaster> m_SendGetMasterCommand;

    //! �A�v���o�[�W�������
    AppVerChk* m_AppVerMng;

    typedef AEETextInputMode InputMode;

    InputMode inputmode_;           //!< �e�L�X�g���̓��[�h

    InputMode m_modeUserId;         //!< ���[�UID�����̓��[�h
    InputMode m_modePassword;       //!< �p�X���[�h�����̓��[�h

    bool initailed_;                //!< ����N���t���O
    int foucusIndex_;               //!< �R���g���[���̃t�H�[�J�X�ʒu

    MultitaskInterface* nextwnd_task_;      //!< ��ʑJ�ڃ^�X�N(�񓯊�)

    smart_ptr<HSFImage>   ResSuujiImg_;   //!< �����L�[�摜
    smart_ptr<HSFImage>   ResEijiImg_;    //!< �p���L�[�摜
	bool SendStatusInitFinish_;     //!< �����󋵒ʒm0001�ʐM�����R�}���h���M�����t���O

public:

    /**
     * @brief �R���X�g���N�^
     */
    WndLogin();

    /**
     * @brief �f�X�g���N�^
     */
    ~WndLogin();


    /**
     * @sa WndBase::operator const char*()
     */
    virtual operator const char*() { return WNDID_LOGIN; }

    /**
     * @sa WndBase::GetFunctionId()
     */
    virtual const char* GetFunctionId() const { return FUNCID_0001010; }

    /**
     * @brief �R���g���[���̏�����
     * 
     * ��ʃR���g���[���̏��������������{����B
     * ���O�C����ʏ�ԕʂɈȉ��̏��������{����B
     * <li>
     *     <ol>
     *     <li>�������b�N�����ʒm�F�����󋵒ʒm0003�����N�G�X�g</li>
     *     <li>�y�A�����O�ؒf�ɂ�鋭�����O�A�E�g�F�����󋵒ʒm0003�����N�G�X�g</li>
     *     </ol>
     * </li>
     *
     * @retval true �R���g���[���̏��������� 
     * @retval false �R���g���[���̏��������s�i���̏ꍇ��ʕ`�悳��܂���j
     * @sa WndBase::InitializeControl()
     */
    virtual bool InitializeControl();

    /**
     * @brief �R�}���h�n���h��
     * 
     * �e�R�}���h�ʂɑ��쎞�̏��������{���܂��B
     *
     * @param cmdStr �R�}���h������
     * @param wParam �C�x���g�R�[�h�̏ڍ׃I�u�W�F�N�g
     *
     * @sa WndBase::HandleCommand()
     */
    virtual void HandleCommand(const char* cmdStr, uint16 wParam);

    /**
     * @brief �e���b�v���p�L�����擾
     *
     * @retval true �e���b�v�𗘗p����
     * @retval false �e���b�v�𗘗p���Ȃ�
     */
    virtual bool IsTelopUsed() { return false; }

    /**
     * @brief ��ʕ\���O�������s���B
     *
     * ��ʂ̕`�揈���̒���ɌĂяo�����B<BR>
     * �ʐM��Ԃ̒ʒm�B����ђʐM���N�G�X�g�G���[���̒ʒm���s���B
     *
     * @retval TRUE ������
     * @retval FALSE ������
     * @sa WndBase::OnBeginWindow()
     */
    virtual boolean OnBeginWindow();

    /**
     * @brief ��ʕ`��̌㏈�����s���B
     *
     * @retval TRUE ������
     * @retval FALSE ������
     * @sa WndBase::OnDrawOtherAfter()
     */
    boolean OnDrawOtherAfter();    

    /**
     * @brief ���ڃe�L�X�g���͂̓��̓��[�h�擾
     *
     * �t�H�[�J�X�̓������Ă��钼�ڃe�L�X�g���͂̓��̓��[�h���擾����B<BR>
     *
     * @param [in] dt �t�H�[�J�X�������������ڃe�L�X�g���̓R���g���[��
     */
    void TextFocusInNotify(HSFDirectText* dt);

    /**
     * @brief �E�C���h�E�̊J�n����
     *
     * ��ʂ֑J�ڂ���^�C�~���O�ŌĂ΂�܂��B<br>
     * ���W���[��(���t���b�V��)���͌Ă΂�܂���B<br>
     */
    virtual void OnOpenWindow();

    /**
     * @brief �E�C���h�E�̏I������
     *
     * ��ʂ��J�ڂ���^�C�~���O�ŌĂ΂�܂��B<br>
     * �T�X�y���h(���t���b�V��)���͌Ă΂�܂���B<br>
     */
    virtual void OnCloseWindow();


	/**
	 * @brief �L�[�C�x���g���t�b�N����B
	 *
     * �L�[�C�x���g�����̏���������ɌĂяo�����B<BR>
     * �߂�l�ɂ���āA�ȍ~�̏������Ăяo�����ǂ������R���g���[������B<BR>
     *
	 * @param [in] individualObject �ʃI�u�W�F�N�g
	 * @param [in] eCode �C�x���g
	 * @param [in] wParam �L�[�R�[�h
	 * @param [in] dwParam �C���L�[�t���O
	 * @retval TRUE ������(�ȍ~�̃L�[�C�x���g�����A�R�}���h�n���h�����̏������Ăяo����Ȃ�)
	 * @retval FALSE ������(�ȍ~�̃L�[�C�x���g�����A�R�}���h�n���h�����̏������Ăяo��)
	 */
	virtual boolean	HookKeyEvent(void *IndividualObject, AEEEvent eCode, uint16 wParam, uint32 dwParam);

    /**
     * @brief �L�[�C�x���g����
     *
     * �e�L�[�ʂɑ��쎞�̏��������{����B<BR>
     * �㉺�����L�[���쎞�̃e���b�v�\�������̕ύX�A<BR>
     * ���E�����L�[���쎞�̃y�[�W�J�ڏ����̌Ăяo���A<BR>
     * �����L�[�������̈�Ďw�ߏ��I���������̌Ăяo�����s���B
     * 
     * @param [in] wParam �C�x���g�R�[�h
     * @param [in] dwParam �C�x���g�R�[�h�̏ڍ׃I�u�W�F�N�g
     * @retval TRUE ����
     * @retval FALSE �ُ�
     * @sa WndBase::EventKey()
     */
    boolean EventKey(uint16 wParam, uint32 dwParam);

protected:

    /**
	 * @brief �E�C���h�E�̃T�X�y���h����
     *
	 * �T�X�y���h�̃^�C�~���O�ŌĂ΂�܂��B<br>
	 */
    virtual void OnSuspendWindow();

    /**
     * @brief �N����GPS���ʏ����̌Ăяo�����s���B
     *
     */
    void Request_GpsStatus();

    /**
     * @brief �����󋵒ʒm0001���N�G�X�g�f�[�^�̐ݒ�A�ʐM�����̌Ăяo�����s���B
     *
     * @param param�@GPS���ʏ��
     */
    void SendRequest_SendStatusInit(GpsStatus* param) { SendRequest_SendStatusInitImpl(param);} ;

    /**
     * @brief �����󋵒ʒm0001���N�G�X�g�f�[�^�̐ݒ�A�ĒʐM�����̌Ăяo�����s���B
     *
    * @retval true �ʐM���N�G�X�g��������
     * @retval false �ʐM���N�G�X�g�������s
     */
    bool SendRequest_ReSendStatusInit() { return SendRequest_SendStatusInitImpl(NULL, true);};

    /**
     * @brief �F�ؗv�����N�G�X�g�f�[�^�̐ݒ�A�ʐM�����̌Ăяo�����s���B
     *
     * @param param�@GPS���ʏ��
     *
     * @retval true �ʐM���N�G�X�g��������
     * @retval false �ʐM���N�G�X�g�������s
     */
    void SendRequest_Login(GpsStatus* param) { SendRequest_LoginImpl(param);};

    /**
     * @brief �}�X�^�擾���N�G�X�g�f�[�^�̐ݒ�A�ʐM�����̌Ăяo�����s���B
     *
     *
     * @retval true �ʐM���N�G�X�g��������
     * @retval false �ʐM���N�G�X�g�������s
     */
    bool SendRequest_GetMaster();

    /**
     * @brief �ʐM�C�x���g�̃��X�|���X�ʒm����
     * <li>
     *     <ol>
     *     <li>�����󋵒ʒm0001������</li>
     *     </ol>
     * </li>
     * @param [in] wParam �C�x���g�R�[�h
     * @param [in] dwParam �C�x���g�R�[�h�̏ڍ׃I�u�W�F�N�g
     * @retval true ����
     * @retval false �ُ�
     */
    virtual boolean OnResNotify_SendStatusInit(uint16 wParam, uint32 dwParam);


    /**
     * @brief �ʐM�C�x���g�̃��X�|���X�ʒm�����i���O�C���̂ݎ��{�j
     * <li>
     *     <ol>
     *     <li>�F�ؗv��������</li>
     *     </ol>
     * </li>
     * @param [in] wParam �C�x���g�R�[�h
     * @param [in] dwParam �C�x���g�R�[�h�̏ڍ׃I�u�W�F�N�g
     * @retval true ����
     * @retval false �ُ�
     */
    virtual boolean OnResNotify_Login(uint16 wParam, uint32 dwParam);


    /**
     * @brief �ʐM�C�x���g�̃��X�|���X�ʒm�����i���O�C���̂ݎ��{�j
     * <li>
     *     <ol>
     *     <li>�F�ؗv��������</li>
     *     </ol>
     * </li>
     * @param [in] wParam �C�x���g�R�[�h
     * @param [in] dwParam �C�x���g�R�[�h�̏ڍ׃I�u�W�F�N�g
     * @retval true ����
     * @retval false �ُ�
     */
    virtual boolean OnResNotify_GetMaster(uint16 wParam, uint32 dwParam);


    enum INPUT_CHECK {
        CHK_SUCCESS,
        CHK_NULL_ERROR,
        CHK_SIZE_ERROR
    };

    /**
     * @brief ���̓`�F�b�N
     *
     * @return ���̓`�F�b�N����
     */
    INPUT_CHECK InputDataCheck();

    /** 
     * @breif ���̓��[�h�̐��
     * @param [in] mode ���̓��[�h
     * @param [in] bUpdate �\���X�V�L��
     */
    void setInputMode(InputMode mode, bool bUpdatee);

    /** 
     * @breif ���̓��[�h�̕`��
     * ���̓��[�h��`�悵�܂��B
     */
    void drawInputMode();


private:

    /**
     * @brief ���̓t�B�[���h�̏������B
     *
     */
    void ClearInputFeild();
    
    /**
     * @brief �t�H�[�J�X�ʒu��Ԃ��܂��B
     * 
     * @return �t�H�[�J�X�ʒu�̃C���f�b�N�X
     */
    int GetFoucusPos();


    /**
     * @brief �t�H�[�J�X�ʒu��ݒ肵�܂��B
     * 
     * @param �t�H�[�J�X�ʒu�̃C���f�b�N�X
     */
    void SetFoucusPos(int index);

    /**
     * @brief �y�A�����O���[�h�ł��邩�𔻒�
     *
     * @retval true �y�A�����O���[�h
     * @retval false �ʏ탂�[�h�i�y�A�����O�Ȃ��j
     */ 
    bool IsPairingMode();

    /**
     * @brief ���O�C���F�ؐ������̎���ʑJ��
     *
     */ 
    void NextWindowCommand();


    /**
     * @brief �{�^���̗L���ېݒ�
     * @param enable true:�L���ȏꍇ false:�����ȏꍇ
     */ 
    void SetButtonEnable(bool enable);

    /**
     * @brief �{�^���̗L����
     * @retval true �L��
     * @retval false ����
     */ 
    bool IsButtonEnable();

    /**
     * @brief �p�X���[�h��MD5�n�b�V���֐�
     * @param key �ϊ���������
     * @return  �ϊ��㕶����
     */
    const cstring MakeKeyHash(const char* key);

    /**
     * @brief �����󋵒ʒm0001���N�G�X�g�f�[�^�̐ݒ�A�ʐM�����̌Ăяo�����s���B
     *
     * @param reSendCommand�@�đ��t���O�@�đ��̏ꍇ��true
     * @retval true �ʐM���N�G�X�g��������
     * @retval false �ʐM���N�G�X�g�������s
     */
    bool SendRequest_SendStatusInitImpl(GpsStatus* param, bool reSendCommand = false);

    /**
     * @brief �F�ؗv�����N�G�X�g�f�[�^�̐ݒ�A�ʐM�����̌Ăяo�����s���B
     *
     * @param param�@GPS���ʏ��
     *
     * @retval true �ʐM���N�G�X�g��������
     * @retval false �ʐM���N�G�X�g�������s
     */
    bool SendRequest_LoginImpl(GpsStatus* param);


    /**
     * @brief ���O�C���{�^���̉�������
     */
    void LoginButtonPush();


    /**
     * @brief ���O�C����ʂɂ�郍�[�J���̒ʐM�����{���ł��邩����
     * @retrun �ʐM���
     * @retval true ���{��
     * @retval false �����{
     */
    bool IsLoacalHttps();

    /**
     * @brief �����󋵒ʒm0001���N�G�X�g�f�[�^�̍쐬���s���B
     *
     * @param param�@GPS���ʏ��
     */
	void CreateRequest_SendStatusInit(GpsStatus* param);

};

#endif // WND_LOGIN_H__
