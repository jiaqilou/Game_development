#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Font.h"
#include <sstream>
#include <iomanip>

HUD::HUD(Game* game)
	:mGame(game)
	,mFont(nullptr)
{
	// Load font
	mFont = new Font();
	mFont->Load("Assets/Inconsolata-Regular.ttf");

	mTimerText = mFont->RenderText("00:00.00");
	time = 0;
	coin = 0;
	currentcoin = 0;
	mCoinText = mFont->RenderText("00/55");

	mCheckText = mFont->RenderText(" ");
}

HUD::~HUD()
{
	// Get rid of font
	if (mFont)
	{
		mFont->Unload();
		delete mFont;
	}
}

void HUD::Update(float deltaTime)
{
	time += deltaTime;
	mCheckpointTimer += deltaTime;
	mTimerText->Unload();
	delete mTimerText;
	int min = time / 60;
	std::string mintext = std::to_string(min);
	int second = ((int)time % 60);
	std::string sectext = std::to_string(second);
	int fra = (int)((time - (int)time)*100);
	std::string fratext = std::to_string(fra);
	std::string timetext;
	if (mintext.size() == 1) {
		timetext.append("0");
	}
	timetext.append(mintext);
	timetext.append(":");
	if (sectext.size() == 1) {
		timetext.append("0");
	}
	timetext.append(sectext);
	timetext.append(":");
	if (fratext.size() == 1) {
		timetext.append("0");
	}
	timetext.append(fratext);
	mTimerText = mFont->RenderText(timetext);
	//printf("time %f min %i sec %i fra %i \n",time, min,second,fra);
	// TODO
	if (currentcoin != coin) {
		mCoinText->Unload();
		delete mCoinText;
		currentcoin = coin;
		std::string cointext = std::to_string(currentcoin);
		std::string output;
		if (cointext.size() == 1) {
			output.append("0");
		}
		output.append(cointext);
		output.append("/55");
		mCoinText = mFont->RenderText(output);
	}

	if (mCheckpointTimer > 5) {
		mCheckText->Unload();
		delete mCheckText;
		

		mCheckText = mFont->RenderText(" ");
	}
}

void HUD::Draw(Shader* shader)
{
	DrawTexture(shader, mTimerText, Vector2(-420, -325));
	DrawTexture(shader, mCoinText, Vector2(-440, -300));
	DrawTexture(shader, mCheckText, Vector2::Zero);
	// TODO
}

void HUD::DrawTexture(class Shader* shader, class Texture* texture,
				 const Vector2& offset, float scale)
{
	// Scale the quad by the width/height of texture
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth()) * scale,
		static_cast<float>(texture->GetHeight()) * scale,
		1.0f);
	// Translate to position on screen
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(offset.x, offset.y, 0.0f));	
	// Set world transform
	Matrix4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	// Set current texture
	texture->SetActive();
	// Draw quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void HUD::checkpoint_text(std::string drawingtext) {
	/*mCheckText->Unload();
	delete mCheckText;
	Chetext = drawingtext;*/

	mCheckText = mFont->RenderText(drawingtext);
	mCheckpointTimer = 0;
}
