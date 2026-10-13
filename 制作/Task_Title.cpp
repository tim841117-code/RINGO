#include "MyPG.h"
#include "MyGameMain.h"

#include <windows.h>
#include <mmsystem.h>
//タイトル画面
namespace Title
{
	//ゲーム情報
	int				Timer;
	int				srcSet;
	DG::Image::SP   imgBG;
	DG::Image::SP	imgTitle;
	DG::Image::SP	imgPress;

	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		imgBG = DG::Image::Create("./data/image/grass_template2_0.png");
		imgTitle = DG::Image::Create("./data/image/title_text.png");
		imgPress = DG::Image::Create("./data/image/StartKey.png");

		Timer = 0;

		//シーン切り替えを分かりやすくするために入れた
		ge->dgi->EffectState().param.bgColor = ML::Color(1, 0, 0, 1);
	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		imgBG.reset();
		imgTitle.reset();
		imgPress.reset();
	}
	//-----------------------------------------------------------------------------
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();

		if (Timer >= 0 && Timer < 30) {
			++Timer;
			srcSet = 0;
		}
		else if (Timer >= 30 && Timer < 60) {
			++Timer;
			srcSet = 91;
		}
		else {
			Timer = 0;
		}

		TaskFlag rtv = TaskFlag::Title;//取りあえず現在のタスクを指定
		if (true == inp.SE.down) {
			PlaySoundA("./data/sound/se2.wav", NULL, SND_FILENAME | SND_ASYNC);
			rtv = TaskFlag::Game;
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理
	//-----------------------------------------------------------------------------
	void Render()
	{
		ML::Box2D  draw_BG(0, 0, 800, 600);
		ML::Box2D  src_BG(0, 0, 1280, 720);
		imgBG->Draw(draw_BG, src_BG);

		ML::Box2D  draw_TTL(59, 85, 680, 135);
		ML::Box2D  src_TTL(0, 0, 676, 88);
		imgTitle->Draw(draw_TTL, src_TTL);

		ML::Box2D  draw_PRS(102, 300, 596, 91);
		ML::Box2D  src_PRS(0, 0, 596, 91);
		src_PRS.Offset(0, srcSet);
		imgPress->Draw(draw_PRS, src_PRS);

	}
}