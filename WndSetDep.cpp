/*********************************************************************/
/*                           GeoMation                               */
/*                                                                   */
/*     ALL RIGHTS RESERVED.                                          */
/*     COPYRIGHT (C) 2010,  HITACHI SOFTWARE ENGINEERING CO.,LTD.    */
/*     Hitachi Software Engineering Co., Ltd. Confidential           */
/*********************************************************************/

/**
 * @file    WndSetDep.cpp
 * @brief   本部選択画面
 *
 * $Author: tomohiro.kurokawa.ev@ps.hitachi-solutions.com $
 * $Date:: 2010-10-21 15:27:17#$
 */

#include "WndSetDep.h"
#include "MessageBox.h"
#include "PsdUtil.hpp"
#include "CmdLogoff.h"

#include "wndforcelock.h"

#define HEADQUATER_MAX       53     //47都道府県+α

#define HEADQUATER_DEFAULT   12  //東京

#define ADSINFO_FILE         "start.dat"

#define ADSINFO_MAX_STRINGS  64

#include "../../gmkeitai/psdbg.bid"
#include "../../gmkeitai/psd.bid"
#include "./bg/psdbgevttype.hpp"

#define PSDLAUNCH_DEBUG_IP_FLAG "0"
#include "../../gmkeitai/psdlaunch.brh"
#include <tool/stringutil.hpp>


/**
 * @brief WndSetDep コンストラクタ
 */
WndSetDep::WndSetDep()
{

	const char* msg = StringUtil::Load(FILE_GMK_BAR, IDS_STRING_PSDLAUNCH_DEBUG_IP);
	
		    headq_data da[HEADQUATER_MAX] = 
	    {
			{"北海道警察本部","https://172.26.120.1","EPSG:32654"},
			{"函館方面本部","https://172.26.120.4","EPSG:32654"},
			{"旭川方面本部","https://172.26.120.9","EPSG:32654"},
			{"釧路方面本部","https://172.26.120.12","EPSG:32655"},
			{"北見方面本部","https://172.26.120.17","EPSG:32654"},
			{"青森県警察本部","https://172.26.120.20","EPSG:32654"},
			{"岩手県警察本部","https://172.26.120.25","EPSG:32654"},
			{"宮城県警察本部","https://172.26.120.28","EPSG:32654"},
			{"秋田県警察本部","https://172.26.120.33","EPSG:32654"},
			{"山形県警察本部","https://172.26.120.36","EPSG:32654"},
			{"福島県警察本部","https://172.26.120.41","EPSG:32654"},
			{"警視庁本部","https://172.26.120.49","EPSG:32654"},
			{"警視庁多摩本部","https://172.26.120.52","EPSG:32654"},
			{"茨城県警察本部","https://172.26.120.57","EPSG:32654"},
			{"栃木県警察本部","https://172.26.120.60","EPSG:32654"},
			{"群馬県警察本部","https://172.26.120.65","EPSG:32654"},
			{"埼玉県警察本部","https://172.26.120.68","EPSG:32654"},
			{"千葉県警察本部","https://172.26.120.73","EPSG:32654"},
			{"神奈川県警察本部","https://172.26.120.76","EPSG:32654"},
			{"新潟県警察本部","https://172.26.120.81","EPSG:32654"},
			{"山梨県警察本部","https://172.26.120.84","EPSG:32654"},
			{"長野県警察本部","https://172.26.120.89","EPSG:32654"},
			{"静岡県警察本部","https://172.26.120.92","EPSG:32654"},
			{"富山県警察本部","https://172.26.120.129","EPSG:32653"},
			{"石川県警察本部","https://172.26.120.132","EPSG:32653"},
			{"福井県警察本部","https://172.26.120.137","EPSG:32653"},
			{"岐阜県警察本部","https://172.26.120.140","EPSG:32653"},
			{"愛知県警察本部","https://172.26.120.145","EPSG:32653"},
			{"三重県警察本部","https://172.26.120.148","EPSG:32653"},
			{"滋賀県警察本部","https://172.26.120.153","EPSG:32653"},
			{"京都府警察本部","https://172.26.120.156","EPSG:32653"},
			{"大阪府警察本部","https://172.26.120.161","EPSG:32653"},
			{"兵庫県警察本部","https://172.26.120.164","EPSG:32653"},
			{"奈良県警察本部","https://172.26.120.169","EPSG:32653"},
			{"和歌山県警察本部","https://172.26.120.172","EPSG:32653"},
			{"鳥取県警察本部","https://172.26.120.177","EPSG:32653"},
			{"島根県警察本部","https://172.26.120.180","EPSG:32653"},
			{"岡山県警察本部","https://172.26.120.185","EPSG:32653"},
			{"広島県警察本部","https://172.26.120.188","EPSG:32653"},
			{"山口県警察本部","https://172.26.120.193","EPSG:32652"},
			{"徳島県警察本部","https://172.26.120.196","EPSG:32653"},
			{"香川県警察本部","https://172.26.120.201","EPSG:32653"},
			{"愛媛県警察本部","https://172.26.120.204","EPSG:32653"},
			{"高知県警察本部","https://172.26.120.209","EPSG:32653"},
			{"福岡県警察本部","https://172.26.120.212","EPSG:32652"},
			{"佐賀県警察本部","https://172.26.120.217","EPSG:32652"},
			{"長崎県警察本部","https://172.26.120.220","EPSG:32652"},
			{"熊本県警察本部","https://172.26.120.225","EPSG:32652"},
			{"大分県警察本部","https://172.26.120.228","EPSG:32652"},
			{"宮崎県警察本部","https://172.26.120.233","EPSG:32652"},
			{"鹿児島県警察本部","https://172.26.120.236","EPSG:32652"},
			{"沖縄県警察本部","https://172.26.120.241","EPSG:32652"},
			{"警察情報通信学校","https://172.26.121.1","EPSG:32654"}	
		};


	if(0 == STRCMP(msg, PSDLAUNCH_DEBUG_IP_FLAG)){

		//本番用サーバIPアドレス

			for(int num = 0; num <HEADQUATER_MAX; num ++ ){
				headqNamelist.push_back(da[num].headq_name);
				headqIplist.push_back(da[num].headq_ip);
				headqSRSlist.push_back(da[num].headq_srs);
		    }

	}else{

		//開発用サーバIPアドレス
	    headq_data da2[6] = 
	    {
			{"gm-police","https://gm-police.hitachi-solutions.co.jp","EPSG:32654"},
			{"gm-ibp","https://gm-ibp.hitachi-solutions.co.jp","EPSG:32654"},
			{"gm-psdap","https://gm-psdap.hitachi-solutions.co.jp","EPSG:32654"},
			{"gm-psdapdb1","https://gm-psdapdb1.hitachi-solutions.co.jp","EPSG:32654"},
			{"横浜ＳＴ","https://172.26.112.236","EPSG:32654"},
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
 * @brief WndSetDep デストラクタ
 */
WndSetDep::~WndSetDep()
{
	//デストラクタ
}

/**
 * @brief コントロールの初期化 WndBase::InitializeControl()オーバーライド
 */

bool WndSetDep::InitializeControl()
{
	LOG_DEBUG("WndSetDep::InitializeControl() %s", "");

	// title
	SetWindowName("本部選択");

	// リストボックス選択保存利用フラグ設定(LOAD)
	WndList::SetSaveSelectItem(true);

    listItems_.clear();


	listbox_ = GetListBox();
	if (0 == listbox_)
		return false;

    if(FALSE ==  IsExistFile( (const char*)ADSINFO_FILE )){

		// (タイトル + 空行で2行初期カウント)
		int row = GetTitleRowSize() + 1;

		SetPageLabel(txtNumColumns/2 + 2 ,row);

		row ++;

		// リスト最大表示件数取得
		InitListItemSize();
	    row += SetListPosition(1, row, txtNumColumns - 2);
		SetNumKeyExeType(KEY_NUMKEY_DIRECT);
		SetViewHeight(row);

	    // ListBox作成
	    SetPagingList();

	    // 現状のListBox状態でページ等の設定
		InitPageListEx(GetSelectIndexParam());

		// 選択状況復元
    	 WndList::LoadSelectItem();

	}

	return true;

}


/**
 * @brief ダイアログの表示処理
 */

void WndSetDep::ShowComfarmDialog()
{
	LOG_DEBUG("WndSetDep::ShowComfarmDialog() %s", "");

    char Name[256];
    SNPRINTF(Name, sizeof(Name), "%sが選択されました。宜しいですか？",
                                 headqNamelist.at(current_headq_index).c_str());

	LOG_DEBUG("WndSetDep::ShowComfarmDialog() headq     name = %s index = %d",
	                                           headqNamelist.at(current_headq_index).c_str(),
	                                           current_headq_index);

    MessageBox::ShowConf( Name, 8, "はい","いいえ");
    viewing_dialog_flag = TRUE;


}

/**
 * @brief FG、BGアプリ起動処理
 */
void WndSetDep::StartApple( const char* args )
{
	LOG_DEBUG("WndSetDep::StartApple().args = %s", "");
	LOG_DEBUG("%s", args);

	// PlaseWait画面描画
	SrvWndManager* swm = GmkUtil::GetWndManager();
    if(NULL == swm){
		LOG_ERROR("WndSetDep::StartApple(). Can't get SrvWndManager");
	}else{
		
		swm->Show(WNDID_FORCE_LOCK);
		// リスト画面クローズ
		WndPsdPageList::OnCloseWindow();
	}

	//BGアプリに終了通知
	if(ISHELL_PostEvent(GETSHELL(), AEECLSID_PSDBG, EVT_PSDBG_FG_STOP, 0, 0)){
		LOG_DEBUG("WndSetDep::StartApple(). Post EVT_PSDBG_FG_STOP to AEECLSID_PSDBG app");
	}else{
		LOG_ERROR("WndSetDep::StartApple(). error. Can't post EVT_PSDBG_FG_STOP ");
	}

	//アプリケーション引数作成
	char Buf[ADSINFO_MAX_STRINGS] = {};
    int read_size = sizeof(args);

	if((read_size <= 0) || (read_size > ADSINFO_MAX_STRINGS)){
		LOG_ERROR("WndSetDep::StartApple(). Can't set parameter");
	}else{
		STRNCPY(Buf, args , sizeof(Buf));
		LOG_DEBUG("WndSetDep::StartApple().ADS info =");
		LOG_DEBUG("%s", &Buf);
	}

	//他アプリケーション起動

	//Bluetooth接続アプリ
	#if 1
	int bg_ret = ECLASSNOTSUPPORT;

	bg_ret = ISHELL_StartBackgroundApplet(GETSHELL(),AEECLSID_PSDBG,(const char*)&Buf);
	if(SUCCESS == bg_ret || EALREADY == bg_ret){
		LOG_DEBUG("ISHELL_StartBackgroundApplet(AEECLSID_PSDBG) bg_ret = %d");
	}else{
		LOG_ERROR("ISHELL_StartBackgroundApplet(AEECLSID_PSDBG) error.bg_ret = %d");
	}
	#endif

	//データ端末アプリ起動
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
 * @brief ウインドウの開始処理
 */
boolean WndSetDep::OnBeginWindow()
{
	LOG_DEBUG("WndSetDep::OnBeginWindow()");

	//ファイル有無確認を行う。
	if( TRUE ==  IsExistFile( (const char*)ADSINFO_FILE ) ){
		StartApple("");						//アプリケーション起動
	}
	else
	{
		WndPsdPageList::OnBeginWindow();	//本部選択画面起動
	}
	return TRUE;
}

/**
 * @brief ウィンドウレジュームイベント
 */
void WndSetDep::OnResumeWindow()
{
	LOG_DEBUG("WndSetDep::OnResumeWindow()");

	if( viewing_dialog_flag ){
		// 確認ダイアログ描画
		ShowComfarmDialog();
	}

}

/**
 * @brief ウィンドウサスペンドイベント
 */
void WndSetDep::OnSuspendWindow(){
	LOG_DEBUG("WndSetDep::OnSuspendWindow()");
}

/**
 * @brief ウインドウの開始処理
 */
void WndSetDep::OnOpenWindow(){
	LOG_DEBUG("WndSetDep::OnOpenWindow()");
}

/**
 * @brief ウインドウの終了処理
 */
void WndSetDep::OnCloseWindow(){
	LOG_DEBUG("WndSetDep::OnCloseWindow()");
}


/**
 * @brief 初回起動判定ファイル読み出し処理
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
 * @brief 初回起動判定ファイル書込み処理
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
 * @brief 初回起動判定ファイル有無判定処理
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
 * @brief コマンドを処理する WndBase::HandleCommand()オーバーライド
 */
void WndSetDep::HandleCommand(const char* cmdStr, uint16 wParam)
{

	LOG_DEBUG("WndSetDep::HandleCommand()");

    // 確認ダイアログボタン押下時のダイアログで「はい」選択
    if(0 == STRCMP(CMD_PSD_DIALOG_YES, cmdStr))
    {

		LOG_DEBUG("WndSetDep::HandleCommand(CMD_PSD_DIALOG_YES)");

		//ダイアログ画面表示中フラグ削除
		viewing_dialog_flag = FALSE;
		

		// ADS情報文字列作成
		// "IPアドレス" + "SRS情報"
		cstring name = headqIplist.at(current_headq_index)
						+ "," + headqSRSlist.at(current_headq_index);
		
		LOG_DEBUG("本部名 = %s",headqNamelist.at(current_headq_index).c_str());
		LOG_DEBUG("IPアドレス + SRS情報 = %s",name.c_str());

		//共有フォルダファイルに情報を保存
		uint32 write_size;
		cstring empty = "";
		write_size = WriteFile( (char*)ADSINFO_FILE,(unsigned char*)empty.c_str(), empty.Length());

		//書き込み成功/不成功にかかわらず、アプリケーション起動
		StartApple(name.c_str());

    }
        // 確認ダイアログボタン押下時のダイアログで「いいえ」選択
    if(0 == STRCMP(CMD_PSD_DIALOG_NO, cmdStr))
    {
		LOG_DEBUG("WndSetDep::HandleCommand(CMD_PSD_DIALOG_NO)");

		//ダイアログ画面表示中フラグ削除
		viewing_dialog_flag = FALSE;

	}

	WndPsdPageList::HandleCommand(cmdStr, wParam);
}

/**
 * @brief イベントを処理する WndBase::HandleEvent()オーバーライド
 */
boolean WndSetDep::HandleEvent(AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    UNUSED_ALWAYS(wParam);
    UNUSED_ALWAYS(dwParam);
    UNUSED_ALWAYS(eCode);
    return FALSE;
}

/**
 * @brief リストコントロールにデータを設定
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
 * @brief リストの項目を選択したときに呼ばれるコマンド
 */
bool WndSetDep::SelectItemCommand(int index)
{
	LOG_DEBUG("WndSetDep::SelectItemCommand()");

	if ( FALSE == viewing_dialog_flag ){
		// ポップアップがまだ表示されていない場合
		
	    // ユーザが選択した本部番号を取得
		int OutPutSize = GetListItemSize();
		int PageNum = GetPageNum();

		current_headq_index = 0;
		current_headq_index = OutPutSize * (PageNum - 1) + index;

		LOG_DEBUG("WndSetDep::SelectItemCommand() headq_index = %d",current_headq_index);

		if(current_headq_index < 0 || current_headq_index >= HEADQUATER_MAX )
		{
			//異常値の場合は"東京"にセットしなおす
			current_headq_index = HEADQUATER_DEFAULT;
			LOG_ERROR("WndSetDep::SelectItemCommand() change headq_index = %d",current_headq_index);
		}

		// 確認ダイアログ描画
		ShowComfarmDialog();
	}
	else{
		// ポップアップがすでに表示されている場合
		LOG_DEBUG("WndSetDep::SelectItemCommand() Already show popup.");
	}
    return true;
}

bool WndSetDep::NextMoveWnd()
{
    return true;
}

/**
 * @brief ソフトキー初期化
 */
bool WndSetDep::InitializeSoftkey()
{
	if (!SetSoftkeyCenter("C_KETTEI","CmdNonAction","")) return false;

	return true;
}

