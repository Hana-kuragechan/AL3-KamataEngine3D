#pragma once
class TitleScene {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	//終了フラグのgetter
	bool IsFinished() const { return finished_; }
	//終了フラグ
	bool finished_ = false;
	
};
