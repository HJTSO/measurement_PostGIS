/*********************************************************************/
/*                           GeoMation                               */
/*                                                                   */
/*     ALL RIGHTS RESERVED.                                          */
/*     COPYRIGHT (C) 2010,  HITACHI SOFTWARE ENGINEERING CO.,LTD.    */
/*     Hitachi Software Engineering Co., Ltd. Confidential           */
/*********************************************************************/

/**
* @file    WndSetDep.h
* @brief   部門入力画面
*
* $Author: tomohiro.kurokawa.ev@ps.hitachi-solutions.com $
* $Date:: 2010-10-19 20:55:42#$
*/

#ifndef WND_SET_DEP_H__
#define WND_SET_DEP_H__

#include "WndPsdPageList.h"

#define WNDID_SET_DEP           WNDID_PREFIX "SetDep"       //!< 画面ID
//#define FUNCID_0003020          "DW_0003020"                //!< ファンクションID
#define FUNCID_0003020          "DW_0021010"
#define DATATYPE_040            "setdep"

#include <AEEFile.h>

/**
* @brief 部門入力画面
*
* 部門入力画面クラスです。<BR>
* 部門入力画面では、ログイン時に受信した部門マスタ情報をもとに部門を選択させるリストを表示します。<BR>
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
    
    //ダイアログ画面表示中フラグ
    bool viewing_dialog_flag;

public:
    /**
    * @brief コンストラクタ
    */
    WndSetDep();

    /**
    * @brief デストラクタ
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
    * @brief コントロールを初期化する。
    *
    * @return 結果
    * @sa WndBase::InitializeControl()
    */
    virtual bool InitializeControl();

    /**
    * @brief コマンドハンドル
    * 
    * 各コマンド別に操作時の処理を実施します。
    *
    * @param cmdStr コマンド文字列
    * @param wParam イベントコードの詳細オブジェクト
    *
    * @sa WndBase::HandleCommand()
    */
    virtual void HandleCommand(const char* cmdStr, uint16 wParam);

    /**
    * @brief ExclusionMgrによるイベントハンドル
    * イベントをハンドルします。HandleCommandやEventKeyよりも先に呼び出されます。
    *
    * @param wParam イベントコード
    * @param dwParam イベントコードの詳細オブジェクト
    * @retval true 正常
    * @retval false 異常
    * @sa HSFBaseObjSC::HandleEvent()
    */
    virtual boolean HandleEvent(AEEEvent eCode, uint16 wParam, uint32 dwParam);

	/**
	 * @brief ファイルより読み込み復号化
	 *
	 * ファイルを先頭から読み込み、指定バッファに復号して格納する。<br>
	 * 引数 buf に NULL が設定された場合は、読込はせずファイルサイズのみ返す。<br>
	 *
	 * @param [in]  pPath ファイルパス
	 * @param [out] pBuf バッファ
	 * @param [in]  iBufSize バッファサイズ
     *
	 * @return 読み込みサイズ
	 */
    int32  WndSetDep::ReadFile(char* pPath, unsigned char* pBuf, int iBufSize);

	/**
	 * @brief ファイルに書き込む
	 *
	 * 指定バッファをファイル出力する。<br>
	 * 既存ファイルがある場合は上書き、ない場合は新規でファイルを作成する。<br>
	 *
	 * @param [in]  pPath ファイルパス
	 * @param [in]  pBuf バッファ
	 * @param [in]  iBufSize バッファサイズ
     *
	 * @return 書き込みサイズ
	 */
    uint32 WndSetDep::WriteFile(char* pPath, unsigned char* pBuf, int iBufSize);

	/**
	 * @brief ファイル有無判定
	 *
	 * ファイルの有無を確認する<br>
	 * 既存ファイルがある場合は上書き、ない場合は新規でファイルを作成する。<br>
	 *
	 * @param [in]  pPath ファイルパス
     *
	 * @return ファイル有無
	 */
    int32  WndSetDep::IsExistFile(const char* pPath);


	/**
	* @brief 再開処理(レジューム)
	*/
	virtual void OnResumeWindow();


    /**
     * @brief サスペンド発生時処理
     * 
     */
    virtual void OnSuspendWindow();
    
    /**
     * @brief 画面表示前処理を行う。
     *
     * 画面の描画処理の直後に呼び出される。<BR>
     *
     * @retval TRUE 処理済
     * @retval FALSE 未処理
     * @sa WndBase::OnBeginWindow()
     */
    virtual boolean OnBeginWindow();

	/**
	 * @brief ウインドウの開始処理
     *
	 * 画面へ遷移するタイミングで呼ばれます。<br>
     * レジューム(リフレッシュ)時は呼ばれません。<br>
	 */
	virtual void OnOpenWindow();

	/**
	 * @brief ウインドウの終了処理
     *
	 * 画面が遷移するタイミングで呼ばれます。<br>
     * サスペンド(リフレッシュ)時は呼ばれません。<br>
	 */
    virtual void OnCloseWindow();

	/**
	 * @brief 確認ダイアログ表示処理
     *
	 * ダイアログを表示します。<br>
	 */
	void WndSetDep::ShowComfarmDialog();

	/**
	 * @brief FG、BGアプリ起動処理
     *
	 * FG、BGアプリ起動処理します。<br>
	 * 起動アプリは、何もない画面に遷移してサスペンドを待ちます。<br>
	 */
    void  WndSetDep::StartApple(const char *args);


	/**
	 * @brief ソフトキーの初期化
     *
	 * ソフトキーを初期化します。<br>
	 */
    virtual bool InitializeSoftkey();
 
protected:

    /**
    * @brief リストの項目を選択したときに呼ばれるコマンド
    * 
    * @param index 選択するアイテムのインデックス
    * @retval true 正常
    * @retval false 異常
    */
    virtual bool SelectItemCommand(int index);

private:

    /**
    * @brief 全てのリスト情報をページリストに設定
    * 
    * @retval true 正常
    * @retval false 異常
    */
    bool SetPagingList();

    /**
    * @brief 次画面遷移
    * 
    * @retval true 正常
    * @retval false 異常
    */
    bool NextMoveWnd();
};

#endif // WND_SET_DEP_H__
