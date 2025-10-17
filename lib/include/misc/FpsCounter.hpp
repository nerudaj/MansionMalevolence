#pragma once

#include <string>

class [[nodiscard]] FpsCounter final
{
public:
	void update(const float dt);

	[[nodiscard]]
	constexpr const std::string& getText() const noexcept
	{
		return displayText;
	}

protected:
	std::string displayText;
};
