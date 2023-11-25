#pragma once

#include "BunnyEngine/Renderer/Texture.h"
#include "imgui.h"

namespace BE
{
	class StaticMesh;
}

namespace BE::UI
{
	enum ButtonType : int
	{
		None = 0,
		OK = 0b00000001,
		Cancel = 0b00000010,
		OKCancel = 0b00000011,
		Yes = 0b00000100,
		No = 0b00001000,
		YesNo = 0b00001100,
		YesNoCancel = 0b00001110
	};

	bool DrawTextureSelection(const std::string& label, Ref<Texture>& modifyingTexture, bool bLoadAsSRGB);
	bool DrawStaticMeshSelection(const std::string& label, Ref<StaticMesh>& staticMesh, const std::string& helpMessage = "");
	bool DrawSoundSelection(const std::string& label, std::filesystem::path& selectedSoundPath);
	bool DrawVec3Control(const std::string& label, glm::vec3& values, const glm::vec3 resetValues = glm::vec3{ 0.f }, float columnWidth = 100.f);

	//Grid Name needs to be unique
	void BeginPropertyGrid(const std::string& gridName);
	void EndPropertyGrid();

	bool Property(const std::string& label, std::string& value, const std::string& helpMessage = "");
	bool Property(const std::string& label, bool& value, const std::string& helpMessage = "");
	bool Property(const std::string& label, const std::vector<std::string>& customLabels, bool* values, const std::string& helpMessage = "");

	bool PropertyText(const std::string& label, const std::string& text);

	bool PropertyDrag(const std::string& label, int& value, float speed = 1.f, int min = 0, int max = 0, const std::string& helpMessage = "");
	bool PropertyDrag(const std::string& label, float& value, float speed = 1.f, float min = 0.f, float max = 0.f, const std::string& helpMessage = "");
	bool PropertyDrag(const std::string& label, glm::vec2& value, float speed = 1.f, float min = 0.f, float max = 0.f, const std::string& helpMessage = "");
	bool PropertyDrag(const std::string& label, glm::vec3& value, float speed = 1.f, float min = 0.f, float max = 0.f, const std::string& helpMessage = "");
	bool PropertyDrag(const std::string& label, glm::vec4& value, float speed = 1.f, float min = 0.f, float max = 0.f, const std::string& helpMessage = "");

	bool PropertySlider(const std::string& label, int& value, int min, int max, const std::string& helpMessage = "");
	bool PropertySlider(const std::string& label, float& value, float min, float max, const std::string& helpMessage = "");
	bool PropertySlider(const std::string& label, glm::vec2& value, float min, float max, const std::string& helpMessage = "");
	bool PropertySlider(const std::string& label, glm::vec3& value, float min, float max, const std::string& helpMessage = "");
	bool PropertySlider(const std::string& label, glm::vec4& value, float min, float max, const std::string& helpMessage = "");

	bool PropertyColor(const std::string& label, glm::vec3& value, const std::string& helpMessage = "");
	bool PropertyColor(const std::string& label, glm::vec4& value, const std::string& helpMessage = "");

	bool InputFloat(const std::string& label, float& value, float step = 0.f, float stepFast = 0.f, const std::string& helpMessage = "");
	
	//Returns true if selection changed.
	//outSelectedIndex - index of the selected option
	bool Combo(const std::string& label, uint32_t currentSelection, const std::vector<std::string>& options, int& outSelectedIndex, const std::vector<std::string>& tooltips = {}, const std::string& helpMessage = "");
	bool Button(const std::string& label, const std::string& buttonText, const ImVec2& size = ImVec2(0, 0));

	void Tooltip(const std::string& tooltip, float treshHold = BE_HOVER_THRESHOLD);

	void PushItemDisabled();
	void PopItemDisabled();

	void PushFrameBGColor(const glm::vec4& color);
	void PopFrameBGColor();

	void HelpMarker(const std::string& text);

	ButtonType ShowMessage(const std::string& title, const std::string& message, ButtonType buttons);
	ButtonType InputPopup(const std::string& title, const std::string& hint, std::string& input);
}

namespace BE::UI::TextureViewer
{
	// outWindowOpened - In case X button will be clicked, this flag will be set to false.
	// outWindowOpened - if nullptr set, windows will not have X button 
	void OpenTextureViewer(const Ref<Texture>& textureToView, bool* outWindowOpened = nullptr);
}