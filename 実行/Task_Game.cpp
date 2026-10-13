#include "MyPG.h"
#include "MyGameMain.h"
#include <cstdlib>
#include <ctime>
//bgm+se
#include <windows.h>
#include <mmsystem.h>

//ゲーム本編
namespace Game
{
	//ゲーム情報
	//int				player;
	int speedX, speedY, lv, score;
	bool isGameOver;
	int Timer;
	int srcSet;
	const int gamenW = 800;
	const int gamenH = 600;
	const int PlayerW = 64;
	const int PlayerH = 64;
	int playerTimer;
	int playerpicture;
	DG::Image::SP	img;
	DG::Image::SP   imgPlayer;
	DG::Image::SP   imgBG;
	DG::Image::SP   imgApple;
	DG::Image::SP imgNumber;
	DG::Image::SP imgGameOver;
	DG::Image::SP imgPress;
	enum class Angle {
		Down, Left, Up, Right, Non
	};
	struct Chara {
		int   x;
		int   y;
		Angle angle;
	};

	struct Apple {
		int   x;
		int   y;
	};

	Chara player;

	Apple apple;
	void Render_map();
	void Render_player();
	void Player_Update();
	void Render_apple();
	void Apple_where();
	void Apple_Update();
	void Render_score();
	void Render_gameover();
	void SE1();
	void SE2();
	void PlayBGM();
	void StopBGM();

	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		//シーン切り替えを分かりやすくするために入れた
		ge->dgi->EffectState().param.bgColor = ML::Color(1, 0, 1, 0);
		imgPlayer = DG::Image::Create("./data/image/chara.png");
		imgBG = DG::Image::Create("./data/image/grass_template2_0.png");
		imgApple = DG::Image::Create("./data/image/ringo.png");
		imgNumber = DG::Image::Create("./data/image/no.png");
		imgGameOver = DG::Image::Create("./data/image/gameOver.png");
		imgPress = DG::Image::Create("./data/image/StartKey.png");
		isGameOver = false;
		Timer = 0;
		//for random!!!!
		srand((unsigned int)time(NULL));
		//first apple 
		Apple_where();

		player.x = 400 - 32;
		player.y = 300 - 32;
		player.angle = Angle::Down;

		speedX = 0;
		speedY = 3;
		score = 0;
		lv = 1;
		srcSet = 0;
		playerTimer = 0;
		playerpicture = 0;
		PlayBGM();
	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		imgPlayer.reset();
		imgBG.reset();
		imgApple.reset();
		imgNumber.reset();
		StopBGM();
	}
	//-----------------------------------------------------------------------------
	//実行処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();

		if (isGameOver == true) {
			Timer++;

			if (inp.SE.down) {
				SE2();
				return TaskFlag::Ending;
			}

			return TaskFlag::Game;
		}




		Player_Update();

		player.x += speedX;
		player.y += speedY;

		Apple_Update();

		if (player.x < 0 || player.x > gamenW - PlayerW || player.y < 0 || player.y > gamenH - PlayerH)
		{
			isGameOver = true;
			speedX = 0;
			speedY = 0;
		}



		TaskFlag rtv = TaskFlag::Game;

		if (true == inp.ST.down) {
			SE2();
			rtv = TaskFlag::Title;
		}

		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理
	//-----------------------------------------------------------------------------
	void Render()
	{
		Render_map();
		Render_player();
		Render_apple();
		Render_score();
		if (isGameOver == true) {
			Render_gameover();
		}
	}


	//map----------------------------------------------------
	void Render_map() {
		ML::Box2D  draw_BG(0, 0, 800, 600);
		ML::Box2D  src_BG(0, 0, 1280, 720);
		imgBG->Draw(draw_BG, src_BG);
	}
	//player------------------------------------------
	void Render_player() {
		ML::Box2D draw(player.x, player.y, 64, 64);
		ML::Box2D src(playerpicture * 64, 0, 64, 64);
		if (player.angle == Angle::Down) { src.y = 0; }
		if (player.angle == Angle::Left) { src.y = 64; }
		if (player.angle == Angle::Right) { src.y = 128; }
		if (player.angle == Angle::Up) { src.y = 192; }

		imgPlayer->Draw(draw, src);
	}
	void Player_Update() {
		auto inp = ge->in1->GetState();
		int Speed = 3 + lv;
		playerTimer++;

		if (playerTimer >= 10) {
			playerTimer = 0;
			playerpicture++;

			if (playerpicture >= 3) {
				playerpicture = 0;
			}
		}
		if (inp.LStick.BL.down) { speedX = -Speed; speedY = 0; player.angle = Angle::Left; }
		if (inp.LStick.BR.down) { speedX = Speed; speedY = 0; player.angle = Angle::Right; }
		if (inp.LStick.BU.down) { speedX = 0; speedY = -Speed; player.angle = Angle::Up; }
		if (inp.LStick.BD.down) { speedX = 0; speedY = Speed; player.angle = Angle::Down; }
	}
	//apple------------------------------------------------
	void Render_apple() {
		ML::Box2D draw(apple.x, apple.y, 32, 32);
		ML::Box2D src(0, 0, 400, 400);
		imgApple->Draw(draw, src);
	}
	void Apple_where()
	{
		apple.x = rand() % (800 - 32);
		apple.y = rand() % (600 - 32);
	}
	void Apple_Update()
	{
		bool applehitX = player.x < apple.x + 32 && player.x + 64 > apple.x;
		bool applehitY = player.y < apple.y + 32 && player.y + 64 > apple.y;

		if (applehitX && applehitY)
		{
			score++;
			if (lv < 7) { lv++; }
			SE1();
			Apple_where();
		}
	}



	//POINT------------------------------------------------
	void Render_score() {
		int tens = score / 10;   // 10
		int ones = score % 10;   // 0-9

		// 10
		ML::Box2D draw1(376, 20, 24, 32);
		ML::Box2D src1(tens * 24, 0, 24, 32);
		imgNumber->Draw(draw1, src1);

		// 0-9
		ML::Box2D draw2(400, 20, 24, 32);
		ML::Box2D src2(ones * 24, 0, 24, 32);
		imgNumber->Draw(draw2, src2);
	}


	//music------------------------------------------------
	void SE1()
	{
		PlaySoundA("./data/sound/se1.wav", NULL, SND_FILENAME | SND_ASYNC);
	}

	void SE2()
	{
		PlaySoundA("./data/sound/se2.wav", NULL, SND_FILENAME | SND_ASYNC);
	}

	void PlayBGM()
	{
		mciSendStringA("open \"./data/sound/bgm.mp3\" alias bgm", NULL, 0, NULL);
		mciSendStringA("play bgm repeat", NULL, 0, NULL);
	}

	void StopBGM()
	{
		mciSendStringA("stop bgm", NULL, 0, NULL);
		mciSendStringA("close bgm", NULL, 0, NULL);
	}

	void Render_gameover()
	{
		ML::Box2D drawGO(191, 200, 418, 74);
		ML::Box2D srcGO(0, 0, 418, 74);
		imgGameOver->Draw(drawGO, srcGO);


		if (Timer % 60 < 30) {
			srcSet = 0;
		}
		else {
			srcSet = 1;
		}


		if (srcSet == 1) {
			ML::Box2D drawPress(204, 430, 390, 90);
			ML::Box2D srcPress(0, 0, 596, 91);
			imgPress->Draw(drawPress, srcPress);
		}
	}

}