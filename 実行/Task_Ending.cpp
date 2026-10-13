#include "MyPG.h"
#include "MyGameMain.h"

//エンディング
namespace Game
{
	extern int score;
}
namespace Ending
{
	//ゲーム情報
	int Timer;
	int srcSet;
	DG::Image::SP   img;
	DG::Image::SP imgGameOver;
	DG::Image::SP imgApple;
	DG::Image::SP imgNumber;
	DG::Image::SP imgPress;

	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		img = DG::Image::Create("./data/image/grass_template2_0.png");

		imgApple = DG::Image::Create("./data/image/cooltext352302625684791.png");
		imgNumber = DG::Image::Create("./data/image/no.png");
		imgPress = DG::Image::Create("./data/image/StartKey.png");
		Timer = 0;
		srcSet = 0;
	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		img.reset();
		imgApple.reset();
		imgNumber.reset();
		imgPress.reset();
	}
	//-----------------------------------------------------------------------------
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();
		Timer++;
		int score = Game::score;
		int tens = score / 10;
		int ones = score % 10;

		/*
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
		}*/
		if (Timer == 30) {
			PlaySoundA("./data/sound/se1.wav", NULL, SND_FILENAME | SND_ASYNC);
		}

		if (Timer == 40) {
			PlaySoundA("./data/sound/S.wav", NULL, SND_FILENAME);
		}

		if (Timer == 60) {
			PlaySoundA("./data/sound/se1.wav", NULL, SND_FILENAME | SND_ASYNC);
		}

		if (Timer == 70) {
			if (score >= 20) {
				PlaySoundA("./data/sound/M.wav", NULL, SND_FILENAME);
			}
			else {

				if (tens == 1) {
					PlaySoundA("./data/sound/s10.wav", NULL, SND_FILENAME);
				}

				if (ones != 0) {
					if (ones == 1) { PlaySoundA("./data/sound/s1.wav", NULL, SND_FILENAME); }
					if (ones == 2) { PlaySoundA("./data/sound/s2.wav", NULL, SND_FILENAME); }
					if (ones == 3) { PlaySoundA("./data/sound/s3.wav", NULL, SND_FILENAME); }
					if (ones == 4) { PlaySoundA("./data/sound/s4.wav", NULL, SND_FILENAME); }
					if (ones == 5) { PlaySoundA("./data/sound/s5.wav", NULL, SND_FILENAME); }
					if (ones == 6) { PlaySoundA("./data/sound/s6.wav", NULL, SND_FILENAME); }
					if (ones == 7) { PlaySoundA("./data/sound/s7.wav", NULL, SND_FILENAME); }
					if (ones == 8) { PlaySoundA("./data/sound/s8.wav", NULL, SND_FILENAME); }
					if (ones == 9) { PlaySoundA("./data/sound/s9.wav", NULL, SND_FILENAME); }
				}
				else {
					if (score == 0) {
						PlaySoundA("./data/sound/s0.wav", NULL, SND_FILENAME);
					}
				}
			}

		}

		if (Timer == 90) {
			PlaySoundA("./data/sound/se1.wav", NULL, SND_FILENAME | SND_ASYNC);
		}

		if (Timer % 60 < 30) {
			srcSet = 0;
		}
		else {
			srcSet = 1;
		}

		TaskFlag rtv = TaskFlag::Ending;//取りあえず現在のタスクを指定

		if (true == inp.SE.down) {
			PlaySoundA("./data/sound/se2.wav", NULL, SND_FILENAME | SND_ASYNC);
			rtv = TaskFlag::Title;	//次のタスクへ
		}

		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理
	//-----------------------------------------------------------------------------
	void Render()
	{
		ML::Box2D  draw(0, 0, 800, 600);
		ML::Box2D  src(0, 0, 1280, 720);
		img->Draw(draw, src);
		if (Timer >= 30) {
			ML::Box2D drawApple(100, 240, 216, 88);
			ML::Box2D srcApple(0, 0, 216, 88);
			imgApple->Draw(drawApple, srcApple);
		}
		if (Timer >= 60) {

			int score = Game::score;
			int tens = score / 10;   // 10
			int ones = score % 10;   // 0-9

			// 10
			ML::Box2D draw1(376, 240, 72, 96);
			ML::Box2D src1(tens * 24, 0, 24, 32);
			imgNumber->Draw(draw1, src1);

			// 0-9
			ML::Box2D draw2(448, 240, 72, 96);
			ML::Box2D src2(ones * 24, 0, 24, 32);
			imgNumber->Draw(draw2, src2);

		}
		if (Timer >= 90) {
			if (srcSet == 1) {
				ML::Box2D drawPress(204, 430, 390, 90);
				ML::Box2D srcPress(0, 0, 596, 91);
				imgPress->Draw(drawPress, srcPress);
			}
		}


	}
}