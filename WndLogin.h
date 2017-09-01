/*********************************************************************/
/*                       Keitai/Police                               */
/*                                                                   */
/*     ALL RIGHTS RESERVED.                                          */
/*     COPYRIGHT (C) 2010,  HITACHI SOFTWARE ENGINEERING CO.,LTD.    */
/*     Hitachi Software Engineering Co., Ltd. Confidential           */
/*********************************************************************/

/**
 * @file    WndLogin.cpp
 * @brief   ログイン画面
 * 
 * PSD/データ端末アプリのログイン画面クラスです。<BR>
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

#define WNDID_LOGIN   WNDID_PREFIX "Login"  //!< 画面ID

#define FUNCID_0001010          "DW_0001010"    //!< ファンクションID
#define DATATYPE_0001010        "Login"         //!< データタイプ
#define SESSIONID_0001010       FUNCID_0001010  //!< セッションID

//! アプリバージョンチェック完了時実行コマンド(初回活動状況通知トリガー)
#define CMD_FINISHED_VER_CHK    "CmdFinishedVerChk"

class HSFWImage;
class MultitaskInterface;
class AppVerChk;

/**
 * @brief ログイン画面
 *
 * ログイン画面クラスです。<BR>
 * ログイン画面では、直接入力によるユーザID/パスワード入力を実施します。<BR>
 * ペアリングの実施可否を選択してのログインが可能です。
 *
 */
class WndLogin : public WndPsdBase
{

private:
    HSFDirectText* txtMemberId_;            //!< ユーザIDテキストコントロール
    HSFDirectText* txtPassword_;            //!< パスワードテキストコントロール
    HSFStaticCtl* stsVersion_;
    HSFPictButtonCtl* btnLogin_;            //!< ログインボタンコントロール
    HSFPictButtonCtl* btnPairing_;          //!< ペアリングボタンコントロール

    smart_ptr<HSFWImage> imgLabelUserID_ ;  //!< ユーザIDラベル画像
    smart_ptr<HSFWImage> imgLabelPass_ ;    //!< パスワードラベル画像

    // フラグ系パラメータ
    int     login_result_;                  //!< ログイン結果

    //! 認証要求通信処理コマンド
    auto_ptr<ServerCommandLogin> m_LoginCommand;

    //! 活動状況通知0001通信処理コマンド
    auto_ptr<ServerCommandSendStatusInit> m_SendStatusInitCommand;

    //! マスタ取得要求通信処理コマンド
    auto_ptr<ServerCommandGetMaster> m_SendGetMasterCommand;

    //! アプリバージョン情報
    AppVerChk* m_AppVerMng;

    typedef AEETextInputMode InputMode;

    InputMode inputmode_;           //!< テキスト入力モード

    InputMode m_modeUserId;         //!< ユーザID欄入力モード
    InputMode m_modePassword;       //!< パスワード欄入力モード

    bool initailed_;                //!< 初回起動フラグ
    int foucusIndex_;               //!< コントロールのフォーカス位置

    MultitaskInterface* nextwnd_task_;      //!< 画面遷移タスク(非同期)

    smart_ptr<HSFImage>   ResSuujiImg_;   //!< 数字キー画像
    smart_ptr<HSFImage>   ResEijiImg_;    //!< 英字キー画像
	bool SendStatusInitFinish_;     //!< 活動状況通知0001通信処理コマンド送信完了フラグ

public:

    /**
     * @brief コンストラクタ
     */
    WndLogin();

    /**
     * @brief デストラクタ
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
     * @brief コントロールの初期化
     * 
     * 画面コントロールの初期化処理を実施する。
     * ログイン画面状態別に以下の処理を実施する。
     * <li>
     *     <ol>
     *     <li>強制ロック解除通知：活動状況通知0003をリクエスト</li>
     *     <li>ペアリング切断による強制ログアウト：活動状況通知0003をリクエスト</li>
     *     </ol>
     * </li>
     *
     * @retval true コントロールの初期化成功 
     * @retval false コントロールの初期化失敗（この場合画面描画されません）
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
     * @brief テロップ利用有無を取得
     *
     * @retval true テロップを利用する
     * @retval false テロップを利用しない
     */
    virtual bool IsTelopUsed() { return false; }

    /**
     * @brief 画面表示前処理を行う。
     *
     * 画面の描画処理の直後に呼び出される。<BR>
     * 通信状態の通知。および通信リクエストエラー時の通知を行う。
     *
     * @retval TRUE 処理済
     * @retval FALSE 未処理
     * @sa WndBase::OnBeginWindow()
     */
    virtual boolean OnBeginWindow();

    /**
     * @brief 画面描画の後処理を行う。
     *
     * @retval TRUE 処理済
     * @retval FALSE 未処理
     * @sa WndBase::OnDrawOtherAfter()
     */
    boolean OnDrawOtherAfter();    

    /**
     * @brief 直接テキスト入力の入力モード取得
     *
     * フォーカスの当たっている直接テキスト入力の入力モードを取得する。<BR>
     *
     * @param [in] dt フォーカスが当たった直接テキスト入力コントロール
     */
    void TextFocusInNotify(HSFDirectText* dt);

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
	 * @brief キーイベントをフックする。
	 *
     * キーイベント処理の処理よりも先に呼び出される。<BR>
     * 戻り値によって、以降の処理を呼び出すかどうかをコントロールする。<BR>
     *
	 * @param [in] individualObject 個別オブジェクト
	 * @param [in] eCode イベント
	 * @param [in] wParam キーコード
	 * @param [in] dwParam 修飾キーフラグ
	 * @retval TRUE 処理済(以降のキーイベント処理、コマンドハンドル等の処理が呼び出されない)
	 * @retval FALSE 未処理(以降のキーイベント処理、コマンドハンドル等の処理を呼び出す)
	 */
	virtual boolean	HookKeyEvent(void *IndividualObject, AEEEvent eCode, uint16 wParam, uint32 dwParam);

    /**
     * @brief キーイベント処理
     *
     * 各キー別に操作時の処理を実施する。<BR>
     * 上下方向キー操作時のテロップ表示件名の変更、<BR>
     * 左右方向キー操作時のページ遷移処理の呼び出し、<BR>
     * 数字キー押下時の一斉指令情報選択時処理の呼び出しを行う。
     * 
     * @param [in] wParam イベントコード
     * @param [in] dwParam イベントコードの詳細オブジェクト
     * @retval TRUE 正常
     * @retval FALSE 異常
     * @sa WndBase::EventKey()
     */
    boolean EventKey(uint16 wParam, uint32 dwParam);

protected:

    /**
	 * @brief ウインドウのサスペンド処理
     *
	 * サスペンドのタイミングで呼ばれます。<br>
	 */
    virtual void OnSuspendWindow();

    /**
     * @brief 起動時GPS測位処理の呼び出しを行う。
     *
     */
    void Request_GpsStatus();

    /**
     * @brief 活動状況通知0001リクエストデータの設定、通信処理の呼び出しを行う。
     *
     * @param param　GPS測位情報
     */
    void SendRequest_SendStatusInit(GpsStatus* param) { SendRequest_SendStatusInitImpl(param);} ;

    /**
     * @brief 活動状況通知0001リクエストデータの設定、再通信処理の呼び出しを行う。
     *
    * @retval true 通信リクエスト処理成功
     * @retval false 通信リクエスト処理失敗
     */
    bool SendRequest_ReSendStatusInit() { return SendRequest_SendStatusInitImpl(NULL, true);};

    /**
     * @brief 認証要求リクエストデータの設定、通信処理の呼び出しを行う。
     *
     * @param param　GPS測位情報
     *
     * @retval true 通信リクエスト処理成功
     * @retval false 通信リクエスト処理失敗
     */
    void SendRequest_Login(GpsStatus* param) { SendRequest_LoginImpl(param);};

    /**
     * @brief マスタ取得リクエストデータの設定、通信処理の呼び出しを行う。
     *
     *
     * @retval true 通信リクエスト処理成功
     * @retval false 通信リクエスト処理失敗
     */
    bool SendRequest_GetMaster();

    /**
     * @brief 通信イベントのレスポンス通知処理
     * <li>
     *     <ol>
     *     <li>活動状況通知0001を処理</li>
     *     </ol>
     * </li>
     * @param [in] wParam イベントコード
     * @param [in] dwParam イベントコードの詳細オブジェクト
     * @retval true 正常
     * @retval false 異常
     */
    virtual boolean OnResNotify_SendStatusInit(uint16 wParam, uint32 dwParam);


    /**
     * @brief 通信イベントのレスポンス通知処理（ログインのみ実施）
     * <li>
     *     <ol>
     *     <li>認証要求を処理</li>
     *     </ol>
     * </li>
     * @param [in] wParam イベントコード
     * @param [in] dwParam イベントコードの詳細オブジェクト
     * @retval true 正常
     * @retval false 異常
     */
    virtual boolean OnResNotify_Login(uint16 wParam, uint32 dwParam);


    /**
     * @brief 通信イベントのレスポンス通知処理（ログインのみ実施）
     * <li>
     *     <ol>
     *     <li>認証要求を処理</li>
     *     </ol>
     * </li>
     * @param [in] wParam イベントコード
     * @param [in] dwParam イベントコードの詳細オブジェクト
     * @retval true 正常
     * @retval false 異常
     */
    virtual boolean OnResNotify_GetMaster(uint16 wParam, uint32 dwParam);


    enum INPUT_CHECK {
        CHK_SUCCESS,
        CHK_NULL_ERROR,
        CHK_SIZE_ERROR
    };

    /**
     * @brief 入力チェック
     *
     * @return 入力チェック結果
     */
    INPUT_CHECK InputDataCheck();

    /** 
     * @breif 入力モードの絶定
     * @param [in] mode 入力モード
     * @param [in] bUpdate 表示更新有無
     */
    void setInputMode(InputMode mode, bool bUpdatee);

    /** 
     * @breif 入力モードの描画
     * 入力モードを描画します。
     */
    void drawInputMode();


private:

    /**
     * @brief 入力フィールドの初期化。
     *
     */
    void ClearInputFeild();
    
    /**
     * @brief フォーカス位置を返します。
     * 
     * @return フォーカス位置のインデックス
     */
    int GetFoucusPos();


    /**
     * @brief フォーカス位置を設定します。
     * 
     * @param フォーカス位置のインデックス
     */
    void SetFoucusPos(int index);

    /**
     * @brief ペアリングモードであるかを判定
     *
     * @retval true ペアリングモード
     * @retval false 通常モード（ペアリングなし）
     */ 
    bool IsPairingMode();

    /**
     * @brief ログイン認証成功時の次画面遷移
     *
     */ 
    void NextWindowCommand();


    /**
     * @brief ボタンの有効可否設定
     * @param enable true:有効な場合 false:無効な場合
     */ 
    void SetButtonEnable(bool enable);

    /**
     * @brief ボタンの有効可否
     * @retval true 有効
     * @retval false 無効
     */ 
    bool IsButtonEnable();

    /**
     * @brief パスワードをMD5ハッシュ関数
     * @param key 変換元文字列
     * @return  変換後文字列
     */
    const cstring MakeKeyHash(const char* key);

    /**
     * @brief 活動状況通知0001リクエストデータの設定、通信処理の呼び出しを行う。
     *
     * @param reSendCommand　再送フラグ　再送の場合はtrue
     * @retval true 通信リクエスト処理成功
     * @retval false 通信リクエスト処理失敗
     */
    bool SendRequest_SendStatusInitImpl(GpsStatus* param, bool reSendCommand = false);

    /**
     * @brief 認証要求リクエストデータの設定、通信処理の呼び出しを行う。
     *
     * @param param　GPS測位情報
     *
     * @retval true 通信リクエスト処理成功
     * @retval false 通信リクエスト処理失敗
     */
    bool SendRequest_LoginImpl(GpsStatus* param);


    /**
     * @brief ログインボタンの押下処理
     */
    void LoginButtonPush();


    /**
     * @brief ログイン画面によるローカルの通信が実施中であるか判定
     * @retrun 通信状態
     * @retval true 実施中
     * @retval false 未実施
     */
    bool IsLoacalHttps();

    /**
     * @brief 活動状況通知0001リクエストデータの作成を行う。
     *
     * @param param　GPS測位情報
     */
	void CreateRequest_SendStatusInit(GpsStatus* param);

};

#endif // WND_LOGIN_H__
