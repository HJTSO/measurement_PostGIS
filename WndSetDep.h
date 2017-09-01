/*********************************************************************/
/*                           GeoMation                               */
/*                                                                   */
/*     ALL RIGHTS RESERVED.                                          */
/*     COPYRIGHT (C) 2010,  HITACHI SOFTWARE ENGINEERING CO.,LTD.    */
/*     Hitachi Software Engineering Co., Ltd. Confidential           */
/*********************************************************************/

/**
* @file    WndSetDep.h
* @brief   ������͉��
*
* $Author: tomohiro.kurokawa.ev@ps.hitachi-solutions.com $
* $Date:: 2010-10-19 20:55:42#$
*/

#ifndef WND_SET_DEP_H__
#define WND_SET_DEP_H__

#include "WndPsdPageList.h"

#define WNDID_SET_DEP           WNDID_PREFIX "SetDep"       //!< ���ID
//#define FUNCID_0003020          "DW_0003020"                //!< �t�@���N�V����ID
#define FUNCID_0003020          "DW_0021010"
#define DATATYPE_040            "setdep"

#include <AEEFile.h>

/**
* @brief ������͉��
*
* ������͉�ʃN���X�ł��B<BR>
* ������͉�ʂł́A���O�C�����Ɏ�M��������}�X�^�������Ƃɕ����I�������郊�X�g��\�����܂��B<BR>
*
*/

struct headq_data 
{
    char headq_name[64];
    char headq_ip[64];
    char headq_srs[64];
} ;


class WndSetDep : public WndPsdPageList
{


private:


	vector<cstring> headqNamelist;
	vector<cstring> headqIplist;
	vector<cstring> headqSRSlist;

    int current_headq_index;
    
    //�_�C�A���O��ʕ\�����t���O
    bool viewing_dialog_flag;

public:
    /**
    * @brief �R���X�g���N�^
    */
    WndSetDep();

    /**
    * @brief �f�X�g���N�^
    */
    virtual ~WndSetDep();

    /**
    * @sa WndBase::operator const char*()
    */
    virtual operator const char*() { return WNDID_SET_DEP; }

    /**
    * @sa WndBase::GetFunctionId()
    */
//    virtual const char* GetFunctionId() const { return FUNCID_0003020; }
    virtual const char* GetFunctionId() const { return FUNCID_0003020; }

    /**
    * @brief �R���g���[��������������B
    *
    * @return ����
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
    * @brief ExclusionMgr�ɂ��C�x���g�n���h��
    * �C�x���g���n���h�����܂��BHandleCommand��EventKey������ɌĂяo����܂��B
    *
    * @param wParam �C�x���g�R�[�h
    * @param dwParam �C�x���g�R�[�h�̏ڍ׃I�u�W�F�N�g
    * @retval true ����
    * @retval false �ُ�
    * @sa HSFBaseObjSC::HandleEvent()
    */
    virtual boolean HandleEvent(AEEEvent eCode, uint16 wParam, uint32 dwParam);

	/**
	 * @brief �t�@�C�����ǂݍ��ݕ�����
	 *
	 * �t�@�C����擪����ǂݍ��݁A�w��o�b�t�@�ɕ������Ċi�[����B<br>
	 * ���� buf �� NULL ���ݒ肳�ꂽ�ꍇ�́A�Ǎ��͂����t�@�C���T�C�Y�̂ݕԂ��B<br>
	 *
	 * @param [in]  pPath �t�@�C���p�X
	 * @param [out] pBuf �o�b�t�@
	 * @param [in]  iBufSize �o�b�t�@�T�C�Y
     *
	 * @return �ǂݍ��݃T�C�Y
	 */
    int32  WndSetDep::ReadFile(char* pPath, unsigned char* pBuf, int iBufSize);

	/**
	 * @brief �t�@�C���ɏ�������
	 *
	 * �w��o�b�t�@���t�@�C���o�͂���B<br>
	 * �����t�@�C��������ꍇ�͏㏑���A�Ȃ��ꍇ�͐V�K�Ńt�@�C�����쐬����B<br>
	 *
	 * @param [in]  pPath �t�@�C���p�X
	 * @param [in]  pBuf �o�b�t�@
	 * @param [in]  iBufSize �o�b�t�@�T�C�Y
     *
	 * @return �������݃T�C�Y
	 */
    uint32 WndSetDep::WriteFile(char* pPath, unsigned char* pBuf, int iBufSize);

	/**
	 * @brief �t�@�C���L������
	 *
	 * �t�@�C���̗L�����m�F����<br>
	 * �����t�@�C��������ꍇ�͏㏑���A�Ȃ��ꍇ�͐V�K�Ńt�@�C�����쐬����B<br>
	 *
	 * @param [in]  pPath �t�@�C���p�X
     *
	 * @return �t�@�C���L��
	 */
    int32  WndSetDep::IsExistFile(const char* pPath);


	/**
	* @brief �ĊJ����(���W���[��)
	*/
	virtual void OnResumeWindow();


    /**
     * @brief �T�X�y���h����������
     * 
     */
    virtual void OnSuspendWindow();
    
    /**
     * @brief ��ʕ\���O�������s���B
     *
     * ��ʂ̕`�揈���̒���ɌĂяo�����B<BR>
     *
     * @retval TRUE ������
     * @retval FALSE ������
     * @sa WndBase::OnBeginWindow()
     */
    virtual boolean OnBeginWindow();

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
	 * @brief �m�F�_�C�A���O�\������
     *
	 * �_�C�A���O��\�����܂��B<br>
	 */
	void WndSetDep::ShowComfarmDialog();

	/**
	 * @brief FG�ABG�A�v���N������
     *
	 * FG�ABG�A�v���N���������܂��B<br>
	 * �N���A�v���́A�����Ȃ���ʂɑJ�ڂ��ăT�X�y���h��҂��܂��B<br>
	 */
    void  WndSetDep::StartApple(const char *args);


	/**
	 * @brief �\�t�g�L�[�̏�����
     *
	 * �\�t�g�L�[�����������܂��B<br>
	 */
    virtual bool InitializeSoftkey();
 
protected:

    /**
    * @brief ���X�g�̍��ڂ�I�������Ƃ��ɌĂ΂��R�}���h
    * 
    * @param index �I������A�C�e���̃C���f�b�N�X
    * @retval true ����
    * @retval false �ُ�
    */
    virtual bool SelectItemCommand(int index);

private:

    /**
    * @brief �S�Ẵ��X�g�����y�[�W���X�g�ɐݒ�
    * 
    * @retval true ����
    * @retval false �ُ�
    */
    bool SetPagingList();

    /**
    * @brief ����ʑJ��
    * 
    * @retval true ����
    * @retval false �ُ�
    */
    bool NextMoveWnd();
};

#endif // WND_SET_DEP_H__
